-- Implements a simple Nios II system for the DE2 board.
-- Inputs: SW7°0 are parallel port INputs to the Nios II system.
-- CLOCK_50 is the system clock.
-- KEY0 is the active-low system reset.
-- Outputs: LEDG7°0 are parallel port OUTputs from the Nios II system.
-- SDRAM ports correspond to the signals IN Figure 2; their names are those
-- used IN the DE2 User Manual.

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;
USE ieee.std_logic_unsigned.all;

ENTITY module1 IS
PORT (
	SW 			: IN STD_LOGIC_VECTOR(0 DOWNTO 0);
	KEY 			: IN STD_LOGIC_VECTOR(3 DOWNTO 0);
	CLOCK_50 	: IN STD_LOGIC;
	LEDG 			: OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
	DRAM_CLK  	: OUT STD_LOGIC;
	DRAM_CKE		: OUT STD_LOGIC;
	DRAM_ADDR 	: OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
	DRAM_BA_0 	: BUFFER STD_LOGIC;
	DRAM_BA_1 	: BUFFER STD_LOGIC;
	DRAM_CS_N	: OUT STD_LOGIC;
	DRAM_CAS_N 	: OUT STD_LOGIC;
	DRAM_RAS_N 	: OUT STD_LOGIC;
	DRAM_WE_N 	: OUT STD_LOGIC;
	DRAM_DQ 		: INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	DRAM_UDQM 	: BUFFER STD_LOGIC;
	DRAM_LDQM	: BUFFER STD_LOGIC;
	LCD_DATA		: INOUT STD_LOGIC_VECTOR(7 downto 0);
	LCD_ON 		: OUT STD_LOGIC;
	LCD_BLON 	: OUT STD_LOGIC;
	LCD_EN 		: OUT STD_LOGIC;
	LCD_RS 		: OUT STD_LOGIC;
	LCD_RW 		: OUT STD_LOGIC;
	SD_CMD 		: INOUT STD_LOGIC;
	SD_DAT 		: INOUT STD_LOGIC;
	SD_DAT3 		: INOUT STD_LOGIC;
	SD_CLK 		: OUT STD_LOGIC;
	
	--VGA
	VGA_R:out	std_logic_vector(9	downto	0);	
	VGA_G:out	std_logic_vector(9	downto	0);	
	VGA_B:out	std_logic_vector(9	downto	0);	
	VGA_CLK:	out	std_logic;
	VGA_BLANK:	out	std_logic;	
	VGA_HS:out	std_logic;	
	VGA_VS:out	std_logic;	
	VGA_SYNC:out	std_logic;
	
	--SRAM
	SRAM_DQ	:	INOUT	STD_LOGIC_VECTOR(15	downto	0);
	SRAM_ADDR	:	OUT	STD_LOGIC_VECTOR(17	downto	0);
	SRAM_LB_N	:	OUT	STD_LOGIC;
	SRAM_UB_N	:	OUT	STD_LOGIC;
	SRAM_CE_N	:	OUT	STD_LOGIC;
	SRAM_OE_N	:	OUT	STD_LOGIC;
	SRAM_WE_N	:	OUT	STD_LOGIC;
	
	-- Audio
	I2C_SDAT		: INOUT STD_LOGIC;
	I2C_SCLK		: OUT STD_LOGIC;
	AUD_XCK		: OUT STD_LOGIC;
	CLOCK_27		: IN STD_LOGIC;
	AUD_ADCDAT	: IN STD_LOGIC;
	AUD_ADCLRCK	: IN STD_LOGIC;
	AUD_BCLK		: IN STD_LOGIC;
	AUD_DACDAT	: OUT STD_LOGIC;
	AUD_DACLRCK	: IN STD_LOGIC;
	
	-- Keyboard
	ps2_CLK 		: INOUT STD_LOGIC;
	ps2_DAT 		: INOUT STD_LOGIC
);
END module1;

