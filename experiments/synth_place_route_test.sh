#!/bin/sh
#Author: Ayatallah Elakhras <ayatallah.elakhras@epfl.ch>

workingdir="$(pwd)/synth_place_route_reports/"
file=filelist.lst
i=0
myArray=(getTanh 2mm 3mm covariance jacobi_1d atax triangular histogram)

timing_rpt=timing_post_pr.rpt
util_rpt=utilization_post_pr.rpt
sim_report_dir=simulation_reports/
clk_period=4

# while IFS= read -r line
# do
#     # first create the synth directory by copying the needed scripts
#     cp -r synth_place_route_scripts/${myArray[$i]}/synth ${line}/
#     cd ${line}/synth/
#     /softs/xilinx/Vivado/2019.2/bin/vivado -mode batch -source synthesize_4.tcl
#     cd ../../../
#     ((i=i+1))
# done < "$file"

# generate the summary reports
# temporarily having this loop structure
while IFS= read -r line
do
    # first create the synth directory by copying the needed scripts
    cp -r synth_place_route_scripts/${myArray[$i]}/synth ${line}/
    cd ${line}/synth/
    /softs/xilinx/Vivado/2019.2/bin/vivado -mode batch -source synthesize_4.tcl
    cd ../../../
    # generate summary reports
    synth_summary_rpt=${myArray[$i]}_synth_p_r_summary.rpt
    str=`grep -A 2 "Timing Report" "${line}/synth/${timing_rpt}"`
    slack_with_unit=`echo $str | cut -d' ' -f 6`
    slack=`echo $slack_with_unit | cut -d'n' -f 1`
    actual_cp=`echo "$clk_period - $slack" | bc`
    echo "**************************************************" >> $workingdir/$synth_summary_rpt
    echo "Timing Summary for the ${line} example" >> $workingdir/$synth_summary_rpt
    echo "The clk period constraint applied during synthesis is 4ns" >> $workingdir/$synth_summary_rpt
    echo "The slack is ${slack}ns" >> $workingdir/$synth_summary_rpt
    echo "The actual clk period (CP) is ${actual_cp}ns" >> $workingdir/$synth_summary_rpt

    #echo " " >> $workingdir/$synth_summary_rpt
    cycle_count_str=`grep "cycles" "${sim_report_dir}/${myArray[$i]}_cycles_count.rpt"`
    cycle_count=`echo $cycle_count_str | tr -dc '0-9'`
    echo "The cycles count from simulation is ${cycle_count}" >> $workingdir/$synth_summary_rpt
    exec_time=`echo "$cycle_count * $actual_cp" | bc`
    echo "The total execution time is ${exec_time}ns" >> $workingdir/$synth_summary_rpt
    echo " " >> $workingdir/$synth_summary_rpt

    echo "**************************************************" >> $workingdir/$synth_summary_rpt
    echo "Area Summary for the ${line} example" >> $workingdir/$synth_summary_rpt

    luts_str=`grep "Slice LUTs" "${line}/synth/${util_rpt}"`
    luts=`echo $luts_str | cut -d'|' -f 3`
    echo "The LUTs count is ${luts}" >> $workingdir/$synth_summary_rpt

    ffs_str=`grep -m 1 "Slice Registers" "${line}/synth/${util_rpt}"`
    ffs=`echo $ffs_str | cut -d'|' -f 3`
    echo "The FFs count is ${ffs}" >> $workingdir/$synth_summary_rpt

    dsps_str=`grep -m 1 "DSPs" "${line}/synth/${util_rpt}"`
    dsps=`echo $dsps_str | cut -d'|' -f 3`
    echo "The DSPs count is ${dsps}" >> $workingdir/$synth_summary_rpt

    ((i=i+1))
done < "$file"
