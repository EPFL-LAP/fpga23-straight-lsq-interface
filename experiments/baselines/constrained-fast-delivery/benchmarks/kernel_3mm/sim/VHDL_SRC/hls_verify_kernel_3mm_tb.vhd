library IEEE;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use ieee.numeric_std.all;
use std.textio.all;

use work.sim_package.all;



entity kernel_3mm_tb is

end entity kernel_3mm_tb;

architecture behav of kernel_3mm_tb is

	-- Constant declarations

	constant HALF_CLK_PERIOD : TIME := 2.00 ns;
	constant TRANSACTION_NUM : INTEGER := 1;
	constant INPUT_end : STRING := "";
	constant OUTPUT_end : STRING := "../VHDL_OUT/output_end.dat";
	constant DATA_WIDTH_end : INTEGER := 32;
	constant INPUT_A : STRING := "../INPUT_VECTORS/input_A.dat";
	constant OUTPUT_A : STRING := "";
	constant DATA_WIDTH_A : INTEGER := 32;
	constant ADDR_WIDTH_A : INTEGER := 32;
	constant DATA_DEPTH_A : INTEGER := 100;
	constant INPUT_B : STRING := "../INPUT_VECTORS/input_B.dat";
	constant OUTPUT_B : STRING := "";
	constant DATA_WIDTH_B : INTEGER := 32;
	constant ADDR_WIDTH_B : INTEGER := 32;
	constant DATA_DEPTH_B : INTEGER := 100;
	constant INPUT_C : STRING := "../INPUT_VECTORS/input_C.dat";
	constant OUTPUT_C : STRING := "";
	constant DATA_WIDTH_C : INTEGER := 32;
	constant ADDR_WIDTH_C : INTEGER := 32;
	constant DATA_DEPTH_C : INTEGER := 100;
	constant INPUT_D : STRING := "../INPUT_VECTORS/input_D.dat";
	constant OUTPUT_D : STRING := "";
	constant DATA_WIDTH_D : INTEGER := 32;
	constant ADDR_WIDTH_D : INTEGER := 32;
	constant DATA_DEPTH_D : INTEGER := 100;
	constant INPUT_E : STRING := "../INPUT_VECTORS/input_E.dat";
	constant OUTPUT_E : STRING := "../VHDL_OUT/output_E.dat";
	constant DATA_WIDTH_E : INTEGER := 32;
	constant ADDR_WIDTH_E : INTEGER := 32;
	constant DATA_DEPTH_E : INTEGER := 100;
	constant INPUT_F : STRING := "../INPUT_VECTORS/input_F.dat";
	constant OUTPUT_F : STRING := "../VHDL_OUT/output_F.dat";
	constant DATA_WIDTH_F : INTEGER := 32;
	constant ADDR_WIDTH_F : INTEGER := 32;
	constant DATA_DEPTH_F : INTEGER := 100;
	constant INPUT_G : STRING := "";
	constant OUTPUT_G : STRING := "../VHDL_OUT/output_G.dat";
	constant DATA_WIDTH_G : INTEGER := 32;
	constant ADDR_WIDTH_G : INTEGER := 32;
	constant DATA_DEPTH_G : INTEGER := 100;

	-- Signal declarations

	signal tb_clk : std_logic := '0';
	signal tb_rst : std_logic := '0';
	signal tb_start_valid : std_logic := '0';
	signal tb_start_ready : std_logic;
	signal tb_end_valid : std_logic;

	signal end_mem_ce0 : std_logic;
	signal end_mem_we0 : std_logic;
	signal end_mem_dout0 : std_logic_vector(DATA_WIDTH_end - 1 downto 0);
	signal end_mem_din0 : std_logic_vector(DATA_WIDTH_end - 1 downto 0);


	signal A_mem_ce0 : std_logic;
	signal A_mem_we0 : std_logic;
	signal A_mem_din0 : std_logic_vector(DATA_WIDTH_A - 1 downto 0);
	signal A_mem_dout0 : std_logic_vector(DATA_WIDTH_A - 1 downto 0);
	signal A_mem_address0 : std_logic_vector(ADDR_WIDTH_A - 1 downto 0);

	signal A_mem_ce1 : std_logic;
	signal A_mem_we1 : std_logic;
	signal A_mem_din1 : std_logic_vector(DATA_WIDTH_A - 1 downto 0);
	signal A_mem_dout1 : std_logic_vector(DATA_WIDTH_A - 1 downto 0);
	signal A_mem_address1 : std_logic_vector(ADDR_WIDTH_A - 1 downto 0);

	signal B_mem_ce0 : std_logic;
	signal B_mem_we0 : std_logic;
	signal B_mem_din0 : std_logic_vector(DATA_WIDTH_B - 1 downto 0);
	signal B_mem_dout0 : std_logic_vector(DATA_WIDTH_B - 1 downto 0);
	signal B_mem_address0 : std_logic_vector(ADDR_WIDTH_B - 1 downto 0);

	signal B_mem_ce1 : std_logic;
	signal B_mem_we1 : std_logic;
	signal B_mem_din1 : std_logic_vector(DATA_WIDTH_B - 1 downto 0);
	signal B_mem_dout1 : std_logic_vector(DATA_WIDTH_B - 1 downto 0);
	signal B_mem_address1 : std_logic_vector(ADDR_WIDTH_B - 1 downto 0);

	signal C_mem_ce0 : std_logic;
	signal C_mem_we0 : std_logic;
	signal C_mem_din0 : std_logic_vector(DATA_WIDTH_C - 1 downto 0);
	signal C_mem_dout0 : std_logic_vector(DATA_WIDTH_C - 1 downto 0);
	signal C_mem_address0 : std_logic_vector(ADDR_WIDTH_C - 1 downto 0);

	signal C_mem_ce1 : std_logic;
	signal C_mem_we1 : std_logic;
	signal C_mem_din1 : std_logic_vector(DATA_WIDTH_C - 1 downto 0);
	signal C_mem_dout1 : std_logic_vector(DATA_WIDTH_C - 1 downto 0);
	signal C_mem_address1 : std_logic_vector(ADDR_WIDTH_C - 1 downto 0);

	signal D_mem_ce0 : std_logic;
	signal D_mem_we0 : std_logic;
	signal D_mem_din0 : std_logic_vector(DATA_WIDTH_D - 1 downto 0);
	signal D_mem_dout0 : std_logic_vector(DATA_WIDTH_D - 1 downto 0);
	signal D_mem_address0 : std_logic_vector(ADDR_WIDTH_D - 1 downto 0);

	signal D_mem_ce1 : std_logic;
	signal D_mem_we1 : std_logic;
	signal D_mem_din1 : std_logic_vector(DATA_WIDTH_D - 1 downto 0);
	signal D_mem_dout1 : std_logic_vector(DATA_WIDTH_D - 1 downto 0);
	signal D_mem_address1 : std_logic_vector(ADDR_WIDTH_D - 1 downto 0);

	signal E_mem_ce0 : std_logic;
	signal E_mem_we0 : std_logic;
	signal E_mem_din0 : std_logic_vector(DATA_WIDTH_E - 1 downto 0);
	signal E_mem_dout0 : std_logic_vector(DATA_WIDTH_E - 1 downto 0);
	signal E_mem_address0 : std_logic_vector(ADDR_WIDTH_E - 1 downto 0);

	signal E_mem_ce1 : std_logic;
	signal E_mem_we1 : std_logic;
	signal E_mem_din1 : std_logic_vector(DATA_WIDTH_E - 1 downto 0);
	signal E_mem_dout1 : std_logic_vector(DATA_WIDTH_E - 1 downto 0);
	signal E_mem_address1 : std_logic_vector(ADDR_WIDTH_E - 1 downto 0);

	signal F_mem_ce0 : std_logic;
	signal F_mem_we0 : std_logic;
	signal F_mem_din0 : std_logic_vector(DATA_WIDTH_F - 1 downto 0);
	signal F_mem_dout0 : std_logic_vector(DATA_WIDTH_F - 1 downto 0);
	signal F_mem_address0 : std_logic_vector(ADDR_WIDTH_F - 1 downto 0);

	signal F_mem_ce1 : std_logic;
	signal F_mem_we1 : std_logic;
	signal F_mem_din1 : std_logic_vector(DATA_WIDTH_F - 1 downto 0);
	signal F_mem_dout1 : std_logic_vector(DATA_WIDTH_F - 1 downto 0);
	signal F_mem_address1 : std_logic_vector(ADDR_WIDTH_F - 1 downto 0);

	signal G_mem_ce0 : std_logic;
	signal G_mem_we0 : std_logic;
	signal G_mem_din0 : std_logic_vector(DATA_WIDTH_G - 1 downto 0);
	signal G_mem_dout0 : std_logic_vector(DATA_WIDTH_G - 1 downto 0);
	signal G_mem_address0 : std_logic_vector(ADDR_WIDTH_G - 1 downto 0);

	signal G_mem_ce1 : std_logic;
	signal G_mem_we1 : std_logic;
	signal G_mem_din1 : std_logic_vector(DATA_WIDTH_G - 1 downto 0);
	signal G_mem_dout1 : std_logic_vector(DATA_WIDTH_G - 1 downto 0);
	signal G_mem_address1 : std_logic_vector(ADDR_WIDTH_G - 1 downto 0);


	signal tb_temp_idle : std_logic:= '1';
	shared variable transaction_idx : INTEGER := 0;

