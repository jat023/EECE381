--Adam Woods 39241120
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity lab4sun is
  port(CLOCK_50            : in  std_logic;
       KEY                 : in  std_logic_vector(3 downto 0);
       SW                  : in  std_logic_vector(17 downto 0);
       VGA_R, VGA_G, VGA_B : out std_logic_vector(9 downto 0);  -- The outs go to VGA controller
       VGA_HS              : out std_logic;
       VGA_VS              : out std_logic;
       VGA_BLANK           : out std_logic;
       VGA_SYNC            : out std_logic;
       VGA_CLK             : out std_logic;
		  LEDG : out std_logic_vector(7 downto 0);
		  LEDR : out std_logic_vector(17 downto 0));
end lab4sun;

architecture rtl of lab4sun is

 --Component from the Verilog file: vga_adapter.v

  component vga_adapter
    generic(RESOLUTION : string);
    port (resetn                                       : in  std_logic;
          clock                                        : in  std_logic;
          colour                                       : in  std_logic_vector(2 downto 0);
          x                                            : in  std_logic_vector(7 downto 0);
          y                                            : in  std_logic_vector(6 downto 0);
          plot                                         : in  std_logic;
          VGA_R, VGA_G, VGA_B                          : out std_logic_vector(9 downto 0);
          VGA_HS, VGA_VS, VGA_BLANK, VGA_SYNC, VGA_CLK : out std_logic);
  end component;

  signal x      : std_logic_vector(7 downto 0);
  signal y      : std_logic_vector(6 downto 0);
  signal colour : std_logic_vector(2 downto 0);
  signal plot   : std_logic;
  signal store_x : unsigned(7 downto 0) := "01010000"; -- 80
  signal store_y : unsigned(6 downto 0) := "0111100"; --60

begin

  -- includes the vga adapter, which should be in your project 

  vga_u0 : vga_adapter
    generic map(RESOLUTION => "160x120") 
    port map(resetn    => KEY(3),
             clock     => CLOCK_50,
             colour    => colour,
             x         => x,
             y         => y,
             plot      => plot,
             VGA_R     => VGA_R,
             VGA_G     => VGA_G,
             VGA_B     => VGA_B,
             VGA_HS    => VGA_HS,
             VGA_VS    => VGA_VS,
             VGA_BLANK => VGA_BLANK,
             VGA_SYNC  => VGA_SYNC,
             VGA_CLK   => VGA_CLK);


  -- rest of your code goes here, as well as possibly additional files
  ------------------------------------------------------------------------------
  
  
 process(CLOCK_50,Key(0))
 
 variable Horizontal : unsigned (7 downto 0) := "00000000"; -- X component, Unsigned for arithmatic
 variable Vertical : unsigned (6 downto 0) := "0000000"; -- Y component, Unsigned for arithmatic
 variable X_cord : unsigned(7 downto 0);
 variable Y_cord : unsigned (6 downto 0);
 variable Y_temp : unsigned(15 downto 0);
 variable x_mid : unsigned(7 downto 0);
 variable y_mid: unsigned (6 downto 0); 
 variable x_user : unsigned(7 downto 0);
 variable y_user: unsigned (6 downto 0);
 
 variable X_count : unsigned(7 downto 0);
 variable Y_count : unsigned (6 downto 0);
 
 variable dxu: unsigned(7 downto 0);
 variable dyu: unsigned(6 downto 0);
 
 variable dx: signed(8 downto 0);
 variable dy: signed(7 downto 0);
 variable ndy: signed(7 downto 0);
 variable sx: std_logic;
 variable sy: std_logic;
 variable err: signed(8 downto 0);
 variable e2: signed(9 downto 0);
type StateLines is ( Initialize, Draw, Hold, Reset);
variable Lines : StateLines := Hold;

 
 begin
 if(key(3) = '0') then
	plot <='0';
	Horizontal := "00000000";
	Vertical := "0000000";
	Lines := Reset;
	
 elsif(rising_edge(CLOCK_50)) then
 
	case Lines is
	
when Hold =>
--	ledg <= "00000011";
	store_x <= x_user;
	store_y <= y_user;
	if (Key(0) = '0') then
		Lines:= Initialize;
	else
		Lines:= Hold;
	end if;
	
 
 when initialize => 
--	ledg <= "00001100";
	x_mid := store_x; 
   y_mid := Store_y; 
	if( SW(17 downto 10) > "10011111") then
		X_user := "00000000";
	else
		X_user := unsigned(SW(17 downto 10));
--		ledr(17 downto 10) <= SW(17 downto 10);
	end if;
	
	if( SW(9 downto 3) > "1110111") then
		Y_user := "0000000";
	else
		Y_user := unsigned(SW(9 downto 3));
--		ledr(9 downto 3) <= SW(9 downto 3);
	end if;
	colour <= SW(2 downto 0);
	
	if (x_mid < X_user) then 
		dxu := (x_user - x_mid);
--		ledr(17 downto 9) <= std_logic_vector(dxu);
		sx := '1'; -- Give it a 1 since signed
	else 
		dxu := (x_mid - x_user);
--		ledr(17 downto 9) <= std_logic_vector(dxu);
		sx := '0';-- Give it neg 1 since it is signed

	end if;

	if (y_mid < Y_user) then 
		dyu := (y_user -y_mid);
--		ledr(8 downto 1) <= std_logic_vector(dyu);
		sy := '1'; 
	else
		dyu := (y_mid - y_user);
--		ledr(8 downto 1) <= std_logic_vector(dyu);
		sy := '0';
	end if;

--	dx := signed( not(dxu) + "1");
--	dy := signed( not(dyu) + "1");
	dx := signed( "0"&dxu);
	dy := signed( "0"&dyu);
	ndy := (not(dy) +"1");
--	ledr(17 downto 9) <= std_logic_vector(dx);
-- ledg(7 downto 0) <= std_logic_vector(dy);
	err := dx-dy; -- too small
--	ledr(8 downto 0) <= std_logic_vector(err);
	x_cord := x_mid;
	y_cord := y_mid;

	Lines := Draw;
	
when Draw =>
--	ledg <= "00110000";
   plot <= '1';
	x <= std_logic_vector(x_cord);
	y <= std_logic_vector(y_cord);

if ((x_user = X_cord) and (y_user = Y_cord)) then

	Lines := Hold;
--	ledg <= "10110000";
else

		e2 := err&"0"; -- to small?

		if (e2 > ndy) then 
			err := err - dy;
			if(sx = '1')then
				x_cord := x_cord + "1";
			else
				x_cord := x_cord - "1";
			end if;
		end if;

		if (e2 < dx) then
			err := err + dx; -- to small?
			if(sy = '1') then
				y_cord  := y_cord + "1";
			else
				y_cord  := y_cord - "1";
			end if;
		end if;
	Lines := Draw;
--	ledg <= "01110000";
end if;

when Reset =>
	plot <='1';
	colour <= "000";
	if( Horizontal <= "10011111") then -- Here we check to make sure we dont run off the screen
			
			x<= std_logic_vector(Horizontal); -- Send our Value to the x or horizontal position
			y <= std_logic_vector(Vertical);
			Horizontal := Horizontal + 1;
	else
		Horizontal := "00000000";
		if(Vertical <= "1110111" ) then -- Ensure we don't fall off bottom of screen
			Vertical := Vertical +1;
		else 
			x <= std_logic_vector(Horizontal); 
			y <= std_logic_vector(Vertical); 
			Horizontal := "00000000";
			Vertical := "0000000";
			Lines := Hold;
		end if;
				
	end if;
end case;
end if;		
end process;


end RTL;