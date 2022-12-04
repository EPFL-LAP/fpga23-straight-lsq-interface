vlib work
vmap work work
project new . simulation work modelsim.ini 0
project open simulation
project addfile ../VHDL_SRC/delay_buffer.vhd
project addfile ../VHDL_SRC/arithmetic_units.vhd
project addfile ../VHDL_SRC/getTanh_optimized.vhd
project addfile ../VHDL_SRC/sharing_components.vhd
project addfile ../VHDL_SRC/elastic_components.vhd
project addfile ../VHDL_SRC/mul_wrapper.vhd
project addfile ../VHDL_SRC/MemCont.vhd
project addfile ../VHDL_SRC/multipliers.vhd
project addfile ../VHDL_SRC/simpackage.vhd
project addfile ../VHDL_SRC/two_port_RAM.vhd
project addfile ../VHDL_SRC/hls_verify_getTanh_tb.vhd
project addfile ../VHDL_SRC/single_argument.vhd
project addfile ../VHDL_SRC/LSQ_A.v
project calculateorder
project compileall
eval vsim getTanh_tb
run -all
exit
