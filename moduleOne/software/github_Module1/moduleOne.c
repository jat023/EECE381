/*
 * Module 1 - EECE381
 * Tuesday/Thursday 8 AM
 * Group 12
 *
 * Kyle Garsuta
 * Adam Woods
 * James Tu
 * Christopher Yoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include <unistd.h>
#include "stdio.h"
#include "system.h"
#include "sys/alt_irq.h"

#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_ps2.h"
#include <altera_up_ps2_keyboard.h>
#include <altera_up_ps2_mouse.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>

#define drawer_base (volatile int *) 0x4020
#define MY_HW_ONLY_TIMER_BASE (volatile int*) 0x4040
#define LEFT_ARROW 0
#define UP_ARROW 1
#define DOWN_ARROW 2
#define RIGHT_ARROW 3
#define NO_ARROW 4
#define WIDTH 40
#define MAXSIZE 60

//---------------------------------------------------
//---------------STRUCTS ----------------------------
//---------------------------------------------------
struct stack {
	int stk[MAXSIZE];
	int top;
};

typedef struct Arrows {
	int x;
	int y;
	int dir;
	int colour;
	int reset;
} Arrow;

typedef struct stack STACK;
STACK s;
Arrow arrow[5];

//---------------------------------------------------


//---------------------------------------------------
//-----------FUNCTION PROTOTYPES --------------------
//---------------------------------------------------
void drawArrow(Arrow arrowPointer, unsigned int buffer);
void playAudio();
void av_config_setup();
int createRandomArrows();
void push();
int pop();
int peek();
//char loadScreen();
char readKeyboard();
void setUpVideo();
//---------------------------------------------------


int main(void) {

	Arrow tempArrow;
	int i;
	int countasdf = 0;

	int timer_period, status, done;

	int freq, cycles;
	float duration;
	int ticks_start, ticks_end, ticks_duration, whichBuffer;


	// -------------------------------------------------------
	// CAN CREATE A DIFFERENT STACK FOR DIFFERENT SONGS; THEN USE AS APPROPRIATE TO WHAT WAS SELECTED BY THE USER
	// -------------------------------------------------------
	// Variables for the STACK
	int counter = MAXSIZE;
	s.top = -1;

	// DO LOOP fills the stack with arrows for the song
	do {
		int arrowToPush = createRandomArrows();
		push(arrowToPush);
		counter--;
	} while (counter < 60);
	// END DO LOOP that fills stack with arrows


	/*
	char opt;

	while(1) {
		opt = loadScreen();

		if (opt == '1') {
			alt_up_sd_card_dev *device_reference = alt_up_sd_card_open_dev("/dev/sd_card");
			setUpVideo();

			if (device_reference != NULL) {
				playAudio();
			}
		}
		else if (opt == '2') {
			changeSong();
		}
		else if (opt == '3') {

			// ---------------------------------
			// PRINT GOODBYE MESSAGE TO SCREEN
			// ---------------------------------
			return 0;
		}
	}
	*/

	alt_up_sd_card_dev *device_reference = alt_up_sd_card_open_dev("/dev/sd_card");
	setUpVideo();

	if (device_reference != NULL) {
		playAudio();
	}

	return 0;
}


/***********************************************************************
 * loadScreen() function will loop until user enters an option
 * Parameter: None
 * Return: char
 ***********************************************************************
char loadScreen() {

	char optionChosen;

	while (1) {
		// code to display load screen with 3 options:
			// 1. Start
			// 2. Change Song
			// 3. Exit

		optionChosen = readKeyboard();

		if (optionChosen == '1') {
			return optionChosen;
		}
		else if (optionChosen == '2') {
			return optionChosen;
		}
		else if (optionChosen == '3') {
			return optionChosen;
		}
		else {
			// do nothing; continue to loop until proper input is received
		}
	}
}*/

/***********************************************************************
 * Function to set up double pixel buffers
 * Parameter: None
 * Return: Void
 **********************************************************************/
