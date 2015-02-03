/*
 * Audio core testing
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <Altera_UP_SD_Card_Avalon_Interface.h>
#include "stdio.h"
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"
#include "altera_up_avalon_ps2.h"
#include <altera_up_ps2_keyboard.h>
#include <altera_up_ps2_mouse.h>

// Set up the audio/video IP core made in SOPC
void av_config_setup() {
	alt_up_av_config_dev * av_config;
	av_config = alt_up_av_config_open_dev("/dev/audio_and_video_config_0");

	while (!alt_up_av_config_read_ready(av_config)) {

	}
}

int main(void) {

	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/sd_card");

	if (device_reference != NULL) {

		while (1) {

			if ((connected == 0) && (alt_up_sd_card_is_Present())) {

				printf("Card connected.\n");

				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");

					// Start opening audio file
					short int audio_handle = alt_up_sd_card_fopen("audio.wav",
							0);

					if (audio_handle >= 0) {

						// Start audio playback
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

						// TEMP

						KB_CODE_TYPE code_type;
						unsigned char buf;
						char ascii;
						char asciiTemp;

						alt_up_ps2_dev * ps2 = alt_up_ps2_open_dev(
								"/dev/ps2_0");
						alt_up_ps2_init(ps2);

						asciiTemp = ascii;

						// TEMP

						do {

							// TEMP

							decode_scancode(ps2, &code_type, &buf, &ascii);
							if (asciiTemp != ascii) {
								printf("Character: %c \n", ascii);
							}
							asciiTemp = ascii;

							if (asciiTemp != ascii) {
								printf("Character: %c \n", ascii);
							}
							asciiTemp = ascii;

							// TEMP

							short left16 = ((unsigned char) byte_l1 << 8)
									| (unsigned char) byte_l0;

							// Play music
							buf_left[0] = (int) (left16 / 50);

							while (alt_up_audio_write_fifo_space(audioPlayback,
									ALT_UP_AUDIO_LEFT) < 1)
								;

							alt_up_audio_write_fifo(audioPlayback, buf_left, 1,
									ALT_UP_AUDIO_LEFT);
							alt_up_audio_write_fifo(audioPlayback, buf_left, 1,
									ALT_UP_AUDIO_RIGHT);

							// Read next bytes of data
							byte_l0 = alt_up_sd_card_read(audio_handle);
							byte_l1 = alt_up_sd_card_read(audio_handle);

							// Continue playback while data is available
						} while ((byte_l0 >= 0) || (byte_l1 >= 0));

					} else {
						fprintf(stderr, "Error opening audio file.\n");
						break;
					}
				} else {
					printf("Unknown file system.\n");
				}
				connected = 1;
			} else if ((connected == 1)
					&& (alt_up_sd_card_is_Present() == false)) {

				printf("Card disconnected.\n");

				connected = 0;
			}
		}
	}

	return 0;
}

