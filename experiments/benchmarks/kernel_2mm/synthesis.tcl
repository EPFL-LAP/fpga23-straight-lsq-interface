
set_project .
set_top_file 2mm.cpp
synthesize -use-lsq=true -simple-buffers=true -verbose
#cdfg
write_hdl

exit