begin


duv: 	 entity work.kernel_3mm
		port map (
			clk => tb_clk,
			rst => tb_rst,
			end_out => end_mem_din0,
			end_valid => tb_end_valid,
			end_ready => '1',
			A_address0 => A_mem_address0,
			A_ce0 => A_mem_ce0,
			A_we0 => A_mem_we0,
			A_din0 => A_mem_dout0,
			A_dout0 => A_mem_din0,
			A_address1 => A_mem_address1,
			A_ce1 => A_mem_ce1,
			A_we1 => A_mem_we1,
			A_din1 => A_mem_dout1,
			A_dout1 => A_mem_din1,
			B_address0 => B_mem_address0,
			B_ce0 => B_mem_ce0,
			B_we0 => B_mem_we0,
			B_din0 => B_mem_dout0,
			B_dout0 => B_mem_din0,
			B_address1 => B_mem_address1,
			B_ce1 => B_mem_ce1,
			B_we1 => B_mem_we1,
			B_din1 => B_mem_dout1,
			B_dout1 => B_mem_din1,
			C_address0 => C_mem_address0,
			C_ce0 => C_mem_ce0,
			C_we0 => C_mem_we0,
			C_din0 => C_mem_dout0,
			C_dout0 => C_mem_din0,
			C_address1 => C_mem_address1,
			C_ce1 => C_mem_ce1,
			C_we1 => C_mem_we1,
			C_din1 => C_mem_dout1,
			C_dout1 => C_mem_din1,
			D_address0 => D_mem_address0,
			D_ce0 => D_mem_ce0,
			D_we0 => D_mem_we0,
			D_din0 => D_mem_dout0,
			D_dout0 => D_mem_din0,
			D_address1 => D_mem_address1,
			D_ce1 => D_mem_ce1,
			D_we1 => D_mem_we1,
			D_din1 => D_mem_dout1,
			D_dout1 => D_mem_din1,
			E_address0 => E_mem_address0,
			E_ce0 => E_mem_ce0,
			E_we0 => E_mem_we0,
			E_din0 => E_mem_dout0,
			E_dout0 => E_mem_din0,
			E_address1 => E_mem_address1,
			E_ce1 => E_mem_ce1,
			E_we1 => E_mem_we1,
			E_din1 => E_mem_dout1,
			E_dout1 => E_mem_din1,
			F_address0 => F_mem_address0,
			F_ce0 => F_mem_ce0,
			F_we0 => F_mem_we0,
			F_din0 => F_mem_dout0,
			F_dout0 => F_mem_din0,
			F_address1 => F_mem_address1,
			F_ce1 => F_mem_ce1,
			F_we1 => F_mem_we1,
			F_din1 => F_mem_dout1,
			F_dout1 => F_mem_din1,
			G_address0 => G_mem_address0,
			G_ce0 => G_mem_ce0,
			G_we0 => G_mem_we0,
			G_din0 => G_mem_dout0,
			G_dout0 => G_mem_din0,
			G_address1 => G_mem_address1,
			G_ce1 => G_mem_ce1,
			G_we1 => G_mem_we1,
			G_din1 => G_mem_dout1,
			G_dout1 => G_mem_din1,
			start_in => (others => '0'),
			start_ready => tb_start_ready,
			start_valid => tb_start_valid
		);


