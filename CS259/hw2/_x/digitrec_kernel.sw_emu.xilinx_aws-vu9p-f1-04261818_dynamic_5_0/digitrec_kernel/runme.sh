#!/bin/sh

# 
# xocc(TM)
# runme.sh: a xocc-generated Runs Script for UNIX
# Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
# 

if [ -z "$PATH" ]; then
  PATH=/opt/Xilinx/SDK/2018.2.op2258646/bin:/opt/Xilinx/SDx/2018.2.op2258646/bin:/opt/Xilinx/Vivado/2018.2.op2258646/bin
else
  PATH=/opt/Xilinx/SDK/2018.2.op2258646/bin:/opt/Xilinx/SDx/2018.2.op2258646/bin:/opt/Xilinx/Vivado/2018.2.op2258646/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=
else
  LD_LIBRARY_PATH=:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='/home/centos/src/project_data/aws-fpga/SDAccel/examples/xilinx_2018.2/getting_started/cs-259-18f/hw2/_x/digitrec_kernel.sw_emu.xilinx_aws-vu9p-f1-04261818_dynamic_5_0/digitrec_kernel'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

EAStep vivado_hls -f digitrec_kernel.tcl -messageDb vivado_hls.pb
