#include <stdio.h>
#include "io.h"
#include <stdlib.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>
#include <time.h>
#include <stdlib.h>
#include "sys/alt_timestamp.h"
#include "sys/alt_alarm.h"
#include <unistd.h>
#include "system.h"

#include "sys/alt_irq.h" // Interrupts
// Audio related includes
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

//Keyboard Stuff
#include "altera_up_avalon_ps2.h"
#include <altera_up_ps2_keyboard.h>

#define drawer_base (volatile int *) 0x48c0
//#define drawer_base (volatile int *) 0x4020
#define MY_HW_ONLY_TIMER_BASE (volatile int*) 0x48e0
#define MAIN_LOOP_TIMER_BASE (volatile int*) 0x48a0
#define LEFT_ARROW 0
#define UP_ARROW 1
#define DOWN_ARROW 2
#define RIGHT_ARROW 3
#define WIDTH 40
#define MAXARROWS 60

// Audio constants
#define BUF_LEN 96
#define VOLUME_DIVIDER 50

typedef struct Arrows {
	int x;
	int y;
	int dir;
	int colour;
	int scored;
	int bufferState;
} Arrow;

typedef struct Node {
	Arrow arrow;
	struct Node *next;
} ArrowNode;

//void drawArrow(Arrow arrowPointer, unsigned int buffer);

struct video_context {
	alt_up_pixel_buffer_dma_dev * pixel_buffer;
	ArrowNode *root;
};

// Audio context for interrupts
struct audio_context {
	short int audio_handle;
	alt_up_audio_dev * audioPlayback;
	unsigned int buffer[BUF_LEN];
};

// Audio interrupt handler
static void fill_AudioBuffer(void * context, alt_u32 id) {
	struct audio_context *ctxt = (struct audio_context*) context;

	alt_up_audio_write_fifo(ctxt->audioPlayback, ctxt->buffer, BUF_LEN,
			ALT_UP_AUDIO_LEFT);
	alt_up_audio_write_fifo(ctxt->audioPlayback, ctxt->buffer, BUF_LEN,
			ALT_UP_AUDIO_RIGHT);

	IOWR_16DIRECT(AUDIO_0_BASE, 0, 0);
	alt_up_audio_enable_write_interrupt(ctxt->audioPlayback);

	int i;
	for (i = 0; i < BUF_LEN; i++) {
		short int byte_l0 = alt_up_sd_card_read(ctxt->audio_handle);
		short int byte_l1 = alt_up_sd_card_read(ctxt->audio_handle);

		short sample16 = ((unsigned char) byte_l1 << 8)
				| (unsigned char) byte_l0;

		ctxt->buffer[i] = (int) (sample16 / VOLUME_DIVIDER);
	}

	alt_irq_enable(TIMER_HARDWARE_0_IRQ);

	return;
}

void draw_Arrows(void * context) {

	struct video_context *ctxt = (struct video_context*) context;

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);

	Arrow tempArrow;
	ArrowNode *nextNode;
	int speed = 1;

	if (ctxt->root->arrow.bufferState == 0) { //change later

		//Erase arrows in buffer 1
		nextNode = ctxt->root;

		while (nextNode != NULL) {
			tempArrow = nextNode->arrow;

			tempArrow.y = tempArrow.y - speed;

			tempArrow.colour = 0x0000;
			drawArrow(tempArrow, pixel_buffer_addr1);

			nextNode = nextNode->next;
		}

		//alt_up_pixel_buffer_dma_draw_rectangle(drawer_base,0,195,360,210,0x00F0,0);

		//draw each arrow in buffer 1
		nextNode = ctxt->root;

		while (nextNode != NULL) {
			if (nextNode->arrow.y >= 213) {
				nextNode->arrow.colour = 0x0000;
			}
			nextNode->arrow.y = nextNode->arrow.y + speed;
			drawArrow(nextNode->arrow, pixel_buffer_addr1);

			nextNode->arrow.bufferState = 1;

			nextNode = nextNode->next;
		}

		//swap buffers, show buffer 1 on screen. While swap takes about 1/60th of a second.
		alt_up_pixel_buffer_dma_swap_buffers(ctxt->pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(
				ctxt->pixel_buffer))
			;
	}

	//usleep(0030000);

	else {
		//Erase Arrows in buffer 2
		nextNode = ctxt->root;

		while (nextNode != NULL) {
			tempArrow = nextNode->arrow;

			tempArrow.y = tempArrow.y - speed;

			tempArrow.colour = 0x0000;
			drawArrow(tempArrow, pixel_buffer_addr2);

			nextNode = nextNode->next;
		}

		//alt_up_pixel_buffer_dma_draw_rectangle(ctxt->pixel_buffer,0,195,360,210,0x00F0,0);

		//draw each arrow in buffer 2
		nextNode = ctxt->root;

		while (nextNode != NULL) {
			if (nextNode->arrow.y >= 213) {
				nextNode->arrow.colour = 0x0000;
			}
			nextNode->arrow.y = nextNode->arrow.y + speed;
			drawArrow(nextNode->arrow, pixel_buffer_addr2);

			//Change bufferState of Arrow (change later)
			nextNode->arrow.bufferState = 0;

			nextNode = nextNode->next;
		}

		//swap buffers, show buffer 2 on screen
		alt_up_pixel_buffer_dma_swap_buffers(ctxt->pixel_buffer);
		while (alt_up_pixel_buffer_dma_check_swap_buffers_status(
				ctxt->pixel_buffer))
			;
	}

	//usleep(0030000);

	return;
}