arg_inst_end:	 entity work.single_argument
	generic map(
		TV_IN => INPUT_end,
		TV_OUT => OUTPUT_end,
		DATA_WIDTH => DATA_WIDTH_end
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => '1',
		we0 => tb_end_valid,
		mem_dout0 => end_mem_dout0,
		mem_din0 => end_mem_din0,
		done => tb_temp_idle
	);

mem_inst_A:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_A,
		TV_OUT => OUTPUT_A,
		DEPTH => DATA_DEPTH_A,
		DATA_WIDTH => DATA_WIDTH_A,
		ADDR_WIDTH => ADDR_WIDTH_A
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => A_mem_ce0,
		we0 => A_mem_we0,
		address0 => A_mem_address0,
		mem_dout0 => A_mem_dout0,
		mem_din0 => A_mem_din0,
		ce1 => A_mem_ce1,
		we1 => A_mem_we1,
		address1 => A_mem_address1,
		mem_dout1 => A_mem_dout1,
		mem_din1 => A_mem_din1,
		done => tb_temp_idle
	);

mem_inst_B:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_B,
		TV_OUT => OUTPUT_B,
		DEPTH => DATA_DEPTH_B,
		DATA_WIDTH => DATA_WIDTH_B,
		ADDR_WIDTH => ADDR_WIDTH_B
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => B_mem_ce0,
		we0 => B_mem_we0,
		address0 => B_mem_address0,
		mem_dout0 => B_mem_dout0,
		mem_din0 => B_mem_din0,
		ce1 => B_mem_ce1,
		we1 => B_mem_we1,
		address1 => B_mem_address1,
		mem_dout1 => B_mem_dout1,
		mem_din1 => B_mem_din1,
		done => tb_temp_idle
	);

