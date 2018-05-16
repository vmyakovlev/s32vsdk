ENTRY(_start_cm4_0)

STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x2000;
HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x2000;
    
MEMORY {
	m4_0_intvec         : ORIGIN = 0x1E000000,              LENGTH = 0x2000
    TCM_CODE            : ORIGIN = 0x1E002000,              LENGTH = 32K - 0x2000
    TCM_DATA            : ORIGIN = 0x3E000000,              LENGTH = 32K 
    
}
   
SECTIONS
{
    .intvec           ORIGIN(m4_0_intvec)  : 
	{
	    M4_0_VECTOR_TABLE = .;
		M4_0_VECTOR_RAM = .;
	    *(.intvec*)
		M4_0_VECTOR_SIZE = LENGTH(m4_0_intvec);
	}  > m4_0_intvec
    .init             ORIGIN(TCM_CODE)         : { *(.init)  }  > TCM_CODE
	.text 	          ALIGN(0)             : {*(.text*)}  > TCM_CODE
    .syscall 	      ALIGN(0)             : {*(.syscall*)}  > TCM_CODE
    .secinfo 	      ALIGN(0)             : {*(.secinfo*)}  > TCM_CODE
	.fini    	      ALIGN(0)             : {*(.fini*)}  > TCM_CODE
	.eh_frame  	      ALIGN(0)             : {*(.eh_frame*)}  > TCM_CODE
	.init_array	      ALIGN(0)             : {*(.init_array*)}  > TCM_CODE
	.jcr	          ALIGN(0)             : {*(.jcr*)}  > TCM_CODE
	.ARM.exidx  	  ALIGN(0)             : {*(..ARM.exidx*)}  > TCM_CODE
    .rodata 	      ALIGN(0)             : {*(.rodata*)}  > TCM_CODE
	.sdabase 	      ALIGN(16)            : {*(.sdabase*)}  > TCM_CODE
	.sdata 	          ALIGN(0)             : {*(.sdata*)}  > TCM_CODE
	.sdata2 	      ALIGN(0)             : {*(.sdata2*)}  > TCM_CODE
	.sbss 	          ALIGN(0)             : {*(.sbss*)}  > TCM_CODE
	.data 	          ALIGN(0)             : 
	{
	    DATA_SRAM = .;
        DATA_ROM = .;
	    *(.data*)
		DATA_ROM_END = .;
	}  > TCM_CODE
.bss ALIGN(16) :
    {
        _BSS_BEGIN = .;
                                __bss_start__ = .;
        *(.bss*)       /* Zero-filled run time allocate data memory */
        *(COMMON)        
                                __bss_end__ = .;
        _BSS_END = .;
    }   > TCM_CODE
    
	.CodeRelocateRam  ALIGN(0)             : 
	{
	    CODE_RELOCATE_SRAM = .;
	    CODE_RELOCATE_ROM = .;
	    *(.CodeRelocateRam*)
		CODE_RELOCATE_ROM_END = .;
	}  > TCM_CODE
		
	/* stack and heap shall be located in the same memory region */
	.heap     ORIGIN(TCM_DATA): 
    {
    	. = ALIGN(8);
        __end__ = .;
        _end = .;
    	PROVIDE(end = .);
    	. += HEAP_SIZE;
    }  > TCM_DATA   

    .stack   :
    {
        . = ALIGN(8);
        . += STACK_SIZE;
        __CM4_0_STACK_ADDRESS = .; 
    } > TCM_DATA
}