ARCHITECTURE Structure OF module1 IS
	COMPONENT nios_system
	PORT (
		clk_clk 					: IN STD_LOGIC;
		reset_reset_n 			: IN STD_LOGIC;
		sdram_clk_clk 			: OUT STD_LOGIC;
		leds_export 			: OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
		keys_export 			: IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		sdram_wire_addr 		: OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
		sdram_wire_ba 			: BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
		sdram_wire_cas_n 		: OUT STD_LOGIC;
		sdram_wire_cke 		: OUT STD_LOGIC;
		sdram_wire_cs_n 		: OUT STD_LOGIC;
		sdram_wire_dq 			: INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		sdram_wire_dqm 		: BUFFER STD_LOGIC_VECTOR(1 DOWNTO 0);
		sdram_wire_ras_n 		: OUT STD_LOGIC;
		sdram_wire_we_n 		: OUT STD_LOGIC;
		lcd_data_DATA 			: INOUT STD_LOGIC_VECTOR(7 downto 0);
		lcd_data_ON 			: OUT STD_LOGIC;
		lcd_data_BLON 			: OUT STD_LOGIC;
		lcd_data_EN 			: OUT STD_LOGIC;
		lcd_data_RS 			: OUT STD_LOGIC;
		lcd_data_RW 			: OUT STD_LOGIC;
		sd_card_b_SD_cmd 		: INOUT STD_LOGIC;
		sd_card_b_SD_dat 		: INOUT STD_LOGIC;
		sd_card_b_SD_dat3		: INOUT STD_LOGIC;
		sd_card_o_SD_clock 	: OUT STD_LOGIC;
		sram_pixel_DQ        : inout std_logic_vector(15 downto 0) := (others => 'X'); -- DQ
		sram_pixel_ADDR      : out   std_logic_vector(17 downto 0);                    -- ADDR
		sram_pixel_LB_N      : out   std_logic;                                        -- LB_N
		sram_pixel_UB_N      : out   std_logic;                                        -- UB_N
		sram_pixel_CE_N      : out   std_logic;                                        -- CE_N
		sram_pixel_OE_N      : out   std_logic;                                        -- OE_N
		sram_pixel_WE_N      : out   std_logic;                                        -- WE_N
		vga_controller_CLK   : out   std_logic;                                        -- CLK
		vga_controller_HS    : out   std_logic;                                        -- HS
		vga_controller_VS    : out   std_logic;                                        -- VS
		vga_controller_BLANK : out   std_logic;                                        -- BLANK
		vga_controller_SYNC  : out   std_logic;                                        -- SYNC
		vga_controller_R     : out   std_logic_vector(9 downto 0);                     -- R
		vga_controller_G     : out   std_logic_vector(9 downto 0);                     -- G
		vga_controller_B     : out   std_logic_vector(9 downto 0);                      -- B
	
		-- Audio
		audio_clk_clk       	: OUT std_logic;
		av_config_SDAT			: INOUT std_logic;
		av_config_SCLK			: OUT std_logic;
		audio_ADCDAT       	: IN std_logic;
		audio_ADCLRCK      	: IN std_logic;
		audio_BCLK         	: IN std_logic;
		audio_DACDAT       	: OUT std_logic;
		audio_DACLRCK      	: IN std_logic;
		clk_27_clk         	: IN std_logic;
		
		-- Keyboard
		ps2_data_CLK			: INOUT std_logic;
		ps2_data_DAT 			: INOUT std_logic
	);
	END COMPONENT;

	SIGNAL DQM 	: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL BA 	: STD_LOGIC_VECTOR(1 DOWNTO 0);
	
	BEGIN
	DRAM_BA_0 <= BA(0);
	DRAM_BA_1 <= BA(1);
	DRAM_UDQM <= DQM(1);
	DRAM_LDQM <= DQM(0);

	-- Instantiate the Nios II system entity generated by the Qsys tool

	NiosII: nios_system
	PORT MAP (
		clk_clk 					=> CLOCK_50,
		reset_reset_n 			=> SW(0),
		sdram_clk_clk 			=> DRAM_CLK,
		leds_export 			=> LEDG,
		keys_export 			=> KEY(3 DOWNTO 0),
		sdram_wire_addr 		=> DRAM_ADDR,
		sdram_wire_ba 			=> BA,
		sdram_wire_cas_n 		=> DRAM_CAS_N,
		sdram_wire_cke 		=> DRAM_CKE,
		sdram_wire_cs_n 		=> DRAM_CS_N,
		sdram_wire_dq 			=> DRAM_DQ,
		sdram_wire_dqm 		=> DQM,
		sdram_wire_ras_n 		=> DRAM_RAS_N,
		sdram_wire_we_n 		=> DRAM_WE_N ,
		lcd_data_DATA 			=> LCD_DATA,
		lcd_data_ON 			=> LCD_ON,
		lcd_data_BLON 			=> LCD_BLON,
		lcd_data_EN 			=> LCD_EN,
		lcd_data_RS 			=> LCD_RS,
		lcd_data_RW 			=> LCD_RW,
		sd_card_b_SD_cmd 		=> SD_CMD,
		sd_card_b_SD_dat 		=> SD_DAT,
		sd_card_b_SD_dat3		=> SD_DAT3,
		sd_card_o_SD_clock 	=> SD_CLK,
		
		sram_pixel_DQ		=>	SRAM_DQ,
		sram_pixel_ADDR	=>	SRAM_ADDR,
		sram_pixel_LB_N	=>	SRAM_LB_N,
		sram_pixel_UB_N	=>	SRAM_UB_N,		
		sram_pixel_CE_N	=>	SRAM_CE_N,
		sram_pixel_OE_N	=>	SRAM_OE_N,
		sram_pixel_WE_N	=>	SRAM_WE_N,
		
		vga_controller_CLK	=>	VGA_CLK,	
		vga_controller_HS	=>	VGA_HS,	
		vga_controller_VS	=>	VGA_VS,	
		vga_controller_BLANK	=>	VGA_BLANK,	
		vga_controller_SYNC	=>	VGA_SYNC,	
		vga_controller_R	=>	VGA_R,	
		vga_controller_G	=>	VGA_G,	
		vga_controller_B	=>	VGA_B,
		
		-- Audio
		audio_clk_clk        => AUD_XCK,
      av_config_SDAT     	=> I2C_SDAT,
      av_config_SCLK     	=> I2C_SCLK,
      audio_ADCDAT       	=> AUD_ADCDAT,
      audio_ADCLRCK      	=> AUD_ADCLRCK,
      audio_BCLK         	=> AUD_BCLK,
      audio_DACDAT       	=> AUD_DACDAT,
      audio_DACLRCK      	=> AUD_DACLRCK,
      clk_27_clk         	=> CLOCK_27,
		
		-- Keyboard
		ps2_data_CLK => ps2_CLK,
		ps2_data_DAT => ps2_DAT
	);
END Structure;