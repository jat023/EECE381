/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_qsys_0' in SOPC Builder design 'nios_system'
 * SOPC Builder design path: C:/Users/Kyle/Dropbox/School/CPEN_Y3_T2/EECE381/Projects/module_2_v2/quartus/nios_system.sopcinfo
 *
 * Generated: Wed Apr 08 11:29:58 PDT 2015
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_qsys"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x2820
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "tiny"
#define ALT_CPU_DATA_ADDR_WIDTH 0x19
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x1000020
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 0
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x19
#define ALT_CPU_NAME "nios2_qsys_0"
#define ALT_CPU_RESET_ADDR 0x1000000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x2820
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "tiny"
#define NIOS2_DATA_ADDR_WIDTH 0x19
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x1000020
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 0
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 0
#define NIOS2_ICACHE_LINE_SIZE_LOG2 0
#define NIOS2_ICACHE_SIZE 0
#define NIOS2_INST_ADDR_WIDTH 0x19
#define NIOS2_RESET_ADDR 0x1000000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_QSYS
#define __ALTERA_UP_AVALON_CHARACTER_LCD
#define __ALTERA_UP_AVALON_SRAM
#define __ALTERA_UP_AVALON_VIDEO_PIXEL_BUFFER_DMA
#define __ALTERA_UP_SD_CARD_AVALON_INTERFACE
#define __LINE_DRAWER


/*
 * LEDs configuration
 *
 */

#define ALT_MODULE_CLASS_LEDs altera_avalon_pio
#define LEDS_BASE 0x38d0
#define LEDS_BIT_CLEARING_EDGE_REGISTER 0
#define LEDS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDS_CAPTURE 0
#define LEDS_DATA_WIDTH 4
#define LEDS_DO_TEST_BENCH_WIRING 0
#define LEDS_DRIVEN_SIM_VALUE 0
#define LEDS_EDGE_TYPE "NONE"
#define LEDS_FREQ 50000000
#define LEDS_HAS_IN 0
#define LEDS_HAS_OUT 1
#define LEDS_HAS_TRI 0
#define LEDS_IRQ -1
#define LEDS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LEDS_IRQ_TYPE "NONE"
#define LEDS_NAME "/dev/LEDs"
#define LEDS_RESET_VALUE 0
#define LEDS_SPAN 16
#define LEDS_TYPE "altera_avalon_pio"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone II"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_0"
#define ALT_STDERR_BASE 0x38f0
#define ALT_STDERR_DEV jtag_uart_0
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_0"
#define ALT_STDIN_BASE 0x38f0
#define ALT_STDIN_DEV jtag_uart_0
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_0"
#define ALT_STDOUT_BASE 0x38f0
#define ALT_STDOUT_DEV jtag_uart_0
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "nios_system"


/*
 * character_lcd_0 configuration
 *
 */

#define ALT_MODULE_CLASS_character_lcd_0 altera_up_avalon_character_lcd
#define CHARACTER_LCD_0_BASE 0x38f8
#define CHARACTER_LCD_0_IRQ -1
#define CHARACTER_LCD_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CHARACTER_LCD_0_NAME "/dev/character_lcd_0"
#define CHARACTER_LCD_0_SPAN 2
#define CHARACTER_LCD_0_TYPE "altera_up_avalon_character_lcd"


/*
 * de2_ack configuration
 *
 */

#define ALT_MODULE_CLASS_de2_ack altera_avalon_pio
#define DE2_ACK_BASE 0x38a0
#define DE2_ACK_BIT_CLEARING_EDGE_REGISTER 0
#define DE2_ACK_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DE2_ACK_CAPTURE 0
#define DE2_ACK_DATA_WIDTH 1
#define DE2_ACK_DO_TEST_BENCH_WIRING 0
#define DE2_ACK_DRIVEN_SIM_VALUE 0
#define DE2_ACK_EDGE_TYPE "NONE"
#define DE2_ACK_FREQ 50000000
#define DE2_ACK_HAS_IN 0
#define DE2_ACK_HAS_OUT 1
#define DE2_ACK_HAS_TRI 0
#define DE2_ACK_IRQ -1
#define DE2_ACK_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DE2_ACK_IRQ_TYPE "NONE"
#define DE2_ACK_NAME "/dev/de2_ack"
#define DE2_ACK_RESET_VALUE 0
#define DE2_ACK_SPAN 16
#define DE2_ACK_TYPE "altera_avalon_pio"


