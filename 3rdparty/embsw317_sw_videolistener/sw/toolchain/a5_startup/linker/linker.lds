ENTRY(__startup)
    
MEMORY {
    /* Cannot be defined via variables - only numerical constant values */
	entry_point			: ORIGIN = 0xC0000000				LENGTH = 0x1000
    flash_app			: ORIGIN = 0xC0001000				LENGTH = 0x50
	dcd_hd   			: ORIGIN = 0xC0001050,	 			LENGTH = 0xFB0

   a53_0_intvec        : ORIGIN = 0xC0002000,                   		LENGTH = 0x7000                              /* Vector table at the base */
    program_mem	        : ORIGIN = 0xC0009000, 					        LENGTH = 0x8000000 - 0x9000 - 0x2000000 - 0x4000000 /* Main program body */
    heap                : ORIGIN = 0xC9000000 - 0x4000000 - 0x3000000, 	LENGTH = 0x4000000                              /* Heap base */
    stack   	        : ORIGIN = 0xC9000000 - 0x3000000,          	LENGTH = 0x1000000                              /* Stack base */
    stack_irq  	        : ORIGIN = 0xC9000000 - 0x2000000,          	LENGTH = 0x1000000                              /* IRQ Stack base */
    stack_irq2 	        : ORIGIN = 0xC9000000 - 0x1000000,          	LENGTH = 0x1000000                              /* IRQ Stack base */
    uart                : ORIGIN = 0x3B000000,                   		LENGTH = 0x100
	mem_oal             : ORIGIN = 0xC9000000,                   		LENGTH = 0x7000000                            /* Memory for OAL allocations */
	apex20              : ORIGIN = 0x74000000,                   		LENGTH = 0x4000000
	apex21              : ORIGIN = 0x78000000,                   		LENGTH = 0x4000000
}

/* Program layout for running out of RAM. */

SECTIONS
{
	.entry_pt		  ORIGIN(entry_point) : { *(.entry_pt*)  }  > entry_point	
	.flash_app_hdr    ORIGIN(flash_app) : { *(.flash_app_hdr*)  }  > flash_app	
	.boot_data        ORIGIN(flash_app) + 0x30 : { *(.boot_data*)  }  
	.dcd_header 	  ORIGIN(dcd_hd)	: { *(.dcd_header); *(.dcd*) } > dcd_hd
	
    .vectable          ORIGIN(a53_0_intvec) : 
	{
        __vector_table_start = .;
        *(.intc_vector*)
        *(.vectable*)
	} > a53_0_intvec
    __vector_table_size = SIZEOF(.vectable);
    
	.exception_handlers ORIGIN(program_mem) : 
	{
        __exception_table_start = .;
	    *(.exception_table*)
		*(.exception_handlers*)
        *(.monitor_vector_table*)
	} > program_mem
	__exception_table_size = SIZEOF(.exception_handlers);
	
	.startup  	           ALIGN(0)             : 
    {
	*(.startup*)      
	} 
	
	.rodata 	       ALIGN(16)            : {*(.rodata*)}
	.text  	           ALIGN(0)             : 
    {
        __text_start = .;
		*(.text*)      
        __text_end = .;
	} 	  
    __text_size = SIZEOF(.text);
    
	.data               ALIGN(0)            : AT ( ADDR (.text) + SIZEOF (.text) )
    { 
        __data_start = .;
	    _data_lma = LOADADDR(.data); 
		_data_vma = .;
        *(.data);
		*(.data.*);
        _data_lma_end = .; 
		_data_vma_end = .;
        . = ALIGN(16);
        __test_cases_start = .;
        *(.test_cases);
        __test_cases_end = .;
        __data_end = .;
	}
    __data_size = SIZEOF(.data);
    
	.bss               ALIGN(16)            : 
	{
        __bss_start = .;
        _BSS_BEGIN    = .;
		__bss_start__ = .;
        *(.bss)    
		*(.bss.*)
        *(COMMON)        
		__bss_end__   = .;
        _BSS_END      = .;
        __bss_end = .;
    }
    __bss_size = SIZEOF(.bss);
    
    .test_section ALIGN(0x1000) :
    {
        __test_section_start = .;
        *(.test_section);
        __test_section_end = .;
    }
    __test_section_size = SIZEOF(.test_section);
	
    .syscall 	       ALIGN(16)            : {*(.syscall*)}
    .secinfo 	       ALIGN(16)            : {*(.secinfo*)}
    .sdabase 	       ALIGN(16)            : {*(.sdabase*)}
    .sdata 	           ALIGN(16)            : {*(.sdata*)}
    .sdata2 	       ALIGN(16)            : {*(.sdata2*)}
    .sbss 	           ALIGN(16)            : {*(.sbss*)}
    .xlat_table        ALIGN(16)            : {*(.xlat_table*)}
    .t_tables          ALIGN(16)            : {*(.t_tables*)}
	.gcc_except_table  ALIGN(16)            : {*(.gcc_except_table*)}
    .eh_frame          ALIGN(16)            : {*(.eh_frame*)}
	.init 		       ALIGN(16)            : 
	{
         __init_array_start__ = .;
		*( EXCLUDE_FILE (*crtbegin.o) .init_array*);
		__init_array_end__ = .;
		*(.init);
		*(.init*);
		
		    
    }
	.interp            ALIGN(16)            : {*(.interp*)}
	.gnu               ALIGN(16)            : {*(.gnu*)}
	.fini              ALIGN(16)            : {*(.fini*)}
	.dynsym            ALIGN(16)            : {*(.dynsym*)}
	.dynstr            ALIGN(16)            : {*(.dynstr*)}
	.hash              ALIGN(16)            : {*(.hash*)}
	.got               ALIGN(16)            : {*(.got*)}
	.dynamic           ALIGN(16)            : {*(.dynamic*)}
	.rela              ALIGN(16)            : {*(.rela*)}
	.got               ALIGN(16)            : {*(.got*)}
	.jcr               ALIGN(16)            : {*(.jcr*)}
	.note              ALIGN(16)            : {*(.note*)}
	.ARM.extab         ALIGN(16)            : {*(.ARM.extab*)}
	.ARM.exidx         ALIGN(16)            : 
	{
		__exidx_start = .;
		*(.ARM.exidx) *(.ARM.exidx.*)
		
		__exidx_end = .;
	}
    
	.heap               ORIGIN(heap)        : {*(.heap*)} > heap 
    
	.stack              ORIGIN(stack)       :
    {
        __stack_start = .;
        *(.stack*)
        __stack_end = .;
    } > stack
    __stack_size = 0x1000000;
  
    .stack_irq          ORIGIN(stack_irq)   : {*(.stack_irq*)} > stack_irq
  
    .stack_irq2         ORIGIN(stack_irq2)  : {*(.stack_irq2*)} > stack_irq2

    __SP_INIT           = ADDR(.stack) + 0x1000000;
    __SM_STACK_START    = __SP_INIT;
    
    __SP_IRQ_INIT       = ADDR(.stack_irq) + 0x1000000;
    
    __SP_IRQ2_INIT      = ADDR(.stack_irq2) + 0x1000000;
}

PROVIDE (end = .);
PROVIDE (_exit = .);