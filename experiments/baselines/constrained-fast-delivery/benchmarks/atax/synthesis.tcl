
set_project .
set_top_file atax.cpp
synthesize -use-lsq=true -simple-buffers=true -verbose
#cdfg
write_hdl

exit



