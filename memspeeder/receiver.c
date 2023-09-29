// Receiver for naive covert channel communication demo using memory bus loading
//
// build with:   gcc -O0 -o rx receiver.c
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

#define THRESHOLD 0.7
#define MB 128

#define STATE_WAIT_FOR_FIRST_SOT_BIT 1
#define STATE_WAIT_FOR_SECOND_SOT_BIT 2
#define STATE_WAIT_FOR_START 3
#define STATE_RX_CHAR 4


void wait_for_next_half_second() {
	struct timespec start, now, next_second;
	clock_gettime(CLOCK_REALTIME, &start);

	while(1) {
		clock_gettime(CLOCK_REALTIME, &now);

		if(start.tv_nsec < 500000000) {
			if((now.tv_nsec > 500000000)) break;
		} else {
			if((now.tv_sec != start.tv_sec) & (now.tv_nsec > 500000000)) break;
		}
	}
}

// Will return memcpy() data rate in MB/s
int measureMemcpy(int *a, int *b) {
	// Record start time
	clock_t start = clock();

	// Access memory
	memcpy(a, b, MB*1024*1024);

	// Record end time
	clock_t end = clock();

	// Calculate elapsed time
	double time = (end - start) / (double) CLOCKS_PER_SEC;

	// Calculate data rate
	int rate = (int) (MB/time);

	return rate;
}





int main() {
	struct timespec ts;
	int rate;
	int dada[16];
	long sum;
	char c;
	int currentBit;
	int state = STATE_WAIT_FOR_FIRST_SOT_BIT;
	char str[50];
	int idlecounter=0;
	int valueHigh=0;
	int *a, *b;

	a = malloc(MB*1024*1024);
	if(NULL==a) {
		perror("Error allocating memory a");
		exit(1);
	}
	b = malloc(MB*1024*1024);
	if(NULL==b) {
		perror("Error allocating memory b");
		exit(1);
	}

	for(int t=0; t<MB*1024*1024/sizeof(int); t++) a[t]=0xaa;
	for(int t=0; t<MB*1024*1024/sizeof(int); t++) b[t]=0xaa;


    // Print results
//    printf("Memory access bandwidth: %f MB/s\n", 256*MB / (time * MB));
//    printf("Continuing with testData size %i MB\n", testSize);
	setbuf(stdout, NULL);

	while(1) {
		wait_for_next_half_second();

		// Make 16 memcpy measuerements and get the average.
		// This quite effectively hides brief moments of activity.
		for(int i=0; i<16; i++) dada[i] = measureMemcpy(a, b);
		sum = 0;
		for(int i=0; i<16; i++) sum = sum + dada[i];
		rate = sum/16;
		printf("%i MB/s  ", rate);

		// this keeps track what's the observed maximum speed of the bus.
		// i.e. how high high value is.
		if(rate > valueHigh) valueHigh=rate;


		// This thing will timeout the RX if there is no 1 in 10 cycles after
		// some data has been received.
		if(state == STATE_WAIT_FOR_START) {
			idlecounter++;
			if((idlecounter > 4)) {
				printf("\ntimeout.\nk thx bye.\n");
				exit(1);
			}
		} else {
			idlecounter=0;
		}



		// Ugly state machine.. :(
		// sorry about this.. :(

		if(state == STATE_WAIT_FOR_FIRST_SOT_BIT) {
			if(rate < valueHigh*THRESHOLD) {
				// 1
				state = STATE_WAIT_FOR_SECOND_SOT_BIT;
				printf("is sot?  ");
			} else {
				// 0
				printf("         ");
			}
		}


		else if(state == STATE_WAIT_FOR_SECOND_SOT_BIT) {
			if(rate < valueHigh*THRESHOLD) {
				// 1
				state = STATE_WAIT_FOR_START;
				printf("got sot  ");
			} else {
				// 0
				state = STATE_WAIT_FOR_FIRST_SOT_BIT;
				printf("no sot   ");
			}
		}


		else if(state == STATE_WAIT_FOR_START) {
			if(rate < valueHigh*THRESHOLD) {
				printf("got start");

				state = STATE_RX_CHAR;
				currentBit = 8;
				c = 0;
			}
			else printf("         ");
		}


		else if(state == STATE_RX_CHAR) {
			currentBit--;

			if(currentBit <= 7) {
				if(rate < valueHigh*THRESHOLD) {
					// bit 1
					c |= (1<<currentBit); // set bit
					printf("rx bit 1 ");
				} else {
					// bit 0
					printf("rx bit 0 ");
				}
			}

			if(currentBit == 0) {
				// We rx'd all 8 bits for this character.
				// Check if the str has enough space to add a character
				if (strlen(str) + 1 < sizeof(str)) {
					strncat(str, &c, 1);
				} else {
					printf("\nMessage too long. Exiting.\n");
					exit(1);
				}

				state = STATE_WAIT_FOR_START;
			}

		}
		printf("  message: %s\n", str);

	}

	// Free memory
	free(a);
	free(b);

	return 0;
}
