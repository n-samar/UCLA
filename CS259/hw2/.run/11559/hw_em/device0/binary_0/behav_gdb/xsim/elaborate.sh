#!/bin/bash -f
# ****************************************************************************
# Vivado (TM) v2018.2_AR71275_op (64-bit)
#
# Filename    : elaborate.sh
# Simulator   : Xilinx Vivado Simulator
# Description : Script for elaborating the compiled design
#
# Generated by Vivado on Sat Oct 27 03:58:55 UTC 2018
# SW Build 2258646 on Thu Jun 14 20:02:38 MDT 2018
#
# Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
#
# usage: elaborate.sh
#
# ****************************************************************************
xv_lib_path="/opt/Xilinx/Vivado/2018.2.op2258646/patches/AR71275/vivado/lib/lnx64.o/Default:/opt/Xilinx/Vivado/2018.2.op2258646/patches/AR71275/vivado/lib/lnx64.o:/opt/Xilinx/Vivado/2018.2.op2258646/lib/lnx64.o/Default:/opt/Xilinx/Vivado/2018.2.op2258646/lib/lnx64.o"
ExecStep()
{
"$@"
RETVAL=$?
if [ $RETVAL -ne 0 ]
then
exit $RETVAL
fi
}

ExecStep xsc --shared --gcc_link_options "-L/opt/Xilinx/Vivado/2018.2.op2258646/data/simmodels/xsim/2018.2/lnx64/6.2.0/ext/protobuf/ -lprotobuf"  -lib xil_defaultlib -o libdpi.so
ExecStep xelab -wto c26d7ddb419249e99af5ed6fc3dcc120 --incr --debug off --relax --mt 8 -sv_root "." -sv_lib libsdaccel_generic_pcie_v2_0.so -sv_root "." -sv_lib libperformance_monitor_v2_0.so -sv_root "." -sv_lib libsystemc_synchronizer.so -sv_root "." -sv_lib libxsc_main_main.so --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/ddrx_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/axi_crossbar_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/top" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/xdma_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/ddrx_v3/cpp_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/common" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/xdma_v3/cpp_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_0_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_0_0/sim" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_1_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_1_0/sim" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_accel_mon_0_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_accel_mon_0_0/sim" -L sim_clk_gen_v1_0_2 -L xil_defaultlib -L lib_cdc_v1_0_2 -L proc_sys_reset_v5_0_12 -L generic_baseblocks_v2_1_0 -L axi_infrastructure_v1_1_0 -L axi_register_slice_v2_1_17 -L fifo_generator_v13_2_2 -L axi_data_fifo_v2_1_16 -L axi_crossbar_v2_1_18 -L axi_protocol_converter_v2_1_17 -L axi_clock_converter_v2_1_16 -L blk_mem_gen_v8_4_1 -L axi_dwidth_converter_v2_1_17 -L unisims_ver -L unimacro_ver -L secureip -L xpm -sv_root "." -sc_lib libdpi.so --snapshot emu_wrapper_behav xil_defaultlib.emu_wrapper xil_defaultlib.glbl -log elaborate.log -standalone --include /opt/Xilinx/Vivado/2018.2.op2258646/data/simmodels/xsim/2018.2/lnx64/6.2.0/ext/protobuf/include --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/ddrx_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/axi_crossbar_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/top" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/xdma_v3/systemc_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/ddrx_v3/cpp_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/common" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sdaccel_generic_pcie_0_0/sim_tlm/xdma_v3/cpp_srcs" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_0_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_0_0/sim" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_1_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_axi_perf_mon2_1_0/sim" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_accel_mon_0_0/sysc" --include "../../../../prj.srcs/sources_1/bd/emu/ip/emu_sim_accel_mon_0_0/sim"
