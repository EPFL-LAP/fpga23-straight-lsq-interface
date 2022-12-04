library IEEE;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use ieee.numeric_std.all;
use std.textio.all;

use work.sim_package.all;



entity jacobi_1d_tb is

end entity jacobi_1d_tb;

architecture behav of jacobi_1d_tb is

	-- Constant declarations

	constant HALF_CLK_PERIOD : TIME := 2.00 ns;
	constant TRANSACTION_NUM : INTEGER := 1;
	constant INPUT_end : STRING := "";
	constant OUTPUT_end : STRING := "../VHDL_OUT/output_end.dat";
	constant DATA_WIDTH_end : INTEGER := 32;
	constant INPUT_A_1 : STRING := "../INPUT_VECTORS/input_A_1.dat";
	constant OUTPUT_A_1 : STRING := "../VHDL_OUT/output_A_1.dat";
	constant DATA_WIDTH_A_1 : INTEGER := 32;
	constant ADDR_WIDTH_A_1 : INTEGER := 32;
	constant DATA_DEPTH_A_1 : INTEGER := 100;
	constant INPUT_A_2 : STRING := "../INPUT_VECTORS/input_A_2.dat";
	constant OUTPUT_A_2 : STRING := "../VHDL_OUT/output_A_2.dat";
	constant DATA_WIDTH_A_2 : INTEGER := 32;
	constant ADDR_WIDTH_A_2 : INTEGER := 32;
	constant DATA_DEPTH_A_2 : INTEGER := 100;
	constant INPUT_B : STRING := "../INPUT_VECTORS/input_B.dat";
	constant OUTPUT_B : STRING := "../VHDL_OUT/output_B.dat";
	constant DATA_WIDTH_B : INTEGER := 32;
	constant ADDR_WIDTH_B : INTEGER := 32;
	constant DATA_DEPTH_B : INTEGER := 100;

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


	signal A_1_mem_ce0 : std_logic;
	signal A_1_mem_we0 : std_logic;
	signal A_1_mem_din0 : std_logic_vector(DATA_WIDTH_A_1 - 1 downto 0);
	signal A_1_mem_dout0 : std_logic_vector(DATA_WIDTH_A_1 - 1 downto 0);
	signal A_1_mem_address0 : std_logic_vector(ADDR_WIDTH_A_1 - 1 downto 0);

	signal A_1_mem_ce1 : std_logic;
	signal A_1_mem_we1 : std_logic;
	signal A_1_mem_din1 : std_logic_vector(DATA_WIDTH_A_1 - 1 downto 0);
	signal A_1_mem_dout1 : std_logic_vector(DATA_WIDTH_A_1 - 1 downto 0);
	signal A_1_mem_address1 : std_logic_vector(ADDR_WIDTH_A_1 - 1 downto 0);

	signal A_2_mem_ce0 : std_logic;
	signal A_2_mem_we0 : std_logic;
	signal A_2_mem_din0 : std_logic_vector(DATA_WIDTH_A_2 - 1 downto 0);
	signal A_2_mem_dout0 : std_logic_vector(DATA_WIDTH_A_2 - 1 downto 0);
	signal A_2_mem_address0 : std_logic_vector(ADDR_WIDTH_A_2 - 1 downto 0);

	signal A_2_mem_ce1 : std_logic;
	signal A_2_mem_we1 : std_logic;
	signal A_2_mem_din1 : std_logic_vector(DATA_WIDTH_A_2 - 1 downto 0);
	signal A_2_mem_dout1 : std_logic_vector(DATA_WIDTH_A_2 - 1 downto 0);
	signal A_2_mem_address1 : std_logic_vector(ADDR_WIDTH_A_2 - 1 downto 0);

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


	signal tb_temp_idle : std_logic:= '1';
	shared variable transaction_idx : INTEGER := 0;

begin


duv: 	 entity work.jacobi_1d
		port map (
			clk => tb_clk,
			rst => tb_rst,
			end_out => end_mem_din0,
			end_valid => tb_end_valid,
			end_ready => '1',
			A_1_address0 => A_1_mem_address0,
			A_1_ce0 => A_1_mem_ce0,
			A_1_we0 => A_1_mem_we0,
			A_1_din0 => A_1_mem_dout0,
			A_1_dout0 => A_1_mem_din0,
			A_1_address1 => A_1_mem_address1,
			A_1_ce1 => A_1_mem_ce1,
			A_1_we1 => A_1_mem_we1,
			A_1_din1 => A_1_mem_dout1,
			A_1_dout1 => A_1_mem_din1,
			A_2_address0 => A_2_mem_address0,
			A_2_ce0 => A_2_mem_ce0,
			A_2_we0 => A_2_mem_we0,
			A_2_din0 => A_2_mem_dout0,
			A_2_dout0 => A_2_mem_din0,
			A_2_address1 => A_2_mem_address1,
			A_2_ce1 => A_2_mem_ce1,
			A_2_we1 => A_2_mem_we1,
			A_2_din1 => A_2_mem_dout1,
			A_2_dout1 => A_2_mem_din1,
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

mem_inst_A_1:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_A_1,
		TV_OUT => OUTPUT_A_1,
		DEPTH => DATA_DEPTH_A_1,
		DATA_WIDTH => DATA_WIDTH_A_1,
		ADDR_WIDTH => ADDR_WIDTH_A_1
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => A_1_mem_ce0,
		we0 => A_1_mem_we0,
		address0 => A_1_mem_address0,
		mem_dout0 => A_1_mem_dout0,
		mem_din0 => A_1_mem_din0,
		ce1 => A_1_mem_ce1,
		we1 => A_1_mem_we1,
		address1 => A_1_mem_address1,
		mem_dout1 => A_1_mem_dout1,
		mem_din1 => A_1_mem_din1,
		done => tb_temp_idle
	);

mem_inst_A_2:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_A_2,
		TV_OUT => OUTPUT_A_2,
		DEPTH => DATA_DEPTH_A_2,
		DATA_WIDTH => DATA_WIDTH_A_2,
		ADDR_WIDTH => ADDR_WIDTH_A_2
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => A_2_mem_ce0,
		we0 => A_2_mem_we0,
		address0 => A_2_mem_address0,
		mem_dout0 => A_2_mem_dout0,
		mem_din0 => A_2_mem_din0,
		ce1 => A_2_mem_ce1,
		we1 => A_2_mem_we1,
		address1 => A_2_mem_address1,
		mem_dout1 => A_2_mem_dout1,
		mem_din1 => A_2_mem_din1,
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
write_output_transactor_A_1_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_A_1, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_A_1 & " failed!!!" severity note;
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
	file_open(fstatus, fp, OUTPUT_A_1, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_A_1 & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_A_2_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_A_2, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_A_2 & " failed!!!" severity note;
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
	file_open(fstatus, fp, OUTPUT_A_2, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_A_2 & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_B_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_B, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_B & " failed!!!" severity note;
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
	file_open(fstatus, fp, OUTPUT_B, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_B & " failed!!!" severity note;
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