void setUpVideo() {
	int ticks_per_s;

	alt_up_pixel_buffer_dma_dev* pixel_buffer;

	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");
	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);// Set the 1st buffer address

	//this might fix it if the program doesn't run.
	//alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	//while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);
	// Clear first buffer (this makes all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	//Swap the first buffer to front
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);
	// Clear second buffer (this makes all pixels black)
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	//Swap the second buffer to front
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	arrow[1].x = 20;
	arrow[1].y = 20;
	arrow[1].colour = 0xFFFF;
	arrow[1].dir = 0;
	arrow[2].x = 70;
	arrow[2].y = 20;
	arrow[2].colour = 0xFF0F;
	arrow[2].dir = 1;
	arrow[3].x = 120;
	arrow[3].y = 20+14;
	arrow[3].colour = 0xF0FF;
	arrow[3].dir = 2;
	arrow[4].x = 170;
	arrow[4].y = 20;
	arrow[4].colour = 0x0FFF;
	arrow[4].dir = 3;

	/*
	printf(" Hardware-Only Timer\n");
	printf(" Setting timer period to 5 seconds.\n");
	timer_period = 20 * 50000000;
	IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE, 8, timer_period & 0xFFFF);
	IOWR_16DIRECT(MY_HW_ONLY_TIMER_BASE, 12, timer_period >> 16);*/

	printf(" Sys Clock Timer\n");
	ticks_per_s = 50000000; //alt_ticks_per_second();
	printf(" Tick Freq: %d\n", ticks_per_s);
}

/**********************************************************************
 * Draw arrow function
 * Parameter: Arrow arrowPointer
 * Parameter: unsigned int buffer
 * Return: Void
 *********************************************************************/
void drawArrow(Arrow arrowPointer, unsigned int buffer) {
	IOWR_32DIRECT(drawer_base,20,buffer);  // Set buffer
    IOWR_32DIRECT(drawer_base,0,arrowPointer.x); // Set x1
    IOWR_32DIRECT(drawer_base,4,arrowPointer.y); // Set y1
    IOWR_32DIRECT(drawer_base,8,arrowPointer.dir); // Set y2
    IOWR_32DIRECT(drawer_base,12,arrowPointer.colour);  // Set colour
    IOWR_32DIRECT(drawer_base,16,1);  // Start drawing
    while(IORD_32DIRECT(drawer_base,16)==0); // wait until done
}

/***********************************************************************
 * Function for reading keyboard input from the user for the game.
 * Parameter: None
 * Return: char
 **********************************************************************/
char readKeyboard() {
	KB_CODE_TYPE code_type;
	unsigned char buf;
	char ascii;

	alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev("/dev/ps2_0");
	alt_up_ps2_init(ps2);
	decode_scancode(ps2, &code_type, &buf, &ascii);

	return ascii;
}

/***********************************************************************
 * Set up function for audio and video configuration
 * Parameter: None
 * Return: Void
 **********************************************************************/
void av_config_setup() {
	alt_up_av_config_dev * av_config;
	av_config = alt_up_av_config_open_dev("/dev/audio_and_video_config_0");

	while (!alt_up_av_config_read_ready(av_config)) {

	}
}

/***********************************************************************
 * playAudio function will play audio files from the SD card
 * contains arrow drawing code
 * Parameter: None
 * Return: Void
 **********************************************************************/
