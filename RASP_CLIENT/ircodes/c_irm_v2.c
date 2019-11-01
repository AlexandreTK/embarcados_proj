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


bool blocked_execution_falling = true;
bool blocked_execution_rising = true;



long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}
/*
void unblock_execution_falling() {
	blocked_execution_falling = false;
	//printf("UNBLOCKING FALL\n");
	//system ("/usr/local/bin/gpio edge pin none") ;
	return;
}
void unblock_execution_rising() {
	blocked_execution_rising = false;
	return;
}
*/

bool IRStart() {
	long timeRisingEdge = 0;
	long timeFallingEdge[2] = {0, 0};
	long timeSpan[2] = {0, 0};
	
/*
	blocked_execution_falling = true;		
	//wiringPiISR(PIN_IR, INT_EDGE_FALLING, &unblock_execution_falling);
	while(blocked_execution_falling) {
		usleep(1);
	}
	blocked_execution_falling = true;
*/	
	while(digitalRead(PIN_IR) == 1) {
		usleep(5);
	}
	timeFallingEdge[0] = getMicrotime();
	//printf("fall\n");
/*
	blocked_execution_rising = true;
	//wiringPiISR(PIN_IR, INT_EDGE_RISING, &unblock_execution_rising);
	while(blocked_execution_rising) {
		usleep(1);
	}
	blocked_execution_rising = true;
*/
	while(digitalRead(PIN_IR) == 0) {
		usleep(5);
	}
	timeRisingEdge = getMicrotime();
	//printf("rise\n");
/*
	blocked_execution_falling = true;		
	//wiringPiISR(PIN_IR, INT_EDGE_FALLING, &unblock_execution_falling);
	while(blocked_execution_falling) {
		usleep(1);
	}
	blocked_execution_falling = true;
*/
	while(digitalRead(PIN_IR) == 1) {
		usleep(5);
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
	// Comment...
	
	int i;
	for(i=0; i<8; i++) {
		/*
		blocked_execution_rising = true;
		//wiringPiISR(PIN_IR, INT_EDGE_RISING, &unblock_execution_rising);
		while(blocked_execution_rising) {
			usleep(1);
		}
		blocked_execution_rising = true;
		*/
		while(digitalRead(PIN_IR) == 0) {
			usleep(5);
		}
		timeRisingEdge = getMicrotime();
		/*
		blocked_execution_falling = true;		
		//wiringPiISR(PIN_IR, INT_EDGE_FALLING, &unblock_execution_falling);
		while(blocked_execution_falling) {
			usleep(1);
		}
		blocked_execution_falling = true;
		*/
		while(digitalRead(PIN_IR) == 1) {
			usleep(5);
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
	// 0 -> ADDRESS
	// 1 -> LOGICAL INVERSE OF ADDRESS
	// 2 -> COMMAND
	// 3 -> LOGICAL INVERSE OF COMMAND
	if (IRStart() == false) {
		usleep(110000);
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
/*
	wiringPiISR(PIN_IR, INT_EDGE_RISING, &unblock_execution_rising);
	wiringPiISR(PIN_IR, INT_EDGE_FALLING, &unblock_execution_falling);
*/

	while(1) {
		key = getKey();
		printf("Raw %x\n", key);

		if(key != ERROR) {
			printf("%x\n", key);
		}
		
	}
	return 0;
}
