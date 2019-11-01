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


#define SECONDS_NEXT_REQUEST 2
//static char GET_ALARM_INFO_URL[] = "http://raspberrypi.local:3000/get_alarm";
static char GET_ALARM_INFO_URL[] = "http://localhost:3000/get_alarm";

static const char MUSIC_PLAYER[] = "mplayer";
#ifdef __APPLE__
static const char MUSIC_PLAYER_PATH[] = "/usr/local/bin/mplayer";
#else
static const char MUSIC_PLAYER_PATH[] = "/usr/bin/mplayer";
#endif


#define RELAY_1 2
#define RELAY_2 3
#define RELAY_ENABLE  HIGH
#define RELAY_DISABLE LOW

/*
void  SIGINT_handler(int sig)
{
	printf("Exit from SIGINT\n");
	exit(0);
}
*/

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

void * run_motor(void *voidData) {
	digitalWrite(RELAY_1, RELAY_ENABLE);
	digitalWrite(RELAY_2, RELAY_DISABLE);
	sleep(10);
	digitalWrite(RELAY_1, RELAY_DISABLE);
	digitalWrite(RELAY_1, RELAY_DISABLE);

	return NULL;
}

int main(int argc, char* argv[]) {
	pid_t fork_song_pid = 0;
	pthread_t thread_motor;
	char* data = NULL;
    	int play_song_secs;
    	int move_motor_secs;
    	char song_to_play_path[200];
	
	wiringPiSetup();
	pinMode(RELAY_1, OUTPUT);
	pinMode(RELAY_2, OUTPUT);
	digitalWrite(RELAY_1, RELAY_DISABLE);
	digitalWrite(RELAY_2, RELAY_DISABLE);
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
