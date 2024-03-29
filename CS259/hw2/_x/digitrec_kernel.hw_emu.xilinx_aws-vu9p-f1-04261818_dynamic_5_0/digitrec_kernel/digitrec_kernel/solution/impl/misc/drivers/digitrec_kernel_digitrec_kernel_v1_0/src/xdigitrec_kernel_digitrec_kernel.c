// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2018.2_AR71275_op
// Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

/***************************** Include Files *********************************/
#include "xdigitrec_kernel_digitrec_kernel.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XDigitrec_kernel_digitrec_kernel_CfgInitialize(XDigitrec_kernel_digitrec_kernel *InstancePtr, XDigitrec_kernel_digitrec_kernel_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XDigitrec_kernel_digitrec_kernel_Start(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL) & 0x80;
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XDigitrec_kernel_digitrec_kernel_IsDone(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XDigitrec_kernel_digitrec_kernel_IsIdle(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XDigitrec_kernel_digitrec_kernel_IsReady(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XDigitrec_kernel_digitrec_kernel_EnableAutoRestart(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XDigitrec_kernel_digitrec_kernel_DisableAutoRestart(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_AP_CTRL, 0);
}

void XDigitrec_kernel_digitrec_kernel_Set_test_image(XDigitrec_kernel_digitrec_kernel *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TEST_IMAGE_DATA, (u32)(Data));
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TEST_IMAGE_DATA + 4, (u32)(Data >> 32));
}

u64 XDigitrec_kernel_digitrec_kernel_Get_test_image(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TEST_IMAGE_DATA);
    Data += (u64)XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TEST_IMAGE_DATA + 4) << 32;
    return Data;
}

void XDigitrec_kernel_digitrec_kernel_Set_train_images(XDigitrec_kernel_digitrec_kernel *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TRAIN_IMAGES_DATA, (u32)(Data));
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TRAIN_IMAGES_DATA + 4, (u32)(Data >> 32));
}

u64 XDigitrec_kernel_digitrec_kernel_Get_train_images(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TRAIN_IMAGES_DATA);
    Data += (u64)XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_TRAIN_IMAGES_DATA + 4) << 32;
    return Data;
}

void XDigitrec_kernel_digitrec_kernel_Set_knn_mat(XDigitrec_kernel_digitrec_kernel *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_KNN_MAT_DATA, (u32)(Data));
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_KNN_MAT_DATA + 4, (u32)(Data >> 32));
}

u64 XDigitrec_kernel_digitrec_kernel_Get_knn_mat(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_KNN_MAT_DATA);
    Data += (u64)XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_KNN_MAT_DATA + 4) << 32;
    return Data;
}

void XDigitrec_kernel_digitrec_kernel_InterruptGlobalEnable(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_GIE, 1);
}

void XDigitrec_kernel_digitrec_kernel_InterruptGlobalDisable(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_GIE, 0);
}

void XDigitrec_kernel_digitrec_kernel_InterruptEnable(XDigitrec_kernel_digitrec_kernel *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_IER);
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_IER, Register | Mask);
}

void XDigitrec_kernel_digitrec_kernel_InterruptDisable(XDigitrec_kernel_digitrec_kernel *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_IER);
    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_IER, Register & (~Mask));
}

void XDigitrec_kernel_digitrec_kernel_InterruptClear(XDigitrec_kernel_digitrec_kernel *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XDigitrec_kernel_digitrec_kernel_WriteReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_ISR, Mask);
}

u32 XDigitrec_kernel_digitrec_kernel_InterruptGetEnabled(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_IER);
}

u32 XDigitrec_kernel_digitrec_kernel_InterruptGetStatus(XDigitrec_kernel_digitrec_kernel *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XDigitrec_kernel_digitrec_kernel_ReadReg(InstancePtr->Control_BaseAddress, XDIGITREC_KERNEL_DIGITREC_KERNEL_CONTROL_ADDR_ISR);
}

