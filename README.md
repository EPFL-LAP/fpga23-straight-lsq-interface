# Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits

This repository holds the source code, benchmarks and results of the work presented in the paper entitled "Straight to the Queue: Fast Load-Store Queue Allocation in Dataflow Circuits".

## Prerequisites

In order to reproduce the main results, the following dependencies are needed:
1) Dynamatic's virutal machine (VM) which contains an installation of the Dynamatic tool with its dependencies through the following link https://drive.google.com/file/d/1OI24totIPdp-P_inkdH1-slyNPw-huRL/view. The following link contains the VM setup instructions https://dynamatic.epfl.ch/downloads/DynamaticVM_setup_instructions.pdf
2) Guorobi which is an MILP solver needed in Dynamatic's buffer placement through the following link https://www.gurobi.com/downloads/?campaignid=2027425882&adgroupid=77414946611&creative=601650357813&keyword=gurobi&matchtype=e&gclid=eaiaiqobchminsvdsbr3-givrgklch2j8wkaeaayasabegloxvd_bwe
3) Vivado 2019.2

## Code Organization and Result Reproduction

This repository is organized as follows:
1) elastic-circuits/
2) Buffers/
3) components/
4) dot2vhdl/

To reproduce results:
1) Download Dynamatic's virutal machine (VM)
2) Inside the VM, install Guorobi
3) Clone this repository [SPECIFY A LOCATION?]
4) Run the script [NAME] in [WHICH DIRECTORY] to install the plugins of this paper into Dynamatic's infrastructure
5) Run the script [NAME] in [WHICH DIRECTORY] to generate the results. The script runs our 8 benchmarks one after the other on our tool to generate circuits then it simulates the generated designs and prints the cycle count in [FILENAME]. You can check the simulation results generated by Modelsim in [DIRECTORY] [GIVE AN ESTIMATE OF HOW LONG THIS STEP IS]
6) Run the script [NAME] in [WHICH DIRECTORY] to synthesize our generated circuits using Vivado. The script synthesizes the generated circuits one after the other. You can check the clock period of each benchmark in [FILENAME] and the resources in [FILENAME].

To check the results of our two baselines:
1) The results of the first Dynamatic baseline [13] are in [WHICH DIRECTORY]
2) The results of the second Dynamatic baseline [6] are in [WHICH DIRECTORY]

