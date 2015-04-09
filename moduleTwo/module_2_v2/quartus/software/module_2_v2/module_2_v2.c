#include <stdio.h>
#include <time.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <stdlib.h>

#include "system.h"
#include "io.h"

#define PI_REQ (volatile char *) 0x38b0
#define DE2_ACK (char *) 0x38a0
#define X_RES 320
#define Y_RES 240
#define IMG_BUF_SIZE 76800

// Structures
typedef enum {
	idle = 0,
	ready = 1,
	wait_for_image_data = 2,
	read_and_draw_image_data = 3,
	wait_for_path_data = 4,
	read_and_draw_path_data = 5,
} FSM_States;

// Global variables
FSM_States curr_state = idle; // FSM State
int count = 0; // Keep track of image pixel location

/**
 * Writes the acknowledge signal to the corresponding GPIO pin based on the given state
 *
 * Params
 * 	state: the FSM state
 *
 * Modifies
 * 	The corresponding GPIO pins are updated to reflect the expected ack signal corresponding to the given state
 */
void write_de2_ack(FSM_States state) {
	int rep = (int) state;
	*DE2_ACK = rep % 2;
}

/**
 * Reads the pi request signal
 *
 * Returns 1 if the signal is on, 0 otherwise
 */
int read_pi_request() {
	return IORD(PI_REQ, 0);
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
void read_ram_data(int size, char * buffer) {

	printf("Size: %d\n", size);

	int offset = 3;
	int i;
	for (i = 0; i < size; i++) {
		*(buffer + i) = IORD_8DIRECT(DUAL_PORT_RAM_BASE, i + offset);
	}
}

/**
 * Initializes the pixel buffer and clears the screen
 */
alt_up_pixel_buffer_dma_dev* init_pixelbuffer(
		alt_up_pixel_buffer_dma_dev* pixel_buffer) {
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
	return pixel_buffer;
}

void DrawCircle(int x0, int y0, int radius,
		alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	int x = radius;
	int y = 0;
	int radiusError = 1 - x;

	while (x >= y) {
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (-x + x0), (y + y0),
				(x + x0), (y + y0), 0x0000, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (-y + x0), (x + y0),
				(y + x0), (x + y0), 0x0000, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (-x + x0), (-y + y0),
				(x + x0), (-y + y0), 0x0000, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, (-y + x0), (-x + y0),
				(y + x0), (-x + y0), 0x0000, 0);
		y++;
		if (radiusError < 0) {
			radiusError += 2 * y + 1;
		} else {
			x--;
			radiusError += 2 * (y - x) + 1;
		}
	}
}

void DrawStickMan(int x, int y, int thickness, int headThickness,
		alt_up_pixel_buffer_dma_dev* pixel_buffer) {

	int neckX = x; //190
	int neckY = y; //90
	int headrad = headThickness; //15
	int headY = neckY - headrad + 2;
	int headX = neckX + headrad - 4;
	int elbowLeftX = neckX - 30;
	int elbowLeftY = neckY - 30;
	int elbowRightX = neckX + 30;
	int elbowRightY = neckY + 30;
	int buttX = neckX - 40;
	int buttY = neckY + 40;
	int kneeRightX = buttX + 30;
	int kneeRightY = buttY + 23;
	int kneeLeftX = buttX - 30;
	int kneeLeftY = buttY + 23;
	int armExt = 20;
	int legExt = 25;
	int color = 0x0000;
	int i;

	for (i = 0; i <= thickness; i++) {
		//Draw head/body and inner arms/legs
		DrawCircle(headX - thickness, headY, headrad, pixel_buffer);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, neckX - i, neckY,
				elbowLeftX - i, elbowLeftY, color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, neckX - i, neckY,
				elbowRightX - i, elbowRightY, color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, neckX - i, neckY,
				buttX - i, buttY, color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, buttX - i, buttY,
				kneeRightX - i, kneeRightY, color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, buttX - i, buttY,
				kneeLeftX - i, kneeLeftY, color, 0);
		//Draw outer arms/legs
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, elbowLeftX - i,
				elbowLeftY, elbowLeftX - i - armExt, elbowLeftY + armExt, color,
				0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, elbowRightX - i,
				elbowRightY, elbowRightX - i + armExt, elbowRightY - armExt,
				color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, kneeRightX - i,
				kneeRightY, kneeRightX - i, kneeRightY + legExt, color, 0);
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, kneeLeftX - i,
				kneeLeftY, kneeLeftX - i - legExt, kneeLeftY - legExt, color,
				0);
	}
}