/*
 * dual_port_ram configuration
 *
 */

#define ALT_MODULE_CLASS_dual_port_ram altera_avalon_onchip_memory2
#define DUAL_PORT_RAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define DUAL_PORT_RAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define DUAL_PORT_RAM_BASE 0x1000
#define DUAL_PORT_RAM_CONTENTS_INFO ""
#define DUAL_PORT_RAM_DUAL_PORT 1
#define DUAL_PORT_RAM_GUI_RAM_BLOCK_TYPE "AUTO"
#define DUAL_PORT_RAM_INIT_CONTENTS_FILE "nios_system_dual_port_ram"
#define DUAL_PORT_RAM_INIT_MEM_CONTENT 1
#define DUAL_PORT_RAM_INSTANCE_ID "NONE"
#define DUAL_PORT_RAM_IRQ -1
#define DUAL_PORT_RAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DUAL_PORT_RAM_NAME "/dev/dual_port_ram"
#define DUAL_PORT_RAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define DUAL_PORT_RAM_RAM_BLOCK_TYPE "AUTO"
#define DUAL_PORT_RAM_READ_DURING_WRITE_MODE "DONT_CARE"
#define DUAL_PORT_RAM_SINGLE_CLOCK_OP 0
#define DUAL_PORT_RAM_SIZE_MULTIPLE 1
#define DUAL_PORT_RAM_SIZE_VALUE 4096
#define DUAL_PORT_RAM_SPAN 4096
#define DUAL_PORT_RAM_TYPE "altera_avalon_onchip_memory2"
#define DUAL_PORT_RAM_WRITABLE 1


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER_SYSTEM
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0x38f0
#define JTAG_UART_0_IRQ 0
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 64
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 64
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * keys configuration
 *
 */

#define ALT_MODULE_CLASS_keys altera_avalon_pio
#define KEYS_BASE 0x38e0
#define KEYS_BIT_CLEARING_EDGE_REGISTER 0
#define KEYS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEYS_CAPTURE 0
#define KEYS_DATA_WIDTH 4
#define KEYS_DO_TEST_BENCH_WIRING 0
#define KEYS_DRIVEN_SIM_VALUE 0
#define KEYS_EDGE_TYPE "NONE"
#define KEYS_FREQ 50000000
#define KEYS_HAS_IN 1
#define KEYS_HAS_OUT 0
#define KEYS_HAS_TRI 0
#define KEYS_IRQ -1
#define KEYS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define KEYS_IRQ_TYPE "NONE"
#define KEYS_NAME "/dev/keys"
#define KEYS_RESET_VALUE 0
#define KEYS_SPAN 16
#define KEYS_TYPE "altera_avalon_pio"


/*
 * line_drawer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_line_drawer_0 line_drawer
#define LINE_DRAWER_0_BASE 0x3840
#define LINE_DRAWER_0_IRQ -1
#define LINE_DRAWER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LINE_DRAWER_0_NAME "/dev/line_drawer_0"
#define LINE_DRAWER_0_SPAN 32
#define LINE_DRAWER_0_TYPE "line_drawer"


/*
 * onchip_memory2_0 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory2_0 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_BASE 0x0
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE "nios_system_onchip_memory2_0"
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_0_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY2_0_IRQ -1
#define ONCHIP_MEMORY2_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY2_0_NAME "/dev/onchip_memory2_0"
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 4096
#define ONCHIP_MEMORY2_0_SPAN 4096
#define ONCHIP_MEMORY2_0_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY2_0_WRITABLE 1


/*
 * pi_request configuration
 *
 */

#define ALT_MODULE_CLASS_pi_request altera_avalon_pio
#define PI_REQUEST_BASE 0x38b0
#define PI_REQUEST_BIT_CLEARING_EDGE_REGISTER 0
#define PI_REQUEST_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PI_REQUEST_CAPTURE 0
#define PI_REQUEST_DATA_WIDTH 1
#define PI_REQUEST_DO_TEST_BENCH_WIRING 0
#define PI_REQUEST_DRIVEN_SIM_VALUE 0
#define PI_REQUEST_EDGE_TYPE "NONE"
#define PI_REQUEST_FREQ 50000000
#define PI_REQUEST_HAS_IN 1
#define PI_REQUEST_HAS_OUT 0
#define PI_REQUEST_HAS_TRI 0
#define PI_REQUEST_IRQ -1
#define PI_REQUEST_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PI_REQUEST_IRQ_TYPE "NONE"
#define PI_REQUEST_NAME "/dev/pi_request"
#define PI_REQUEST_RESET_VALUE 0
#define PI_REQUEST_SPAN 16
#define PI_REQUEST_TYPE "altera_avalon_pio"