void playAudio() {

	char userInput;
	int connected = 0;
	int songEnded = 0;

	if ((connected == 0) && (alt_up_sd_card_is_Present())) {
		printf("Card connected.\n");

		if (alt_up_sd_card_is_FAT16()) {
			printf("FAT16 file system detected.\n");

			// Start opening audio file
			short int audio_handle = alt_up_sd_card_fopen("audio.wav", 0);

			if (audio_handle >= 0) {
				// Start audio play back
				unsigned int buf_left[1];
				av_config_setup();

				alt_up_audio_dev * audioPlayback;
				audioPlayback = alt_up_audio_open_dev("/dev/audio_0");

				if (audioPlayback == NULL) {
					fprintf(stderr, "Error opening audio device.\n");
				} else {
					printf("Audio device initiated.\n");
				}

				short int byte_l0 = alt_up_sd_card_read(audio_handle);
				short int byte_l1 = alt_up_sd_card_read(audio_handle);

				// Do LOOP: includes both playing audio and continuously reading KB input
				do {
					userInput = readKeyboard();
					int arrowNumber = pop();

					if (arrowNumber == UP_ARROW) {
						if ((byte_l0 >= 0) || (byte_l1 >= 0)) {
							while("arrow still has not reached top" && ((byte_l0 >= 0) || (byte_l1 >= 0))) {
								// code from Chris's computer to create UP arrow

								short left16 = ((unsigned char) byte_l1 << 8) | (unsigned char) byte_l0;
								buf_left[0] = (int) (left16 / 50);
								while (alt_up_audio_write_fifo_space(audioPlayback, ALT_UP_AUDIO_LEFT) < 1);

								// Send audio bits to LEFT and RIGHT buffers
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1, ALT_UP_AUDIO_LEFT);
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1,	ALT_UP_AUDIO_RIGHT);

								// Read next bytes of data
								byte_l0 = alt_up_sd_card_read(audio_handle);
								byte_l1 = alt_up_sd_card_read(audio_handle);
							}
						} else {
							songEnded = 1;
						}
					} else if (arrowNumber == DOWN_ARROW) {
						if ((byte_l0 >= 0) || (byte_l1 >= 0)) {
							while("arrow still has not reached top" && ((byte_l0 >= 0) || (byte_l1 >= 0))) {
								// code from Chris's computer to create UP arrow

								short left16 = ((unsigned char) byte_l1 << 8) | (unsigned char) byte_l0;
								buf_left[0] = (int) (left16 / 50);
								while (alt_up_audio_write_fifo_space(audioPlayback, ALT_UP_AUDIO_LEFT) < 1);

								// Send audio bits to LEFT and RIGHT buffers
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1, ALT_UP_AUDIO_LEFT);
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1,	ALT_UP_AUDIO_RIGHT);

								// Read next bytes of data
								byte_l0 = alt_up_sd_card_read(audio_handle);
								byte_l1 = alt_up_sd_card_read(audio_handle);
							}
						} else {
							songEnded = 1;
						}
					} else if (arrowNumber == LEFT_ARROW) {
						if ((byte_l0 >= 0) || (byte_l1 >= 0)) {
							while("arrow still has not reached top" && ((byte_l0 >= 0) || (byte_l1 >= 0))) {
								// code from Chris's computer to create UP arrow

								short left16 = ((unsigned char) byte_l1 << 8) | (unsigned char) byte_l0;
								buf_left[0] = (int) (left16 / 50);
								while (alt_up_audio_write_fifo_space(audioPlayback, ALT_UP_AUDIO_LEFT) < 1);

								// Send audio bits to LEFT and RIGHT buffers
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1, ALT_UP_AUDIO_LEFT);
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1,	ALT_UP_AUDIO_RIGHT);

								// Read next bytes of data
								byte_l0 = alt_up_sd_card_read(audio_handle);
								byte_l1 = alt_up_sd_card_read(audio_handle);
							}
						} else {
							songEnded = 1;
						}
					} else if (arrowNumber == RIGHT_ARROW) {
						if ((byte_l0 >= 0) || (byte_l1 >= 0)) {
							while("arrow still has not reached top" && ((byte_l0 >= 0) || (byte_l1 >= 0))) {
								// code from Chris's computer to create UP arrow

								short left16 = ((unsigned char) byte_l1 << 8) | (unsigned char) byte_l0;
								buf_left[0] = (int) (left16 / 50);
								while (alt_up_audio_write_fifo_space(audioPlayback, ALT_UP_AUDIO_LEFT) < 1);

								// Send audio bits to LEFT and RIGHT buffers
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1, ALT_UP_AUDIO_LEFT);
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1,	ALT_UP_AUDIO_RIGHT);

								// Read next bytes of data
								byte_l0 = alt_up_sd_card_read(audio_handle);
								byte_l1 = alt_up_sd_card_read(audio_handle);
							}
						} else {
							songEnded = 1;
						}
					} else {
						//do not draw arrow
						//or draw an arrow according to background colour to mimic "no" arrow
						if ((byte_l0 >= 0) || (byte_l1 >= 0)) {
							while("arrow still has not reached top" && ((byte_l0 >= 0) || (byte_l1 >= 0))) {
								// code from Chris's computer to create UP arrow

								short left16 = ((unsigned char) byte_l1 << 8) | (unsigned char) byte_l0;
								buf_left[0] = (int) (left16 / 50);
								while (alt_up_audio_write_fifo_space(audioPlayback, ALT_UP_AUDIO_LEFT) < 1);

								// Send audio bits to LEFT and RIGHT buffers
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1, ALT_UP_AUDIO_LEFT);
								alt_up_audio_write_fifo(audioPlayback, buf_left, 1,	ALT_UP_AUDIO_RIGHT);

								// Read next bytes of data
								byte_l0 = alt_up_sd_card_read(audio_handle);
								byte_l1 = alt_up_sd_card_read(audio_handle);
							}
						} else {
							songEnded = 1;
						}
					}

					// Continue audio play while song is still playing
				} while (songEnded == 0);

			} else {
				fprintf(stderr, "Error opening audio file.\n");
				//break;
			}
		} else {
			printf("Unknown file system.\n");
		}
		connected = 1;
	} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
		printf("Card disconnected.\n");
		connected = 0;
	}
}


