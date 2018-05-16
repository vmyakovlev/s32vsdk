ENTRY(__startup)
    
MEMORY 
{
  /* Cannot be defined via variables - only numerical constant values */
  entry_point   : ORIGIN = 0xC0000000                           LENGTH = 0x1000
  flash_app     : ORIGIN = 0xC0001000                           LENGTH = 0x50
  dcd_hd        : ORIGIN = 0xC0001050,                          LENGTH = 0xFB0
  a53_0_intvec  : ORIGIN = 0xC0002000,                          LENGTH = 0x7000                              /* Vector table at the base */
  program_mem   : ORIGIN = 0xC0009000,                          LENGTH = 0xB000000 - 0x9000 - 0x3000000 - 0x6000000 /* Main program body */
  
  heap          : ORIGIN = 0xCB000000 - 0x6000000 - 0x3000000,  LENGTH = 0x6000000                              /* Heap base */
  stack         : ORIGIN = 0xCB000000 - 0x3000000,              LENGTH = 0x1000000                              /* Stack base */
  stack_irq     : ORIGIN = 0xCB000000 - 0x2000000,              LENGTH = 0x1000000                              /* IRQ Stack base */
  stack_irq2    : ORIGIN = 0xCB000000 - 0x1000000,              LENGTH = 0x1000000                              /* IRQ Stack base */
  
  uart          : ORIGIN = 0x3B000000,                          LENGTH = 0x100
  
  mem_oal_ddr0  : ORIGIN = 0x8B000000,                          LENGTH = 0x5000000                            /* Memory for OAL allocations */
  mem_oal_ddr1  : ORIGIN = 0xCB000000,                          LENGTH = 0x5000000                            /* Memory for OAL allocations */
  mem_oal_sram  : ORIGIN = 0x3E800000,                          LENGTH = 0x400000                            /* Memory for OAL allocations */
  
  apex20        : ORIGIN = 0x74000000,                          LENGTH = 0x4000000
  apex21        : ORIGIN = 0x78000000,                          LENGTH = 0x4000000
  sram_single_s : ORIGIN = 0x3e800000,                          LENGTH = 0x0                                  /* SRAM memory single banked region not touched by SRAM library */
  sram_single_d : ORIGIN = 0x3e800000,                          LENGTH = 0x300000                             /* SRAM memory single banked region managed by SRAM library */
  sram_multi_s  : ORIGIN = 0x3eb00000,                          LENGTH = 0x0                                  /* SRAM memory multi banked region not touched by SRAM library */
  sram_multi_d  : ORIGIN = 0x3eb00000,                          LENGTH = 0x100000                             /* SRAM memory multi banked region managed by SRAM library */  
}

/* Program layout for running out of RAM. */

