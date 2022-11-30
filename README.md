# Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits

This repository holds the source code, benchmarks and results of the work presented in the paper entitled "Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits".

## Prerequisites

In order to reproduce the main results, the following dependencies are needed:
1) Dynamatic's virutal machine (VM) which contains an installation of the Dynamatic tool with its dependencies through the following link https://drive.google.com/file/d/1OI24totIPdp-P_inkdH1-slyNPw-huRL/view. The following link contains the VM setup instructions https://dynamatic.epfl.ch/downloads/DynamaticVM_setup_instructions.pdf
2) Guorobi optimizer version 9.5.1 build v9.5.1rc2 (linux64). It is an MILP solver needed in Dynamatic's buffer placement through the following link https://www.gurobi.com/downloads/?campaignid=2027425882&adgroupid=77414946611&creative=601650357813&keyword=gurobi&matchtype=e&gclid=eaiaiqobchminsvdsbr3-givrgklch2j8wkaeaayasabegloxvd_bwe
3) Vivado 2019.2

It would be better to install both Vivado and the Gurobi optimizer inside Dynamatic's VM to have the whole environment in one place.

## Code Organization 

## Results Reproduction

To reproduce the results:
1) Download Dynamatic's virutal machine (VM)
2) Inside the VM, install the Gurobi optimizer.
3) Go through lines 7 to 11 in the compile_simulate_test.sh script and make sure that the intialization variables needed to run Gurobi are consistent with your installation path and license of Gurobi. 
4) Inside the VM, install Vivado 2019.2.
5) Clone this repository in home/Dynamatic/etc/dynamatic/
6) From inside the directory of the cloned repository, run the install_plugins.sh script using the following command "bash ./install_plugins.sh" to install the plugins of this paper into Dynamatic's infrastructure
7) From inside the directory of the cloned repository, run the script compile_simulate_test.sh using the following command "bash ./compile_simulate_test.sh" which runs our HLS tool to produce VHDL netlists out of the C++ inputs. The script runs the source code of the 8 benchmarks present in the benchmarks/ directory one after the other, then it simulates the generated designs. Reports of results are generated in simulation_reports/ directory. We generate three types of reports for each benchmark. [TODO: Explain the contents of each report]
8) From inside the directory of the cloned repository, run the script synth_place_route_test.sh which runs synthesis as well as place and route using Vivado 2019.2 on our generated circuits. The script synthesizes the generated circuits of the 8 benchmarks one after the other. You can check the summary of the timing and area results in synth_place_route_reports/ directory.

The results of the two baselines that we referenced in the paper can be found in the baselines/ directory.

