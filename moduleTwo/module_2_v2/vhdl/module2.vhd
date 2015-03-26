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

ENTITY module2 IS
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
	
	-- Dual-port RAM
	GPIO_1 		: INOUT STD_LOGIC_VECTOR(35 downto 0)
);
END;

ARCHITECTURE Structure OF module2 IS
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
		
		-- Dual-port RAM
		dual_port_ram_s2_address    : in    std_logic_vector(11 downto 0) := "000000000000"; -- address
      dual_port_ram_s2_chipselect : in    std_logic                     := 'X';             -- chipselect
      dual_port_ram_s2_clken      : in    std_logic                     := 'X';             -- clken
      dual_port_ram_s2_write      : in    std_logic                     := 'X';             -- write
      dual_port_ram_s2_readdata   : out   std_logic_vector(7 downto 0);                     -- readdata
      dual_port_ram_s2_writedata  : in    std_logic_vector(7 downto 0)  := "00000000"; -- writedata
      clk_pi_clk                  : in    std_logic                     := 'X'              -- clk
	);
	END COMPONENT;

	SIGNAL DQM 	: STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL BA 	: STD_LOGIC_VECTOR(1 DOWNTO 0);
	
	BEGIN
	DRAM_BA_0 <= BA(0);
	DRAM_BA_1 <= BA(1);
	DRAM_UDQM <= DQM(1);
	DRAM_LDQM <= DQM(0);

	GPIO_1(3) <= CLOCK_50;
	
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
		
		-- Dual-port RAM
		dual_port_ram_s2_chipselect	=> '1',
		dual_port_ram_s2_clken			=> '1',
		dual_port_ram_s2_address    	=> GPIO_1(35 downto 24),    	-- dual_port_ram_s2.address
		
		-- dual_port_ram_s2_address    	=> "000000000000",    	-- dual_port_ram_s2.address

		dual_port_ram_s2_readdata   	=> GPIO_1(23 downto 16),   	--                 .readdata
      dual_port_ram_s2_writedata  	=> GPIO_1(15 downto 8),  		--                 .writedata
      dual_port_ram_s2_write      	=> GPIO_1(7),      				--                 .write
      clk_pi_clk                  	=> GPIO_1(6)	               --           clk_pi.clk
	);
END Structure;