SECTIONS
{
  .entry_pt         ORIGIN(entry_point)       : { *(.entry_pt*)  }  > entry_point	
  .flash_app_hdr    ORIGIN(flash_app)         : { *(.flash_app_hdr*)  }  > flash_app	
  .boot_data        ORIGIN(flash_app) + 0x30  : { *(.boot_data*)  }  
  .dcd_header       ORIGIN(dcd_hd)            : { *(.dcd_header); *(.dcd*) } > dcd_hd

  .vectable         ORIGIN(a53_0_intvec) : 
  {
    *(.intc_vector*)
    *(.vectable*)
  } > a53_0_intvec
    
  .exception_handlers ORIGIN(program_mem) : 
  {
    *(.exception_table*)
    *(.exception_handlers*)
  } > program_mem

  .startup         ALIGN(0)             : 
  {
    *(.startup*)      
  } 

  .rodata          ALIGN(16)            : {*(.rodata*)}
  .text            ALIGN(0)             : 
  {
    *(.text*)      
  }   

  .data            ALIGN(0)             : AT ( ADDR (.text) + SIZEOF (.text) )
  { 
    _data_lma = LOADADDR(.data); 
    _data_vma = .;
    *(.data);
    *(.data.*);
    _data_lma_end = .; 
    _data_vma_end = .;
  }
  
  .bss             ALIGN(16)            : 
  {
    _BSS_BEGIN    = .;
    __bss_start__ = .;
    *(.bss)    
    *(.bss.*)
    *(COMMON)        
    __bss_end__   = .;
    _BSS_END      = .;      
  }

  .syscall            ALIGN(16)         : {*(.syscall*)}
  .secinfo            ALIGN(16)         : {*(.secinfo*)}
  .sdabase            ALIGN(16)         : {*(.sdabase*)}
  .sdata              ALIGN(16)         : {*(.sdata*)}
  .sdata2             ALIGN(16)         : {*(.sdata2*)}
  .sbss               ALIGN(16)         : {*(.sbss*)}
  .xlat_table         ALIGN(16)         : {*(.xlat_table*)}
  .gcc_except_table   ALIGN(16)         : {*(.gcc_except_table*)}
  .eh_frame           ALIGN(16)         : {*(.eh_frame*)}
  .init               ALIGN(16)         : 
  {
    __init_array_start__ = .;
    *( EXCLUDE_FILE (*crtbegin.o) .init_array*);
    __init_array_end__ = .;
    *(.init);
    *(.init*);      
  }
  .interp            ALIGN(16)          : {*(.interp*)}
  .gnu               ALIGN(16)          : {*(.gnu*)}
  .fini              ALIGN(16)          : {*(.fini*)}
  .dynsym            ALIGN(16)          : {*(.dynsym*)}
  .dynstr            ALIGN(16)          : {*(.dynstr*)}
  .hash              ALIGN(16)          : {*(.hash*)}
  .got               ALIGN(16)          : {*(.got*)}
  .dynamic           ALIGN(16)          : {*(.dynamic*)}
  .rela              ALIGN(16)          : {*(.rela*)}
  .got               ALIGN(16)          : {*(.got*)}
  .jcr               ALIGN(16)          : {*(.jcr*)}
  .note              ALIGN(16)          : {*(.note*)}
  .ARM.extab         ALIGN(16)          : {*(.ARM.extab*)}
  .ARM.exidx         ALIGN(16)          : 
  {
    __exidx_start = .;
    *(.ARM.exidx) *(.ARM.exidx.*)
    
    __exidx_end = .;
  }
    
  .heap              ORIGIN(heap)         : 
  {
    _heap_start = .;
    *(.heap*);
    _heap_end   = .;
  } > heap
    
  .stack             ORIGIN(stack)         : {*(.stack*)} > stack   

  .stack_irq         ORIGIN(stack_irq)         : {*(.stack_irq*)} > stack_irq

  .stack_irq2        ORIGIN(stack_irq2)         : {*(.stack_irq2*)} > stack_irq2
  
  __SRAM_SINGLE_S_START  = ORIGIN(sram_single_s);
  __SRAM_SINGLE_S_SIZE   = LENGTH(sram_single_s);
  __SRAM_SINGLE_D_START  = ORIGIN(sram_single_d);
  __SRAM_SINGLE_D_SIZE   = LENGTH(sram_single_d);
  __SRAM_MULTI_S_START   = ORIGIN(sram_multi_s);
  __SRAM_MULTI_S_SIZE    = LENGTH(sram_multi_s);
  __SRAM_MULTI_D_START   = ORIGIN(sram_multi_d);  
  __SRAM_MULTI_D_SIZE    = LENGTH(sram_multi_d);

  __SP_INIT       = ADDR(.stack) + 0x1000000;
  
  __SP_IRQ_INIT       = ADDR(.stack_irq) + 0x1000000;
  
  __SP_IRQ2_INIT       = ADDR(.stack_irq2) + 0x1000000;
}

PROVIDE (end = .);
PROVIDE (_exit = .);