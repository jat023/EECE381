/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include <time.h>
#include "system.h"
#include "io.h"


typedef enum {
	ready = 0,
	waiting_for_image_data = 1,
	reading_image_data = 2,
	waiting_for_path = 3,
	reading_path_data = 4,
	drawing_image = 5,
	drawing_path = 6
} FSM_States;

FSM_States curr_state = ready;

// Writes the FSM state to the corresponding GPIO pins
// TODO Implement
void write_fsm_state(FSM_States state) {
	printf("%d\n", state); // Dummy function
}

// Read the pi request signal from the corresponding GPIO pin
// TODO Implement
int read_pi_request(){
	srand(time(NULL));
	return rand() % 2; // Dummy value
}

int main() {
  printf("Hello from Nios II!\n");

  while(1) {

	  // Set the fsm state signal to current state
	  write_fsm_state(curr_state);
	  FSM_States next_state = curr_state;

	  switch(curr_state) {
      
      /*
	  	  case ready:
	  		  while(!read_pi_request());
	  		  next_state = waiting_for_image_data;
	  		  while(read_pi_request());
	  		  break;
	  	  case waiting_for_image_data:
	  		  while(!read_pi_request());
	  		  next_state = reading_image_data;
	  		  while(read_pi_request());
	  		  break;
	  	  case reading_image_data:
	  		  break;
	  	  case waiting_for_path:
	  		  while(!read_pi_request());
	  		  next_state = reading_path_data;
	  		  while(read_pi_request());
	  		  break;
	  	  case reading_path_data:
	  		  break;
	  	  case drawing_image:
	  		  break;
	  	  case drawing_path:
	  		  break;
	  	  default:
	  		  next_state = ready;
	  		  break;
	  }
    */

	  // Set the new state;
	  curr_state = next_state;

	  int val = IORD_8DIRECT(DUAL_PORT_RAM_BASE, 0);
	  if(old_val != val) {
		  old_val = val;
		  printf("%x\n", val);
	  }

  }
  
  return 0;
}
