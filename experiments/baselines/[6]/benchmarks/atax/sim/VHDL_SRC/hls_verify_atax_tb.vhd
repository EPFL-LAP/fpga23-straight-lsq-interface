library IEEE;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_textio.all;
use ieee.numeric_std.all;
use std.textio.all;

use work.sim_package.all;



entity atax_tb is

end entity atax_tb;

architecture behav of atax_tb is

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
	constant DATA_DEPTH_A : INTEGER := 400;
	constant INPUT_x : STRING := "../INPUT_VECTORS/input_x.dat";
	constant OUTPUT_x : STRING := "";
	constant DATA_WIDTH_x : INTEGER := 32;
	constant ADDR_WIDTH_x : INTEGER := 32;
	constant DATA_DEPTH_x : INTEGER := 20;
	constant INPUT_y : STRING := "../INPUT_VECTORS/input_y.dat";
	constant OUTPUT_y : STRING := "../VHDL_OUT/output_y.dat";
	constant DATA_WIDTH_y : INTEGER := 32;
	constant ADDR_WIDTH_y : INTEGER := 32;
	constant DATA_DEPTH_y : INTEGER := 20;
	constant INPUT_tmp : STRING := "../INPUT_VECTORS/input_tmp.dat";
	constant OUTPUT_tmp : STRING := "../VHDL_OUT/output_tmp.dat";
	constant DATA_WIDTH_tmp : INTEGER := 32;
	constant ADDR_WIDTH_tmp : INTEGER := 32;
	constant DATA_DEPTH_tmp : INTEGER := 20;

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

	signal x_mem_ce0 : std_logic;
	signal x_mem_we0 : std_logic;
	signal x_mem_din0 : std_logic_vector(DATA_WIDTH_x - 1 downto 0);
	signal x_mem_dout0 : std_logic_vector(DATA_WIDTH_x - 1 downto 0);
	signal x_mem_address0 : std_logic_vector(ADDR_WIDTH_x - 1 downto 0);

	signal x_mem_ce1 : std_logic;
	signal x_mem_we1 : std_logic;
	signal x_mem_din1 : std_logic_vector(DATA_WIDTH_x - 1 downto 0);
	signal x_mem_dout1 : std_logic_vector(DATA_WIDTH_x - 1 downto 0);
	signal x_mem_address1 : std_logic_vector(ADDR_WIDTH_x - 1 downto 0);

	signal y_mem_ce0 : std_logic;
	signal y_mem_we0 : std_logic;
	signal y_mem_din0 : std_logic_vector(DATA_WIDTH_y - 1 downto 0);
	signal y_mem_dout0 : std_logic_vector(DATA_WIDTH_y - 1 downto 0);
	signal y_mem_address0 : std_logic_vector(ADDR_WIDTH_y - 1 downto 0);

	signal y_mem_ce1 : std_logic;
	signal y_mem_we1 : std_logic;
	signal y_mem_din1 : std_logic_vector(DATA_WIDTH_y - 1 downto 0);
	signal y_mem_dout1 : std_logic_vector(DATA_WIDTH_y - 1 downto 0);
	signal y_mem_address1 : std_logic_vector(ADDR_WIDTH_y - 1 downto 0);

	signal tmp_mem_ce0 : std_logic;
	signal tmp_mem_we0 : std_logic;
	signal tmp_mem_din0 : std_logic_vector(DATA_WIDTH_tmp - 1 downto 0);
	signal tmp_mem_dout0 : std_logic_vector(DATA_WIDTH_tmp - 1 downto 0);
	signal tmp_mem_address0 : std_logic_vector(ADDR_WIDTH_tmp - 1 downto 0);

	signal tmp_mem_ce1 : std_logic;
	signal tmp_mem_we1 : std_logic;
	signal tmp_mem_din1 : std_logic_vector(DATA_WIDTH_tmp - 1 downto 0);
	signal tmp_mem_dout1 : std_logic_vector(DATA_WIDTH_tmp - 1 downto 0);
	signal tmp_mem_address1 : std_logic_vector(ADDR_WIDTH_tmp - 1 downto 0);


	signal tb_temp_idle : std_logic:= '1';
	shared variable transaction_idx : INTEGER := 0;

begin


