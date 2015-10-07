#!/bin/bash

export PROJECT_TARGET_DIR=.
export PROJECT_SRC_DIR=../../../src

export CROSS_COMPILE=arm-linux-gnueabihf-
export TARGET_TOOLS_DIR=/opt/arm-toolchain
export TARGET_ROOTFS_DIR=/opt/arch-linux-arm/rootfs

PATH=$TARGET_TOOLS_DIR/bin/:$PATH
