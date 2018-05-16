/**
  @file             sm_linker.lds
  @version          0.0.0.0

  @brief            The SM core linker script
  @details

  Project           M4_ETH_PROJECT_NAME
  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/

MEMORY {
	theMem  : ORIGIN = 0xC4F00000 LENGTH = 0x100000
}

SECTIONS {
    .monitor_vector_table ORIGIN(theMem) : {
        __sm_vector_table_start = .;
        *(.monitor_vector_table*)
    } > theMem
    __sm_vector_table_size = SIZEOF(.monitor_vector_table);
    
    .t_tables ALIGN(0x1000) : 
    {
        __sm_t_tables_start = .;
        *(.t_tables*)
    } > theMem
    __sm_t_tables_size = SIZEOF(.t_tables);

    .sm_stack ALIGN(0x1000) :
    {
        __sm_stack_start = .;
        __smstack_size = 1024;
        __smstack_end = .;
        . = . + __smstack_size;
        __smstack_start = .;
    } > theMem
    __sm_stack_size = SIZEOF(.sm_stack);
    
    .general_stack ALIGN(0x1000) :
    {
        __general_stack_size = 1024;
        . = . + __general_stack_size;
        __SP_INIT = .;
    } > theMem
    
    .sm_rodata_cached ALIGN(0x1000) :
    {
        __sm_rodata_cached_start = .;
        *(.rodata*);
        __sm_init_vec_start = .;
        *(.sm_init_vectors);
        __sm_init_vec_end = .;
        __test_cases_start = .;
        *(.test_cases);
        __test_cases_end = .;
    } > theMem
    __sm_rodata_cached_size = SIZEOF(.sm_rodata_cached);
    
    .sm_text_cached ALIGN(0x1000) :
    {
        __sm_text_cached_start = .;
        *(.text*)
    } > theMem
    __sm_text_cached_size = SIZEOF(.sm_text_cached);
    
	.sm_data_cached ALIGN(0x1000) :
    {
        __sm_data_cached_start = .;
        *(.data*)
        *(.sdata*)
        *(.sdata2*)
        . = ALIGN(8);
        __bss_start__ = .;
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        __bss_end__   = .;
        __sm_sc_prm_start = .;
        *(.sm_sc_param_storage);
        __sm_sc_prm_end = .;
        __sm_sc_qr_start = .;
        *(.sm_query_result_storage);
        __sm_sc_qr_end = .;
    } > theMem
    __sm_data_cached_size = SIZEOF(.sm_data_cached);
    
    .sm_data_non_cached ALIGN(0x1000) :
    {
        __sm_data_non_cached_start = .;
    } > theMem
    __sm_data_non_cached_size = SIZEOF(.sm_data_non_cached);
    
	.note       ALIGN(16)   : {*(.note*)}   > theMem

    __SM_STACK_START = __smstack_start;
}
