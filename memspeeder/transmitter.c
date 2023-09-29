// Transmitter for naive covert channel communication demo using memory bus loading
//
// build with:   gcc -O0 -o tx transmitter.c
//
// implements "8n1" asynchronous serial data
// 0 = not so much load on memory bus = high bus speed
// 1 = high load on memory bus = low bus speed
// When transmitting a bit, the load is increased (or not!) for a full second.
// The receiver samples the memory speed is measured every half a second
//
// The first byte is 0xAB (10101011). It serves as a preable and start-of
// transmission. This helps to really catch the start.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MB 128

void tx_byte(char character, int *a, int *b) {
	struct timespec ts_start, ts_now;

	// Wain until seconds change.
	// This adds a brief pause between characters.
	timespec_get(&ts_start, TIME_UTC);
	while(1) {
		timespec_get(&ts_now, TIME_UTC);
		if(ts_now.tv_sec != ts_start.tv_sec) break;
	}

	// Send start bit. lähetä starttibitti
	// Repeat memcpy until seconds change
	printf("bit: start\n");
	timespec_get(&ts_start, TIME_UTC);
	while(1) {
		timespec_get(&ts_now, TIME_UTC);
		if(ts_now.tv_sec != ts_start.tv_sec) break;
		memcpy(a, b, MB*1024*1024);
		memcpy(b, a, MB*1024*1024);
	}

	for(int k=7; k>=0; k--) {
		// Send bits
		printf("bit: %d\n", (character >> k) & 1);

		if((character >> k) & 1) {
			// 1
			// Repeat memcpy until seconds change
			timespec_get(&ts_start, TIME_UTC);
			while(1) {
				timespec_get(&ts_now, TIME_UTC);
				if(ts_now.tv_sec != ts_start.tv_sec) break;
				memcpy(a, b, MB*1024*1024);
				memcpy(b, a, MB*1024*1024);
			}
		} else {
			// 0
			// Just wait until seconds change
			timespec_get(&ts_start, TIME_UTC);
			while(1) {
				timespec_get(&ts_now, TIME_UTC);
				if(ts_now.tv_sec != ts_start.tv_sec) break;
			}
		}
	}

	// transmit "stop" bit
	printf("bit: stop\n");
	timespec_get(&ts_start, TIME_UTC);
	while(1) {
		timespec_get(&ts_now, TIME_UTC);
		if(ts_now.tv_sec != ts_start.tv_sec) break;
	}
}


int main(int argc, char* argv[]) {
	int *a, *b;

	if (argc < 2) {
		printf("Usage: %s <string>\n", argv[0]);
		exit(1);
	}

	a = malloc(MB*1024*1024);
	if(NULL==a) {
		perror("Error allocating memory block a");
		exit(1);
	}

	b = malloc(MB*1024*1024);
	if(NULL==a) {
		perror("Error allocating memory block b");
		exit(1);
	}

	for(int t=0; t<MB/sizeof(int); t++) a[t]=0xaa;
	for(int t=0; t<MB/sizeof(int); t++) b[t]=0xaa;


	// send 0b10101011 as "start of transmission"
	tx_byte(0xab, a, b);

	for (int i=1; i<argc; i++) {
		// loop through words
		char* word = argv[i];
		int len = strlen(word);
		for(int j=0; j<len; j++) {
			// And letters
			tx_byte(word[j], a, b);
		}

		// send space between words, but not at the end.
		if(i<argc-1) tx_byte(0x20, a, b);
	}

	// Free memory
	free(a);
	free(b);

	return 0;
}
