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



//************* SERVER DEFINITION ***************
#define SECONDS_NEXT_REQUEST 2
//static char GET_ALARM_INFO_URL[] = "http://raspberrypi.local:3000/get_alarm";
static char GET_ALARM_INFO_URL[] = "http://localhost:3000/get_alarm";

static const char MUSIC_PLAYER[] = "mplayer";
#ifdef __APPLE__
static const char MUSIC_PLAYER_PATH[] = "/usr/local/bin/mplayer";
#else
static const char MUSIC_PLAYER_PATH[] = "/usr/bin/mplayer";
#endif

pid_t fork_song_pid = 0;
//*******************************************


//************* MOTOR DEFINITION ***************
#define RELAY_1 2
#define RELAY_2 3
#define RELAY_ENABLE  HIGH
#define RELAY_DISABLE LOW
//*******************************************


//************* IR DEFINITION ***************
#define ERROR 0xFE
#define PIN_IR 0 //GPIO17 WPI_PIN0

#define FULL_CYCLE_USEC 110000
#define SLEEP_USEC 10
//*******************************************


/*
void  SIGINT_handler(int sig)
{
	printf("Exit from SIGINT\n");
	exit(0);
}
*/

//************* SERVER CODE ***************
struct url_data {
    size_t size;
    char* data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1);

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

char *handle_url(char* url) {
    CURL *curl;

    struct url_data data;
    data.size = 0;
    data.data = malloc(4096); /* reasonable size initial buffer */
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",  
                        curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}
//*******************************************



//************* MOTOR CODE ***************
void * run_motor(void *voidData) {
	digitalWrite(RELAY_1, RELAY_ENABLE);
	digitalWrite(RELAY_2, RELAY_DISABLE);
	sleep(10);
	digitalWrite(RELAY_1, RELAY_DISABLE);
	digitalWrite(RELAY_1, RELAY_DISABLE);

	return NULL;
}
//*******************************************





//************* IR CODE ***************
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

void * run_infrared(void *voidData) {
	char key;
	while(1) {
		key = getKey();
		//printf("Raw %x\n", key);

		if(key != ERROR) {
			printf("%x\n", key);
		}
		if(key == 0x44) {
			digitalWrite(RELAY_2, RELAY_DISABLE);
			digitalWrite(RELAY_1, RELAY_ENABLE);
		}
		if(key == 0x40) {
			digitalWrite(RELAY_1, RELAY_DISABLE);
			digitalWrite(RELAY_2, RELAY_ENABLE);
		}
		if(key == 0x43) {
			digitalWrite(RELAY_1, RELAY_DISABLE);
			digitalWrite(RELAY_2, RELAY_DISABLE);
		}
		if(key == 0x47) {
			if ((fork_song_pid!=0)) {
				//printf("Stopping alarm\n");
				if (kill(fork_song_pid,0) == 0) {
					kill(fork_song_pid, SIGINT);
					fork_song_pid = 0;
				} else {
					fprintf(stderr, "pid do not exists");
				}
			}
		}
	}
	return NULL;
}
//*************************************



int main(int argc, char* argv[]) {
	// SERVER
	//pid_t fork_song_pid = 0;
	char* data = NULL;
    	int play_song_secs;
    	int move_motor_secs;
    	char song_to_play_path[200];
	// MOTOR
	pthread_t thread_motor;
	// INFRARED
	pthread_t thread_infrared;
	//char key;
	
	

	wiringPiSetup();
	// MOTOR
	pinMode(RELAY_1, OUTPUT);
	pinMode(RELAY_2, OUTPUT);
	digitalWrite(RELAY_1, RELAY_DISABLE);
	digitalWrite(RELAY_2, RELAY_DISABLE);
	// INFRARED
	pinMode(PIN_IR, INPUT);
	pinMode(PIN_IR, PUD_UP);


	if (pthread_create(&thread_infrared, NULL, run_infrared, NULL)) {
		fprintf(stderr, "An error occured while creating new thread");
		return 1;
	}

	/*
	if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
          printf("SIGINT install error\n");
          exit(1);
     	}*/

	while(1) {
		
		data = handle_url(GET_ALARM_INFO_URL);
	
		if (!data[0]) {
			fprintf(stderr, "ERROR - no data received\n");
			sleep(SECONDS_NEXT_REQUEST);
			continue;
		}

		sscanf(data, "%d,%d,%s", &move_motor_secs, &play_song_secs, song_to_play_path);
		free(data);
		
		//printf("%d,", play_song_secs);
		//printf("%d,", move_motor_secs);
		//printf("%s\n", song_to_play_path);
		
		if (move_motor_secs>=0) {
			if (pthread_create(&thread_motor, NULL, run_motor, NULL)) {
				fprintf(stderr, "An error occured while creating new thread");
				return 1;
			}
		}

        if (play_song_secs >= 0) {
			fork_song_pid = fork();
			if (fork_song_pid == 0) {
				sleep(play_song_secs);
				execl(MUSIC_PLAYER_PATH, MUSIC_PLAYER, song_to_play_path, NULL);
				return 0;
			}
		} else {
			if ((play_song_secs==-2) && (fork_song_pid!=0)) {
				//printf("Stopping alarm\n");
				if (kill(fork_song_pid,0) == 0) {
					kill(fork_song_pid, SIGINT);
					fork_song_pid = 0;
				} else {
					fprintf(stderr, "pid do not exists");
				}
			}
        }

		sleep(SECONDS_NEXT_REQUEST);
	}

		//pthread_join(thread_motor, NULL);
		return 0;
}