/*
 * pixel_buffer configuration
 *
 */

#define ALT_MODULE_CLASS_pixel_buffer altera_up_avalon_sram
#define PIXEL_BUFFER_BASE 0x80000
#define PIXEL_BUFFER_IRQ -1
#define PIXEL_BUFFER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_NAME "/dev/pixel_buffer"
#define PIXEL_BUFFER_SPAN 524288
#define PIXEL_BUFFER_TYPE "altera_up_avalon_sram"


/*
 * pixel_buffer_dma configuration
 *
 */

#define ALT_MODULE_CLASS_pixel_buffer_dma altera_up_avalon_video_pixel_buffer_dma
#define PIXEL_BUFFER_DMA_BASE 0x38c0
#define PIXEL_BUFFER_DMA_IRQ -1
#define PIXEL_BUFFER_DMA_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_DMA_NAME "/dev/pixel_buffer_dma"
#define PIXEL_BUFFER_DMA_SPAN 16
#define PIXEL_BUFFER_DMA_TYPE "altera_up_avalon_video_pixel_buffer_dma"


/*
 * sd_card configuration
 *
 */

#define ALT_MODULE_CLASS_sd_card Altera_UP_SD_Card_Avalon_Interface
#define SD_CARD_BASE 0x3400
#define SD_CARD_IRQ -1
#define SD_CARD_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SD_CARD_NAME "/dev/sd_card"
#define SD_CARD_SPAN 1024
#define SD_CARD_TYPE "Altera_UP_SD_Card_Avalon_Interface"


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x1000000
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x16
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 8
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 12
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 8388608
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * timer_system configuration
 *
 */

#define ALT_MODULE_CLASS_timer_system altera_avalon_timer
#define TIMER_SYSTEM_ALWAYS_RUN 0
#define TIMER_SYSTEM_BASE 0x3880
#define TIMER_SYSTEM_COUNTER_SIZE 32
#define TIMER_SYSTEM_FIXED_PERIOD 0
#define TIMER_SYSTEM_FREQ 50000000
#define TIMER_SYSTEM_IRQ 1
#define TIMER_SYSTEM_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_SYSTEM_LOAD_VALUE 49999
#define TIMER_SYSTEM_MULT 0.0010
#define TIMER_SYSTEM_NAME "/dev/timer_system"
#define TIMER_SYSTEM_PERIOD 1
#define TIMER_SYSTEM_PERIOD_UNITS "ms"
#define TIMER_SYSTEM_RESET_OUTPUT 0
#define TIMER_SYSTEM_SNAPSHOT 1
#define TIMER_SYSTEM_SPAN 32
#define TIMER_SYSTEM_TICKS_PER_SEC 1000.0
#define TIMER_SYSTEM_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_SYSTEM_TYPE "altera_avalon_timer"


/*
 * timer_timestamp configuration
 *
 */

#define ALT_MODULE_CLASS_timer_timestamp altera_avalon_timer
#define TIMER_TIMESTAMP_ALWAYS_RUN 0
#define TIMER_TIMESTAMP_BASE 0x3860
#define TIMER_TIMESTAMP_COUNTER_SIZE 32
#define TIMER_TIMESTAMP_FIXED_PERIOD 0
#define TIMER_TIMESTAMP_FREQ 50000000
#define TIMER_TIMESTAMP_IRQ 2
#define TIMER_TIMESTAMP_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_TIMESTAMP_LOAD_VALUE 49999
#define TIMER_TIMESTAMP_MULT 0.0010
#define TIMER_TIMESTAMP_NAME "/dev/timer_timestamp"
#define TIMER_TIMESTAMP_PERIOD 1
#define TIMER_TIMESTAMP_PERIOD_UNITS "ms"
#define TIMER_TIMESTAMP_RESET_OUTPUT 0
#define TIMER_TIMESTAMP_SNAPSHOT 1
#define TIMER_TIMESTAMP_SPAN 32
#define TIMER_TIMESTAMP_TICKS_PER_SEC 1000.0
#define TIMER_TIMESTAMP_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TIMESTAMP_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
