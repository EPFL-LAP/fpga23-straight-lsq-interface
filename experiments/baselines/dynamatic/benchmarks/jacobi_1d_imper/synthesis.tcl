
set_project .
set_top_file jacobi_1d_imper.cpp
synthesize -use-lsq=false -simple-buffers=true -verbose
#cdfg
write_hdl

exit



