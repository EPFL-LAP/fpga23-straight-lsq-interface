
set_project .
set_top_file jacobi_1d.cpp
set_period 4
synthesize -use-lsq=true -verbose
optimize
write_hdl

exit