void DrawMainScreen(alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer, 0, 0, 320, 240, 0xFFFF, 0);
	DrawStickMan(190, 90, 1, 15, pixel_buffer);
}

int getColor(char colorByte) {
	int color;
	switch (colorByte) {
	case 1:
		//White
		color = 0xFFFF;
		break;
	case 2:
		//Dark Green
		color = 0x0300;
		break;
	case 3:
		//Green
		color = 0x0F00;
		break;
	case 4:
		//Light Green
		color = 0xAFA0;
		break;
	case 5:
		//Yellow
		color = 0xFF00;
		break;
	case 6:
		//Black
		color = 0x0000;
		break;
	case 7:
		//Purple
		color = 0x9090;
		break;
	case 8:
		//Blue
		color = 0x00F0;
		break;
	case 9:
		//Olive Green
		color = 0xAD00;
		break;
	case 10:
		//Brown
		color = 0xA300;
		break;
	default:
		//White by default
		color = 0xFFFF;
		break;
	}
	return color;
}

/**
 * Draws the image starting at the location specified by pixelStart
 * reads data from the buffer which should contain colors specified by a single byte
 * Clears the screen if pixelStart = 0
 */
void draw_pixels(int pixelStart, int numPixels, int resolutionX,
		int resolutionY, char* buffer,
		alt_up_pixel_buffer_dma_dev* pixel_buffer) {
	if (pixelStart == 0) {
		alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
	}

	int i, j;
	int offset = 0;
	int startX = pixelStart % resolutionX;
	int startY = pixelStart / resolutionX;

	for (i = startX; i < resolutionX; i++) {
		alt_up_pixel_buffer_dma_draw_line(pixel_buffer, i, startY, i, startY,
				getColor(buffer[offset]), 0);
		//alt_up_pixel_buffer_dma_draw(pixel_buffer,getColor(buffer[offset]),i,startY);
		numPixels--;
		offset++;
		if (numPixels == 0) {
			return;
		}
	}
	for (j = startY + 1; j < resolutionY; j++) {
		for (i = 0; i < resolutionX; i++) {
			alt_up_pixel_buffer_dma_draw_line(pixel_buffer, i, j, i, j,
					getColor(buffer[offset]), 0);
			//alt_up_pixel_buffer_dma_draw(pixel_buffer,getColor(buffer[offset]),i,j);
			numPixels--;
			offset++;
			if (numPixels == 0) {
				return;
			}
		}
	}

	//debugging print statements:
	//printf("Drawing pixel2\n");
	//printf("Coords2:%d,%d,%d,%d\n", i,startY,i,startY);
	//printf("Color2 = %d\n", getColor(buffer[offset]));
}

// TODO Implement
/**
 * Draws the image -- this needs to be implemented
 */
int draw_image(alt_up_pixel_buffer_dma_dev* pixel_buffer) {

	int more_data = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);

	// Determine the number of valid bytes to read
	int left = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 1) << 8;
	int right = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 2);
	int size = left | right;

	char * buffer = malloc(sizeof(char) * size);
	read_ram_data(size, buffer);

	write_de2_ack(curr_state);

	// Draw image section
	draw_pixels(count, size, X_RES, Y_RES, buffer, pixel_buffer);

	// Free buffer
	free(buffer);

	// Update count
	count += size;

	printf("More data: %x\n", more_data);

	return more_data;
}

