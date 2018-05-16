ENTRY(_start_cm4_0)
    
MEMORY {
	m4_0_intvec         : ORIGIN = 0x3E800000,              LENGTH = 0x2000
    sram      	        : ORIGIN = 0x3E802000,              LENGTH = 1024K - 0x2000 - 4K
    heap                : ORIGIN = 0x3E900000 - 4K - 0K,    LENGTH = 0K
    stack   	        : ORIGIN = 0x3E900000 - 4K,         LENGTH = 4K
    c5_0_code           : ORIGIN = 0x3E900000,              LENGTH = 1024K
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
    .init             ORIGIN(sram)         : { *(.init)  }  > sram
	.text 	          ALIGN(0)             : {*(.text*)}
    .syscall 	      ALIGN(0)             : {*(.syscall*)}
    .secinfo 	      ALIGN(0)             : {*(.secinfo*)}
	.fini    	      ALIGN(0)             : {*(.fini*)}
	.eh_frame  	      ALIGN(0)             : {*(.eh_frame*)}
	.init_array	      ALIGN(0)             : {*(.init_array*)}
	.jcr	          ALIGN(0)             : {*(.jcr*)}
	.ARM.exidx  	  ALIGN(0)             : {*(..ARM.exidx*)}
    .rodata 	      ALIGN(0)             : {*(.rodata*)}
	.sdabase 	      ALIGN(16)            : {*(.sdabase*)}
	.sdata 	          ALIGN(0)             : {*(.sdata*)}
	.sdata2 	      ALIGN(0)             : {*(.sdata2*)}
	.sbss 	          ALIGN(0)             : {*(.sbss*)}
	.data 	          ALIGN(0)             : 
	{
	    DATA_SRAM = .;
        DATA_ROM = .;
	    *(.data*)
		DATA_ROM_END = .;
	}
.bss ALIGN(16) :
    {
        _BSS_BEGIN = .;
                                __bss_start__ = .;
        *(.bss*)       /* Zero-filled run time allocate data memory */
        *(COMMON)        
                                __bss_end__ = .;
        _BSS_END = .;
    } 
    
	.CodeRelocateRam  ALIGN(0)             : 
	{
	    CODE_RELOCATE_SRAM = .;
	    CODE_RELOCATE_ROM = .;
	    *(.CodeRelocateRam*)
		CODE_RELOCATE_ROM_END = .;
	}
		
    .heap                                  : {*(.heap*)}    > heap 
    .stack                                 : 
	{
	  __CM4_0_STACK_ADDRESS = . + 4K; 
	} > stack  
	.c5_0_code                             : {*(.stack*)}   > c5_0_code  
}