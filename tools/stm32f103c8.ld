/* 64-KiB Flash / 20-KiB SRAM. Link failure is preferable to silent overflow. */
ENTRY(Reset_Handler)
MEMORY { FLASH(rx): ORIGIN = 0x08000000, LENGTH = 64K
 RAM(rwx): ORIGIN = 0x20000000, LENGTH = 20K }
_estack = ORIGIN(RAM)+LENGTH(RAM);
SECTIONS {
 .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
 .text : { *(.text*) *(.rodata*) . = ALIGN(4); } > FLASH
 .ARM.extab : { *(.ARM.extab*) } > FLASH
 .ARM.exidx : { *(.ARM.exidx*) } > FLASH
 _sidata = LOADADDR(.data);
 .data : { . = ALIGN(4); _sdata = .; *(.data*) . = ALIGN(4); _edata = .; } > RAM AT > FLASH
 .bss (NOLOAD) : { . = ALIGN(8); _sbss = .; *(.bss*) *(COMMON) . = ALIGN(8); _ebss = .; } > RAM
 .heap_stack (NOLOAD) : { . = ALIGN(8); end = .; . += 512; . += 1024; } > RAM
 ASSERT(SIZEOF(.isr_vector)==236, "Invalid vector table")
 ASSERT(SIZEOF(.data)+SIZEOF(.bss)+SIZEOF(.heap_stack)<=LENGTH(RAM), "RAM overflow")
}
