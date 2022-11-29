#!/bin/sh
#Authors: Andrea Guerrieri <andrea.guerrieri@epfl.ch> Ayatallah Elakhras <ayatallah.elakhras@epfl.ch>

#export TODAY_IS=`date +%F%H%M%S`

#report_file=regression_test_detailed_$TODAY_IS.rpt
#report=regression_test_$TODAY_IS.rpt

report=$2
report_file=$3
cycle_file=$4

#workingdir=`pwd`
workingdir="$(pwd)/simulation_reports/"


#Exits wrapper
#report () 
#{
#  printf "$(date)\t$1\t$2\n" >> log.csv
#}
#
#printf "Date\tOperation\tResult\n" >> log.csv

echo "" >> $workingdir/$report_file	
echo "" >> $workingdir/$report_file
echo "**************************************************" >> $workingdir/$report_file
date >> $workingdir/$report_file

testcase=$1

#echo "****Executing test case $testcase ****" >> $workingdir/$report_file
#Wo optimization
#sh simulation_scripts/execute.sh $testcase $workingdir/$report >> $workingdir/$report_file
#sh simulation_scripts/check.sh $testcase $workingdir/$report >> $workingdir/$report_file

echo "****Executing test case $testcase optimized ****" >> $workingdir/$report_file
#With optimization
sh simulation_scripts/execute.sh $testcase $workingdir/$report optimized >> $workingdir/$report_file
sh simulation_scripts/check.sh $testcase $workingdir/$report optimized >> $workingdir/$report_file

#################################################
#while IFS= read -r line
#do
#	echo $line >> $workingdir/$cycle_file 
#done < "$workingdir/$report_file"
#################################################

str=`grep -A 1 "Failure: NORMAL EXIT" "$workingdir/$report_file" | tail -n 1`

#substr="ns"
#prefix=${str%%$substr*}
#index=${#prefix}
#echo $index >> $workingdir/$cycle_file 

sim_time=`echo $str | cut -d' ' -f 3`
temp_cycle_time=`expr $sim_time - 30`
cycle_time=`expr $temp_cycle_time / 4`
echo "The total simulation time for the $testcase example is $sim_time ns" >> $workingdir/$cycle_file 
echo "The cycle count for the $testcase example is $cycle_time cycles" >> $workingdir/$cycle_file 



#report "Execution" $result

