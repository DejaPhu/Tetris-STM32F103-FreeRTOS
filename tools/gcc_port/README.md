# FreeRTOS GCC ARM_CM3 port provenance

Files port.c and portmacro.h are unmodified upstream sources from:
https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/V10.3.1-kernel-only/portable/GCC/ARM_CM3

They match the kernel version 10.3.1 shipped in the CubeMX project. Original MIT headers are retained. They are used only by tools/build_gcc.py. The Keil project still builds its original RVDS/ARM_CM3 port and original ARMASM startup.
