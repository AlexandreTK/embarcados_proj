// IMPLEMENTATION OF NEC Infrared Transmission Protocol
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <wiringPi.h>
#include <stdbool.h> 

#include <time.h> 
#include <sys/time.h>

#define ERROR 0xFE
#define PIN_IR 0 //GPIO17 WPI_PIN0

#define FULL_CYCLE_USEC 110000
#define SLEEP_USEC 10

bool blocked_execution_falling = true;
bool blocked_execution_rising = true;



long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

bool IRStart() {
	long timeRisingEdge = 0;
	long timeFallingEdge[2] = {0, 0};
	long timeSpan[2] = {0, 0};
	// Start signal is composed with a 9 ms leading space and a 4.5 ms pulse.
	
	while(digitalRead(PIN_IR) == 1) {
		usleep(SLEEP_USEC);
	}
	timeFallingEdge[0] = getMicrotime();
	while(digitalRead(PIN_IR) == 0) {
		usleep(SLEEP_USEC);
	}
	timeRisingEdge = getMicrotime();
	while(digitalRead(PIN_IR) == 1) {
		usleep(SLEEP_USEC);
	}
	timeFallingEdge[1] = getMicrotime();

	timeSpan[0] =  timeRisingEdge - timeFallingEdge[0];
	timeSpan[1] =  timeFallingEdge[1] - timeRisingEdge;

	if ((timeSpan[0] > 8500) && (timeSpan[0] < 9500) &&  (timeSpan[1] > 4000) && (timeSpan[1] < 5000) ) {
		return true;
	} else {
		return false;
	}

	//return false;
}

char getByte() {
	char byte = 0;
	long timeRisingEdge = 0;
	long timeFallingEdge = 0;
	long timeSpan = 0;
    	// Logic '0' == 0.56 ms LOW and 0.56 ms HIGH
    	// Logic '1' == 0.56 ms LOW and 0.169 ms HIGH
	int i;
	for(i=0; i<8; i++) {
		
		while(digitalRead(PIN_IR) == 0) {
			usleep(SLEEP_USEC);
		}
		timeRisingEdge = getMicrotime();
		
		while(digitalRead(PIN_IR) == 1) {
			usleep(SLEEP_USEC);
		}
		timeFallingEdge = getMicrotime();
		
		timeSpan = timeFallingEdge - timeRisingEdge;

		if ((timeSpan > 1600) && (timeSpan < 1800)) {
			byte |= 1 << i;
		}
	}
	return byte;
}

char getKey() {
	char bytes[4]= {0, 0, 0, 0};
	// bytes[0] -> ADDRESS
	// bytes[1] -> LOGICAL INVERSE OF ADDRESS
	// bytes[2] -> COMMAND
	// bytes[3] -> LOGICAL INVERSE OF COMMAND
	if (IRStart() == false) {
		usleep(FULL_CYCLE_USEC); // One message frame lasts 108 ms.
		return ERROR;
	} else {
		int i;
		for(i=0; i<4; i++) {
			bytes[i] = getByte();
		}
		if ((bytes[0]+bytes[1]==0xFF) && (bytes[2]+bytes[3]==0xFF)) {
			return bytes[2];
		} else {
			return ERROR;
		}
	}
}




int main(int argc, char* argv[]) {
	char key;

	printf("IRM Test Start ...\n");

	wiringPiSetup();
	
	pinMode(PIN_IR, INPUT);
	pinMode(PIN_IR, PUD_UP);

	while(1) {
		key = getKey();
		//printf("Raw %x\n", key);

		if(key != ERROR) {
			printf("%x\n", key);
		}
		
	}
	return 0;
}