/***********************************************************************
 * Push function for C implemented stack
 * Parameter: None
 * Return: Void
 **********************************************************************/
void push() {
	int num;

	if (s.top == (MAXSIZE - 1)) {
		printf("Stack full\n");
		return;
	}
	else {
		s.top = s.top + 1;
		s.stk[s.top] = num;
	}

	return;
}

/***********************************************************************
 * Pop function for C implemented stack
 * Parameter: None
 * Return: int num
 **********************************************************************/
int pop() {
	int num;

	if (s.top == -1) {
		printf("Stack is empty\n");
		return (s.top);
	}
	else {
		num = s.stk[s.top];
		s.top = s.top - 1;
	}

	return(num);
}

/***********************************************************************
 * Peek function for C implemented stack
 * Parameter: None
 * Return: Void
 **********************************************************************/
int peek() {
	return s.stk[s.top];
}


/***********************************************************************
 * Function creates arrows randomly based on srand() function
 * Parameter: None
 * Return: Void
 **********************************************************************/
int createRandomArrows() {
	// Variables for random arrow generator
	int randomArrow;
	//int randomDouble;
	//int randomSecond;
	srand(time(NULL)); // Initialise the seed for the random number generator

	randomArrow = rand() % 5; // randomArrow is in the range of 0 to 4, inclusive
	//randomDouble = rand() % 2; // randomDouble determines if two arrows will be presented
	//randomSecond = rand() % 3; // randomSecond determines the second arrow if double arrows are true

	if (randomArrow == LEFT_ARROW) {
		/*if (randomDouble == 1) {
			if (randomSecond == 0) {

			} else if (randomSecond == 1) {

			} else {

			}
		} else {*/
			return LEFT_ARROW;
		//}
	} else if (randomArrow == UP_ARROW) {
/*		if (randomDouble == 1) {
			if (randomSecond == 0) {

			} else if (randomSecond == 1) {

			} else {

			}
		} else {*/
			return UP_ARROW;
		//}
	} else if (randomArrow == DOWN_ARROW) {
/*		if (randomDouble == 1) {
			if (randomSecond == 0) {

			} else if (randomSecond == 1) {

			} else {

			}
		} else {*/
			return DOWN_ARROW;
		//}
	} else if (randomArrow == NO_ARROW) {

		return NO_ARROW;

	} else {
/*		if (randomDouble == 1) {
			if (randomSecond == 0) {

			} else if (randomSecond == 1) {

			} else {

			}
		} else {*/
			return RIGHT_ARROW;
	//	}
	}
}
