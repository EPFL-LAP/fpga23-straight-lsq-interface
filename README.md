# Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits

This repository holds the source code, benchmarks and results of the work presented in the paper entitled "Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits", which proposes a methodology for interfacing dataflow circuits with Load-Store Queues (LSQs) while exploiting levels of parallelism that were not possible to achieve in prior work.

## Prerequisites

In order to reproduce our results, the following dependencies are needed:
1) Dynamatic's virutal machine (VM) which contains an installation of the Dynamatic tool with its dependencies through the following link https://drive.google.com/file/d/1OI24totIPdp-P_inkdH1-slyNPw-huRL/view. The following link contains the VM setup instructions https://dynamatic.epfl.ch/downloads/DynamaticVM_setup_instructions.pdf
Ideally, the rest of the dependencies should be installed inside Dynamatic's VM to have the entire working environment in one place.
2) Gurobi optimizer version 9.5.1 build v9.5.1rc2 (linux64) through the following link https://www.gurobi.com/downloads/?campaignid=2027425882&adgroupid=77414946611&creative=601650357813&keyword=gurobi&matchtype=e&gclid=eaiaiqobchminsvdsbr3-givrgklch2j8wkaeaayasabegloxvd_bwe Gurobi is an MILP solver required in Dynamatic's buffer placement. 
3) Vivado 2019.2

## Repository Organization 

This repository is composed of two main directories:
1) plugins/ holds the implementation of the methodology proposed in the paper as a set of plugins that should be installed into the Dynamatic tool through the provided install_plugins.sh script.

2) experiments/ holds the (i) C++ source files of the benchmarks used for generating the results reported in the paper, (ii) hls_verifier that verifies the correctness of the generated circuits, (iii) scripts that automate the process of running the HLS tool, simulating, synthesizing the generated designs and reporting the results.


## Results Reproduction

### Clone this repository

Inside Dynamatic's VM, execute the following commands from a terminal.
`cd Dynamatic/etc/dynamatic/
git clone git@github.com:EPFL-LAP/fpga23-straight-lsq-interface.git`

### Ensure that tools installations comply with our scripts

Our scripts use the installation paths of the Gurobi optimizer and Vivado. Make sure to do the following checks and do any necessary changes in the corresponding scripts to make them compatible with your installation paths.

1) Go through lines 7 to 11 in the experiments/compile_simulate_test.sh script and make sure that the intialization variables needed to run Gurobi are consistent with your installation path and license of Gurobi. 
2) Make sure that line 20 in the experiments/synth_place_route_test.sh script is consistent with the path where Vivado is installed. For instance, if your Vivado installation is in "opt", the command should be modified to "/opt/xilinx/Vivado/2019.2/bin/vivado".

### Install our plugins into Dynamatic

To install our plugins into Dynamatic's infrastructure, run the following commands from a terminal. 
`cd Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/
chmod +x install_plugins.sh
bash ./install_plugins.sh`

### Run the HLS flow and simulate the designs

To generate circuits out of the C++ source files of our benchmarks, run the following commands from a terminal. 
`cd Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments
chmod +x compile_simulate_test.sh
bash ./compile_simulate_test.sh`

The previous commands do the following for all of the benchmarks one after the other:
1) Run our HLS flow to generate VHDL netlists
2) Simulate the generated designs using Modelsim 10.5c to report the cycles count
3) Runs the HLS verifier to verify the correctness of our designs.

The outcome of this step is the generation of the following reports for each benchmark in the Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments/simulation_reports/ directory, where benchmarkName is replaced with the corresponding benchmark name.
1) benchmarkName_detailed_sim.rpt
2) benchmarkName_verification.rpt
3) benchmarkName_cycles_count.rpt

If you would like to check the generated designs (HDL netlists), go to the Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments/benchmarks/benchmarkName/hdl directory, where benchmarkName should be the name of the benchmark of your choice.

If you would like to simulate the generated desgins using Modelsim, go to the Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments/benchmarks/benchmarkName/sim/HLS_VERIFY directory where you will find the Modelsim project there.


### Synthesize and place and route

To synthesize, place and route the generated designs using Vivado, run the following commands from a terminal. 

`cd Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments
chmod +x synth_place_route_test.sh
bash ./synth_place_route_test.sh`

The outcome of the above step is the generation of a report with a summary of the timing and area results for each benchmark in the Dynamatic/etc/dynamatic/fpga23-straight-lsq-interface/experiments/synth_place_route_reports/ directory. 

### Check baselines results

The results of the two baselines that we referenced in the paper can be found in the baselines/ directory.

## Contact

For any questions, please contact Ayatallah Elakhras (ayatallah.elakhras@epfl.ch).