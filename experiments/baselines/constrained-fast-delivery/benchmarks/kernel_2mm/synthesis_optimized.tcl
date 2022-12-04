
set_project .
set_top_file kernel_2mm.cpp
synthesize -use-lsq=true -verbose
set_period 4
optimize
write_hdl

exit