duv: 	 entity work.atax
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
			x_address0 => x_mem_address0,
			x_ce0 => x_mem_ce0,
			x_we0 => x_mem_we0,
			x_din0 => x_mem_dout0,
			x_dout0 => x_mem_din0,
			x_address1 => x_mem_address1,
			x_ce1 => x_mem_ce1,
			x_we1 => x_mem_we1,
			x_din1 => x_mem_dout1,
			x_dout1 => x_mem_din1,
			y_address0 => y_mem_address0,
			y_ce0 => y_mem_ce0,
			y_we0 => y_mem_we0,
			y_din0 => y_mem_dout0,
			y_dout0 => y_mem_din0,
			y_address1 => y_mem_address1,
			y_ce1 => y_mem_ce1,
			y_we1 => y_mem_we1,
			y_din1 => y_mem_dout1,
			y_dout1 => y_mem_din1,
			tmp_address0 => tmp_mem_address0,
			tmp_ce0 => tmp_mem_ce0,
			tmp_we0 => tmp_mem_we0,
			tmp_din0 => tmp_mem_dout0,
			tmp_dout0 => tmp_mem_din0,
			tmp_address1 => tmp_mem_address1,
			tmp_ce1 => tmp_mem_ce1,
			tmp_we1 => tmp_mem_we1,
			tmp_din1 => tmp_mem_dout1,
			tmp_dout1 => tmp_mem_din1,
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

mem_inst_x:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_x,
		TV_OUT => OUTPUT_x,
		DEPTH => DATA_DEPTH_x,
		DATA_WIDTH => DATA_WIDTH_x,
		ADDR_WIDTH => ADDR_WIDTH_x
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => x_mem_ce0,
		we0 => x_mem_we0,
		address0 => x_mem_address0,
		mem_dout0 => x_mem_dout0,
		mem_din0 => x_mem_din0,
		ce1 => x_mem_ce1,
		we1 => x_mem_we1,
		address1 => x_mem_address1,
		mem_dout1 => x_mem_dout1,
		mem_din1 => x_mem_din1,
		done => tb_temp_idle
	);

mem_inst_y:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_y,
		TV_OUT => OUTPUT_y,
		DEPTH => DATA_DEPTH_y,
		DATA_WIDTH => DATA_WIDTH_y,
		ADDR_WIDTH => ADDR_WIDTH_y
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => y_mem_ce0,
		we0 => y_mem_we0,
		address0 => y_mem_address0,
		mem_dout0 => y_mem_dout0,
		mem_din0 => y_mem_din0,
		ce1 => y_mem_ce1,
		we1 => y_mem_we1,
		address1 => y_mem_address1,
		mem_dout1 => y_mem_dout1,
		mem_din1 => y_mem_din1,
		done => tb_temp_idle
	);

mem_inst_tmp:	 entity work.two_port_RAM 
	generic map(
		TV_IN => INPUT_tmp,
		TV_OUT => OUTPUT_tmp,
		DEPTH => DATA_DEPTH_tmp,
		DATA_WIDTH => DATA_WIDTH_tmp,
		ADDR_WIDTH => ADDR_WIDTH_tmp
	)
	port map(
		clk => tb_clk,
		rst => tb_rst,
		ce0 => tmp_mem_ce0,
		we0 => tmp_mem_we0,
		address0 => tmp_mem_address0,
		mem_dout0 => tmp_mem_dout0,
		mem_din0 => tmp_mem_din0,
		ce1 => tmp_mem_ce1,
		we1 => tmp_mem_we1,
		address1 => tmp_mem_address1,
		mem_dout1 => tmp_mem_dout1,
		mem_din1 => tmp_mem_din1,
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
write_output_transactor_y_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_y, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_y & " failed!!!" severity note;
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
	file_open(fstatus, fp, OUTPUT_y, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_y & " failed!!!" severity note;
		assert false report "ERROR: Simulation using HLS TB failed." severity failure;
	end if;
	write(token_line, string'("[[[/runtime]]]"));
	writeline(fp, token_line);
	file_close(fp);
	wait;
end process;
write_output_transactor_tmp_runtime_proc : process
	file fp             : TEXT;
	variable fstatus    : FILE_OPEN_STATUS;
	variable token_line : LINE;
	variable token      : STRING(1 to 1024);

begin
	file_open(fstatus, fp, OUTPUT_tmp, WRITE_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_tmp & " failed!!!" severity note;
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
	file_open(fstatus, fp, OUTPUT_tmp, APPEND_MODE);
	if (fstatus /= OPEN_OK) then
		assert false report "Open file " & OUTPUT_tmp & " failed!!!" severity note;
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