mem_inst_C:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_C,
		TV_OUT => OUTPUT_C,
		DEPTH => DATA_DEPTH_C,
		DATA_WIDTH => DATA_WIDTH_C,
		ADDR_WIDTH => ADDR_WIDTH_C
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => C_mem_ce0,
		we0 => C_mem_we0,
		address0 => C_mem_address0,
		mem_dout0 => C_mem_dout0,
		mem_din0 => C_mem_din0,
		ce1 => C_mem_ce1,
		we1 => C_mem_we1,
		address1 => C_mem_address1,
		mem_dout1 => C_mem_dout1,
		mem_din1 => C_mem_din1,
		done => tb_temp_idle
	);

mem_inst_D:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_D,
		TV_OUT => OUTPUT_D,
		DEPTH => DATA_DEPTH_D,
		DATA_WIDTH => DATA_WIDTH_D,
		ADDR_WIDTH => ADDR_WIDTH_D
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => D_mem_ce0,
		we0 => D_mem_we0,
		address0 => D_mem_address0,
		mem_dout0 => D_mem_dout0,
		mem_din0 => D_mem_din0,
		ce1 => D_mem_ce1,
		we1 => D_mem_we1,
		address1 => D_mem_address1,
		mem_dout1 => D_mem_dout1,
		mem_din1 => D_mem_din1,
		done => tb_temp_idle
	);

mem_inst_E:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_E,
		TV_OUT => OUTPUT_E,
		DEPTH => DATA_DEPTH_E,
		DATA_WIDTH => DATA_WIDTH_E,
		ADDR_WIDTH => ADDR_WIDTH_E
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => E_mem_ce0,
		we0 => E_mem_we0,
		address0 => E_mem_address0,
		mem_dout0 => E_mem_dout0,
		mem_din0 => E_mem_din0,
		ce1 => E_mem_ce1,
		we1 => E_mem_we1,
		address1 => E_mem_address1,
		mem_dout1 => E_mem_dout1,
		mem_din1 => E_mem_din1,
		done => tb_temp_idle
	);

