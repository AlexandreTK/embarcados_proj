#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef __APPLE__
static const char MUSIC_PLAYER_PATH[] = "/usr/local/bin/mplayer";
#else
static const char MUSIC_PLAYER_PATH[] = "/usr/bin/mplayer";
#endif
static const char MUSIC_PLAYER[] = "mplayer";

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
    tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

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

void run_play_song(int play_song_secs, char * song_to_play_path) {
	sleep(play_song_secs);

	//system(song_to_play_cmd);
	execl(MUSIC_PLAYER_PATH, MUSIC_PLAYER, song_to_play_path, NULL);
}

int main(int argc, char* argv[]) {
	pid_t fork_pid = 0;
	char* data = NULL;

	
	while(1) {
		
		data = handle_url("http://raspberrypi.local:3000/get_alarm");
	
		int play_song_secs;
		int move_motor_secs;
		char song_to_play_path[200];
		if (!data[0]) {
			fprintf(stderr, "ERROR - no data received\n");
			sleep(10);
			continue;
		}
		sscanf(data, "%d,%d,%s", &move_motor_secs, &play_song_secs, song_to_play_path);
		free(data);
		
		printf("%d\n", play_song_secs);
		printf("%d\n", move_motor_secs);
		printf("%s\n", song_to_play_path);
		
		if (move_motor_secs==-1) {
			// Do nothing
		} else {
			//alarm(move_motor_secs);
		}
        if (play_song_secs==-1) {
            // Do nothing
        } else {
			fork_pid = fork();
			if (fork_pid == 0) {
				run_play_song(play_song_secs, song_to_play_path);
				return 0;
			} else {
				//sleep(10);
				//kill(fork_pid, SIGKILL);
			}
			
        }

		sleep(10);
	}
		return 0;
}
