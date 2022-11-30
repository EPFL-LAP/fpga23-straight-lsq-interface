
set_project .
set_top_file triangular.cpp
synthesize -simple-buffers=true -verbose
set_period 4
optimize
write_hdl

exit



