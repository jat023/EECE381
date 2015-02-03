library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity VGA_graphics is
	PORT (
		SW : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		KEY : IN STD_LOGIC_VECTOR(0 DOWNTO 0);
		CLOCK_50 : IN STD_LOGIC;
		LEDG : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		DRAM_CLK, DRAM_CKE : OUT STD_LOGIC;
		DRAM_ADDR : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
		DRAM_BA_0, DRAM_BA_1 : BUFFER STD_LOGIC;
		DRAM_CS_N, DRAM_CAS_N, DRAM_RAS_N, DRAM_WE_N : OUT STD_LOGIC;
		DRAM_DQ : INOUT STD_LOGIC_VECTOR(15 DOWNTO 0);
		DRAM_UDQM, DRAM_LDQM : BUFFER STD_LOGIC;
		
		VGA_R:out	std_logic_vector(9	downto	0);	
		VGA_G:out	std_logic_vector(9	downto	0);	
		VGA_B:out	std_logic_vector(9	downto	0);	
		VGA_CLK:	out	std_logic;
		VGA_BLANK:	out	std_logic;	
		VGA_HS:out	std_logic;	
		VGA_VS:out	std_logic;	
		VGA_SYNC:out	std_logic;
		
		SRAM_DQ	:	INOUT	STD_LOGIC_VECTOR(15	downto	0);
		SRAM_ADDR	:	OUT	STD_LOGIC_VECTOR(17	downto	0);
		SRAM_LB_N	:	OUT	STD_LOGIC;
		SRAM_UB_N	:	OUT	STD_LOGIC;
		SRAM_CE_N	:	OUT	STD_LOGIC;
		SRAM_OE_N	:	OUT	STD_LOGIC;
		SRAM_WE_N	:	OUT	STD_LOGIC
		);
END VGA_graphics;


ARCHITECTURE Structure OF VGA_graphics IS
    component VGA_nios_system is
        port (
            clk_clk              : in    std_logic                     := 'X';             -- clk
            reset_reset_n        : in    std_logic                     := 'X';             -- reset_n
            switches_export      : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
            leds_export          : out   std_logic_vector(7 downto 0);                     -- export
            sdram_wire_addr      : out   std_logic_vector(11 downto 0);                    -- addr
            sdram_wire_ba        : out   std_logic_vector(1 downto 0);                     -- ba
            sdram_wire_cas_n     : out   std_logic;                                        -- cas_n
            sdram_wire_cke       : out   std_logic;                                        -- cke
            sdram_wire_cs_n      : out   std_logic;                                        -- cs_n
            sdram_wire_dq        : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_wire_dqm       : out   std_logic_vector(1 downto 0);                     -- dqm
            sdram_wire_ras_n     : out   std_logic;                                        -- ras_n
            sdram_wire_we_n      : out   std_logic;                                        -- we_n
            sdram_clk_clk        : out   std_logic;                                        -- clk
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
            vga_controller_B     : out   std_logic_vector(9 downto 0)                      -- B
        );
    end component VGA_nios_system;

	SIGNAL DQM : STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL BA : STD_LOGIC_VECTOR(1 DOWNTO 0);
	
	BEGIN
	
	DRAM_BA_0 <= BA(0);
	DRAM_BA_1 <= BA(1);
	DRAM_UDQM <= DQM(1);
	DRAM_LDQM <= DQM(0);
	
    System : component VGA_nios_system
        port map (
            clk_clk => CLOCK_50,
            reset_reset_n => KEY(0),
            switches_export => SW,
            leds_export => LEDG,
				
            sdram_clk_clk => DRAM_CLK,
				
				sdram_wire_addr => DRAM_ADDR,
				sdram_wire_ba => BA,
				sdram_wire_cas_n => DRAM_CAS_N,
				sdram_wire_cke => DRAM_CKE,
				sdram_wire_cs_n => DRAM_CS_N,
				sdram_wire_dq => DRAM_DQ,
				sdram_wire_dqm => DQM,
				sdram_wire_ras_n => DRAM_RAS_N,
				sdram_wire_we_n => DRAM_WE_N,
				
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
				vga_controller_B	=>	VGA_B
        );
END Structure;