int main() {

	ArrowNode *arrowList[40];

	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/sd_card");

	printf("starting \n");
	Arrow arrow[5];
	int i;
	int numNodes;

	//Timer Stuff
	int timer_period;
	int status;
	int done;

	//Keyboard stuff
	KB_CODE_TYPE code_type;
	unsigned char buf;
	char ascii;
	int success;
	char asciiTemp;

	//Main loop stuff
	int loopCount = 0;
	int arrowCount = 0;

	alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev("/dev/ps2_0");
	alt_up_ps2_init(ps2);

	asciiTemp = ascii;

	printf("aaaa");

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	// Use the name of your pixel buffer DMA core
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev("/dev/pixel_buffer_dma");

	unsigned int pixel_buffer_addr1 = PIXEL_BUFFER_BASE;
	unsigned int pixel_buffer_addr2 = PIXEL_BUFFER_BASE + (512 * 240 * 2);

	//this might fix it if the program doesn't run.
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	printf("bbbb");

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr1);

	// Clear first buffer (this makes all pixels black)
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	//Swap the first buffer to front
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;

	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			pixel_buffer_addr2);
	// Clear second buffer (this makes all pixels black)
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

	//Swap the second buffer to front
	//alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	//while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

	printf("cccc");

	arrow[0].x = 30;
	arrow[0].y = 0;
	arrow[0].colour = 0xFFFF;
	arrow[0].dir = LEFT_ARROW;
	arrow[0].scored=0;

	arrow[1].x = 60;
	arrow[1].y = 0;
	arrow[1].colour = 0xFFFF;
	arrow[1].dir = UP_ARROW;
	arrow[1].scored=0;

	arrow[2].x = 100;
	arrow[2].y = 0;
	arrow[2].colour = 0xFFFF;
	arrow[2].dir = DOWN_ARROW;
	arrow[2].scored=0;

	arrow[3].x = 130;
	arrow[3].y = 0;
	arrow[3].colour = 0xFFFF;
	arrow[3].dir = RIGHT_ARROW;
	arrow[3].scored=0;

	ArrowNode *rootNode;
	ArrowNode *currentNode;
	ArrowNode *nextNode;
	ArrowNode *finalNode;
	ArrowNode *tempNode;

	rootNode = malloc(sizeof(ArrowNode));
	rootNode->arrow = arrow[1];
	currentNode = rootNode;

	/*
	 for (i = 0; i < 4; i++) {
	 arrow[num_arrow] = arrow[i];
	 nextNode = malloc(sizeof(ArrowNode));
	 nextNode->arrow = arrow[num_arrow];
	 currentNode->next = nextNode;
	 currentNode = nextNode;
	 }*/

	int counter = 0;

	printf("initializing arrowList");

	time_t t;
	/* Intializes random number generator */
	srand((unsigned) time(&t));

	do {
		int randomNumber = rand() % 4;

		nextNode = malloc(sizeof(ArrowNode));
		if(nextNode == NULL){
			printf("Error");
		}
		nextNode->arrow = arrow[randomNumber];
		currentNode->next = nextNode;
		arrowList[counter] = currentNode;
		currentNode = nextNode;

		counter++;
	} while (counter < 40);

	printf("initialization successful");

	//should fix the bug where 2 arrows are drawn
	rootNode->next = NULL;

	finalNode = nextNode;
	finalNode->next = NULL;

	numNodes = 0;
	nextNode = rootNode;

	/*printf("initialization successful");
	 while (nextNode != finalNode) {
	 //printf("NodeX: %d \n", nextNode->arrow.x);
	 nextNode = nextNode->next;
	 numNodes++;
	 }
	 printf("numNodes: %d \n", numNodes);*/

	struct video_context * context = malloc(sizeof(struct video_context));
	if(context == NULL){
		printf("Error");
	}
	context->root = rootNode;
	context->pixel_buffer = pixel_buffer;

	//if (device_reference != NULL) {
	//while (1) {
	if ((connected == 0) && (alt_up_sd_card_is_Present())) {

		printf("Card connected.\n");

		if (alt_up_sd_card_is_FAT16()) {
			printf("FAT16 file system detected.\n");

			// Start audio setup
			// Start opening audio file
			short int audio_handle = alt_up_sd_card_fopen("audio.wav", 0);

			if (audio_handle >= 0) {

				// Set up the audio/video IP core made in SOPC
				alt_up_av_config_dev * av_config;
				av_config = alt_up_av_config_open_dev(
						"/dev/audio_and_video_config_0");

				while (!alt_up_av_config_read_ready(av_config))
					;

				alt_up_audio_dev * audioPlayback;
				audioPlayback = alt_up_audio_open_dev(AUDIO_0_NAME);
				if (audioPlayback == NULL) {
					fprintf(stderr, "Error opening audio device.\n");
				} else {
					printf("Audio device initiated.\n");
				}

				// Enable interrupts
				struct audio_context *context = malloc(
						sizeof(struct audio_context));
				context->audio_handle = audio_handle;
				context->audioPlayback = audioPlayback;

				alt_irq_register(AUDIO_0_IRQ, (void*) context,
						(void*) fill_AudioBuffer);
				alt_up_audio_enable_write_interrupt(audioPlayback);
				// Finish audio setup

				free(context);
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

	printf("loop starting");
	int score=0;

	while (1) {
		decode_scancode(ps2, &code_type, &buf, &ascii);
		if (asciiTemp != ascii) {
			break;
		}
		asciiTemp = ascii;
	}


	//if (device_reference != NULL) {
	while (1) {

		//Add arrow to linked list
		if (loopCount == 40 && arrowCount < 40) {
			/*finalNode = malloc(sizeof(ArrowNode));
			 finalNode->next = arrowList[arrowCount];
			 currentNode->next = nextNode;
			 arrowList[counter] = currentNode;
			 currentNode = nextNode;*/

			//finalNode->next = malloc(sizeof(ArrowNode));
			finalNode->next = arrowList[arrowCount];
			finalNode = finalNode->next;
			finalNode->next = NULL;
			loopCount = 0;
			arrowCount++;
		}

		// Draw Arrows
		if (rootNode->arrow.y >= 218) {
			tempNode = rootNode;
			rootNode = rootNode->next;
			context->root = rootNode;
			//free(tempNode->next);
			//free(&tempNode);
			//Re-add //free(tempNode);
			//tempNode = NULL;

		}

		draw_Arrows(context);

		//Read Keyboard
		decode_scancode(ps2, &code_type, &buf, &ascii);
		//if (asciiTemp != ascii) {
		//printf("%c \n", ascii);
			if (ascii == 'A') {
				if(rootNode->arrow.y > 173 && rootNode->arrow.y < 183 && rootNode->arrow.dir == LEFT_ARROW){
					rootNode->arrow.colour = 0x0F00;
					if( rootNode->arrow.scored == 0) {
						score++;
						rootNode->arrow.scored = 1;
					}
				}
			} else if (ascii == 'W') {
				if(rootNode->arrow.y > 180 && rootNode->arrow.y < 190 && rootNode->arrow.dir == UP_ARROW){
					rootNode->arrow.colour = 0x0F00;
					if( rootNode->arrow.scored == 0) {
						score++;
						rootNode->arrow.scored = 1;
					}
				}
			} else if (ascii == 'S') {
				if(rootNode->arrow.y > 180 && rootNode->arrow.y < 190 && rootNode->arrow.dir == DOWN_ARROW){
					rootNode->arrow.colour = 0x0F00;
					if( rootNode->arrow.scored == 0) {
						score++;
						rootNode->arrow.scored = 1;
					}
				}
			} else if (ascii == 'D') {
				if(rootNode->arrow.y > 173 && rootNode->arrow.y < 183 && rootNode->arrow.dir == RIGHT_ARROW){
					rootNode->arrow.colour = 0x0F00;
					if( rootNode->arrow.scored == 0) {
						score++;
						rootNode->arrow.scored = 1;
					}
				}
			}
			if( rootNode->arrow.y > 190 && rootNode->arrow.colour != 0x0F00 ) {
				rootNode->arrow.colour = 0xF000;
			}
			//free(buf);
		//}


		//alt_up_pixel_dma_draw_box(drawer_base,0,195,360,200,0x00F0,0);
		alt_up_pixel_buffer_dma_draw_rectangle(pixel_buffer,0,188,319,203,0x00F0,1);

		//free(context);
		// Finish video interrupts
		loopCount++;

		if( rootNode == NULL) {
			printf("Score: %d \n", score);
			break;
		}
	}

	return 0;
}

void drawArrow(Arrow arrowPointer, unsigned int buffer) {
	//printf("drawing arrow\n");
	IOWR_32DIRECT(drawer_base, 20, buffer);
	// Set buffer
	IOWR_32DIRECT(drawer_base, 0, arrowPointer.x);
	// Set x1
	IOWR_32DIRECT(drawer_base, 4, arrowPointer.y);
	// Set y1
	IOWR_32DIRECT(drawer_base, 8, arrowPointer.dir);
	// Set y2
	IOWR_32DIRECT(drawer_base, 12, arrowPointer.colour);
	// Set colour
	IOWR_32DIRECT(drawer_base, 16, 1);
	// Start drawing
	while (IORD_32DIRECT(drawer_base,16) == 0)
		; // wait until done
}