mem_inst_F:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_F,
		TV_OUT => OUTPUT_F,
		DEPTH => DATA_DEPTH_F,
		DATA_WIDTH => DATA_WIDTH_F,
		ADDR_WIDTH => ADDR_WIDTH_F
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => F_mem_ce0,
		we0 => F_mem_we0,
		address0 => F_mem_address0,
		mem_dout0 => F_mem_dout0,
		mem_din0 => F_mem_din0,
		ce1 => F_mem_ce1,
		we1 => F_mem_we1,
		address1 => F_mem_address1,
		mem_dout1 => F_mem_dout1,
		mem_din1 => F_mem_din1,
		done => tb_temp_idle
	);

mem_inst_G:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_G,
		TV_OUT => OUTPUT_G,
		DEPTH => DATA_DEPTH_G,
		DATA_WIDTH => DATA_WIDTH_G,
		ADDR_WIDTH => ADDR_WIDTH_G
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => G_mem_ce0,
		we0 => G_mem_we0,
		address0 => G_mem_address0,
		mem_dout0 => G_mem_dout0,
		mem_din0 => G_mem_din0,
		ce1 => G_mem_ce1,
		we1 => G_mem_we1,
		address1 => G_mem_address1,
		mem_dout1 => G_mem_dout1,
		mem_din1 => G_mem_din1,
		done => tb_temp_idle
	);



----------------------------------------------------------------------------
-- Write "[[[runtime]]]" and "[[[/runtime]]]" for output transactor
write_output_transactor_end_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_end, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_end & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	while transaction_idx /= TRANSACTION_NUM loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	file_open(fstatus, fp, OUTPUT_end, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_end & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_E_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_E, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_E & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	while transaction_idx /= TRANSACTION_NUM loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	file_open(fstatus, fp, OUTPUT_E, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_E & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_F_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_F, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_F & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	while transaction_idx /= TRANSACTION_NUM loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	file_open(fstatus, fp, OUTPUT_F, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_F & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_G_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_G, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_G & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	while transaction_idx /= TRANSACTION_NUM loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	file_open(fstatus, fp, OUTPUT_G, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_G & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
----------------------------------------------------------------------------



----------------------------------------------------------------------------
generate_sim_done_proc : process
begin
	while (transaction_idx /= TRANSACTION_NUM) loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	wait until tb_clk'event and tb_clk = '1';
	assert false report "simulation done!" severity note;
	assert false report "NORMAL EXIT (note: failure is to force the simulator to stop)" severity failure;
	wait;
end process;

----------------------------------------------------------------------------
gen_clock_proc : process
begin
	tb_clk <= '0';
	while (true) loop
		wait for HALF_CLK_PERIOD;
		tb_clk <= not tb_clk;
	end loop;
	wait;
end process;

----------------------------------------------------------------------------
gen_reset_proc : process
begin
	tb_rst <= '1';
	wait for 10 ns;
	tb_rst <= '0';
	wait;
end process;

----------------------------------------------------------------------------
generate_idle_signal: process(tb_clk,tb_rst)
begin
   if (tb_rst = '1') then
       tb_temp_idle <= '1';
   elsif rising_edge(tb_clk) then
       tb_temp_idle <= tb_temp_idle;
       if (tb_start_valid = '1') then
           tb_temp_idle <= '0';
       elsif(tb_end_valid = '1') then
           tb_temp_idle <= '1';
       end if;
   end if;
end process generate_idle_signal;

----------------------------------------------------------------------------
generate_start_signal : process(tb_clk, tb_rst)
begin
   if (tb_rst = '1') then
       tb_start_valid <= '0';
   elsif rising_edge(tb_clk) then
       if (tb_temp_idle = '1' and tb_start_ready = '1' and tb_start_valid = '0') then
           tb_start_valid <= '1';
       else
           tb_start_valid <= '0';
       end if;
   end if;
end process generate_start_signal;

----------------------------------------------------------------------------
transaction_increment : process
begin
	wait until tb_rst = '0';
	while (tb_temp_idle /= '1') loop
		wait until tb_clk'event and tb_clk = '1';
	end loop;
	wait until tb_temp_idle = '0';

	while (true) loop
		while (tb_temp_idle /= '1') loop
			wait until tb_clk'event and tb_clk = '1';
		end loop;
		transaction_idx := transaction_idx + 1;
		wait until tb_temp_idle = '0';
	end loop;
end process;

----------------------------------------------------------------------------


end architecture behav;

