#include <stdio.h>
#include <time.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>

#include "system.h"
#include "io.h"

#define PI_REQUEST_SIG (volatile char *) 0x38a0
#define DE2_STATE (char *) 0x38b0

typedef enum {
	ready = 0,
	waiting_for_image_data = 1,
	reading_image_data = 2,
	waiting_for_path_data = 3,
	reading_path_data = 4,
	drawing_image = 5,
	drawing_path = 6
} FSM_States;

/**
 * Writes the FSM state to the corresponding GPIO pins
 *
 * Params
 * 	state: the FSM state to write
 *
 * Modifies
 * 	The corresponding GPIO pins are updated to reflect the given state
 */
void write_fsm_state(FSM_States state) {
	*DE2_STATE = (int) state;
}

/**
 * Reads the pi request signal
 *
 * Returns 1 if the signal is on, 0 otherwise
 */
int read_pi_request() {
	return (int)(*PI_REQUEST_SIG);
}

/*
 * Reads a given number of bytes from the dual port ram
 *
 * Params
 * 	count : the number of bytes to read
 * 	buffer : a pointer to a char buffer; the buffer size must >= count
 *
 * Modifies
 * 	The read values are stored in buffer
 */
void read_ram_data(int count, char * buffer) {

	int i;
	for (i = 0; i < count; i++) {
		*(buffer + i) = IORD_8DIRECT(DUAL_PORT_RAM_BASE, i);
	}
}

// TODO Implement
/**
 * Draws the image -- this needs to be implemented
 */
void draw_image() {
	printf("Drawing image\n"); // Dummy task

	// Below is a copy of the code provided by Chris

	alt_up_pixel_buffer_dma_dev* pixel_buffer;
	pixel_buffer = alt_up_pixel_buffer_dma_open_dev(PIXEL_BUFFER_DMA_NAME);
	if (pixel_buffer == 0) {
		printf(
				"error initializing pixel buffer (check name in alt_up_pixel_buffer_dma_open_dev)\n");
	}
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer,
			PIXEL_BUFFER_BASE);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer))
		;
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 0, 20);
	// Set x1
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 4, 20);
	// Set y1
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 8, 315);
	// Set x2
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 12, 200);
	// Set y2
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 16, 0x00F0);
	// Set colour
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 20, 1);
	// Start drawing
	while (IORD_32DIRECT(LINE_DRAWER_0_BASE,20) == 0)
		; // wait until done
}

// TODO Implement
/**
 * Draws the path -- this needs to be implemented
 */
void draw_path() {
	printf("Drawing path\n"); // Dummy task
}

// Global variables
FSM_States curr_state = ready;

int main() {

	printf("Welcome, Orienteerer!\n");

	int old_val = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);

	while (1) {

		printf("Pi request signal is: %d", read_pi_request());
		printf("Current state is: %d", curr_state);

		FSM_States next_state = curr_state;

		switch (curr_state) {
		case ready:
			printf("State: Ready\n");
			next_state = waiting_for_image_data;

			// Pi request signal is initially low
			while (!read_pi_request())
				;

			// Wait for pi to initialize request
			while (read_pi_request())
				;
			break;
		case waiting_for_image_data:
			printf("State: Waiting for image data\n");

			// Wait for pi to acknowledge state change by lowering its request signal
			while (read_pi_request())
				;
			next_state = reading_image_data;

			break;
		case reading_image_data: {
			printf("State: Reading image data\n");

			// The 2nd byte should contain the total number of bytes to read
			int count = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 1);
			char * buffer = malloc(sizeof(char) * count);
			read_ram_data(count, buffer);

			// TODO store the recently read data to SD card

			// The first byte should be 1 if there is more data after this batch, 0 otherwise
			int finished = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);
			if (finished) {
				next_state = waiting_for_path_data;
			} else {
				next_state = waiting_for_image_data;
			}
		}
			break;
		case waiting_for_path_data:
			printf("State: Waiting for path data\n");

			// Wait for pi to acknowledge state change by lowering its request signal
			while (read_pi_request())
				;
			next_state = reading_path_data;

			break;
		case reading_path_data: {
			printf("State: Reading path data\n");

			// The 2nd byte should contain the total number of bytes to read
			int count = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 1);
			char * buffer = malloc(sizeof(char) * count);
			read_ram_data(count, buffer);

			// TODO store the recently read data to SD card

			// The first byte should be 1 if there is more data after this batch, 0 otherwise
			int finished = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);
			if (finished) {
				next_state = drawing_image;
			} else {
				next_state = waiting_for_path_data;
			}
		}
			break;
		case drawing_image:
			printf("State: Drawing image\n");

			// TODO Draw image -- implement this function
			draw_image();
			next_state = drawing_path;

			break;
		case drawing_path:
			printf("State: Drawing path\n");

			//TODO Draw path -- implement this function
			draw_path();
			next_state = ready;

			break;
		default:
			next_state = ready;
			break;
		}

		// Set the new state and update signal
		curr_state = next_state;
		write_fsm_state(curr_state);

		// TODO Remove
		// Test the dual port ram by reading the first byte
		int val = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);
		if (old_val != val) {
			old_val = val;
			printf("%x\n", val);
		}

	}

	return 0;
}
