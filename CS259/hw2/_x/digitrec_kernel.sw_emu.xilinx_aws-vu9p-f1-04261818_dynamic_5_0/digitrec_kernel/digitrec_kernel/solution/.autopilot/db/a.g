#!/bin/sh
lli=${LLVMINTERP-lli}
exec $lli \
    /home/centos/src/project_data/aws-fpga/SDAccel/examples/xilinx_2018.2/getting_started/cs-259-18f/hw2/_x/digitrec_kernel.sw_emu.xilinx_aws-vu9p-f1-04261818_dynamic_5_0/digitrec_kernel/digitrec_kernel/solution/.autopilot/db/a.g.bc ${1+"$@"}