void draw_line(int x, int y, int x2, int y2, int color) {
	// Set x1
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 0, x);
	// Set y1
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 4, y);
	// Set x2
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 8, x2);
	// Set y2
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 12, y2);
	// Set colour
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 16, color);
	// Start drawing
	IOWR_32DIRECT(LINE_DRAWER_0_BASE, 20, 1);
	// wait until done
	while (IORD_32DIRECT(LINE_DRAWER_0_BASE,20) == 0)
		;
}

// TODO Implement
/**
 * Draws the path -- this needs to be implemented
 */
int draw_path(alt_up_pixel_buffer_dma_dev* pixel_buffer) {

	printf("starting path function\n");
	int i = 0;
	int whichByte;
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int x_prev = 0;
	unsigned int y_prev = 0;

	int loopCount = 0;

	int more_data = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);

	// Determine the number of valid bytes to read
	int left = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 1) << 8;
	int right = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 2);
	int size = left | right;

	printf("reading\n");
	char * buffer = malloc(sizeof(char) * size);
	read_ram_data(size, buffer);

	write_de2_ack(curr_state);

	left = *buffer << 8;
	right = *(buffer + 1);
	x = left | right;
	y = *(buffer + 2);

	for(i = 3; i < size; i+=3){
		x_prev = x;
		y_prev = y;

		left = *(buffer + i) << 8;
		right = *(buffer + i + 1);
		x = left | right;
		y = *(buffer + i + 2);

		draw_line(x_prev, y_prev, x, y, 0xF000);
	}

	// Free buffer
	free(buffer);

	printf("More data: %x\n", more_data);

	return more_data;
}

int main() {

	printf("Welcome, Orienteerer!\n");

	// Initialize next state to current state
	FSM_States next_state = curr_state;

	alt_up_pixel_buffer_dma_dev* pixel_buffer = init_pixelbuffer(pixel_buffer); // Pixel buffer
	int mainScreen = 0;
	int bool = 0;

	while (1) {

		//printf("Pi request signal is: %d\n", read_pi_request());
		//printf("Current state is: %d\n", curr_state);

		switch (curr_state) {
		case idle:
			// Triggered by PI_REQ LOW
			if (!read_pi_request()) {
				next_state = ready;
				write_de2_ack(curr_state);
			}
			if (mainScreen == 0) {
				DrawMainScreen(pixel_buffer);
				mainScreen = 1;
			}
			break;
		case ready:
			count = 0;

			if (read_pi_request()) {
				// Triggered by PI_REQ HIGH
				next_state = wait_for_image_data;
				write_de2_ack(curr_state);
			}
			break;
		case wait_for_image_data:

			if (!read_pi_request()) {
				// Triggered by PI_REQ LOW
				next_state = read_and_draw_image_data;
				write_de2_ack(curr_state);
			}

			break;
		case read_and_draw_image_data:
			// Read and draw IMAGE

			if (read_pi_request()) {
				if (draw_image(pixel_buffer) == 1) {
					// If more data is available
					next_state = wait_for_image_data;
				} else {
					next_state = wait_for_path_data;
				}
			}

			break;
		case wait_for_path_data:

			if (!read_pi_request()) {
				// Triggered by PI_REQ LOW
				next_state = read_and_draw_path_data;
				write_de2_ack(curr_state);
				bool = 0;
			}

			break;
		case read_and_draw_path_data:
			// Read and draw PATH

			if (read_pi_request()) {
				if (draw_path(pixel_buffer) == 1) {
					// If more data is available
					next_state = wait_for_path_data;
				} else {
					next_state = idle;
				}
			}

			break;
		default:
			next_state = idle;
			break;
		}

		if (next_state != curr_state) {
			curr_state = next_state;
			printf("State: %d\n", curr_state);
		}

	}

	return 0;
}
