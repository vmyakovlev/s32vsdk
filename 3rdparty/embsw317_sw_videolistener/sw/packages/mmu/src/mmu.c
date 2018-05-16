/**
  @defgroup         dxgrMMU MMU Driver
  @details          Purpose of the MMU module is to provide a simple interface to the virtual memory
                    subsystem. It contains functionality enabling user to create virtual to physical
                    memory mappings, setting various memory attributes, enabling and disabling the
                    memory management unit and perform certain cache-related operations.
                    
                    Translation parameters are configured statically and refers to 3 translation
                    levels with the smallest granule size 4kB. Input addresses are limited to be
                    at most 32 bits long due to translation tables size reduction, output address
                    is always 32 bit long.
                    
                    Module operation
                    ----------------
                    Module expects to be properly initialized by @ref mmu_init() before any other
                    operation is performed.
                    @note Currently only the EL3 is supported and call to @ref mmu_init() on
                          different exception level will cause the MMU_E_SYSTEM error.
                    
                    When properly initialized user calls the @ref mmu_add_mapping() to prepare
                    translation tables according to desired setup and is responsible for correct
                    memory segmentation. Size of the translation tables is limited by number of entries
                    defined by @ref MMU_CFG_ENTRY_POOL_SIZE and should be kept as small as possible to
                    reduce the memory footprint. To prepare the attributes mask parameters for the
                    @ref mmu_add_mapping() one can use the @ref mmu_get_attr() to properly combine
                    chosen memory type and memory attributes as they are defined within mmu_cfg.h
                    and mmu_mem_attr.h headers. Once all mappings are prepared the MMU can be started
                    by @ref mmu_start().
                    
                    Translation can be stopped by @ref mmu_stop().
                    @note To ensure memory coherency after the MMU is stopped the caches should
                          be cleaned up. User can clean caches for various memory ranges by calls
                          of @ref cache_d_clean_by_va_range().
                    
                    Memory attributes
                    -----------------
                    Each mapping requires a set of memory attributes when its being created by
                    the @ref mmu_add_mapping(). The helper function @ref mmu_get_attr() accepts
                    two parameters: memory type and attributes mask. The attributes mask can contain
                    various attributes joined by bitwise OR operation. Currently supported memory
                    types are configurable and provided by the mmu_cfg.h in form of macros prefixed
                    with MTYPE_. Memory attributes are provided by mmu_mem_attr.h as macros
                    prefixed with MATTR_. Example use of the @ref mmu_get_attr() can look like
                    this:
                    \code
                        mem_attr_t theAttrMask = mmu_get_attr(MTYPE_NORMAL, MA_EL2_EL3_RW | MA_NON_EXEC)
                    \endcode

  @addtogroup       dxgrMMU
  @{

  @file             mmu.c
  @brief            The MMU driver
  @details

  Project           M4_ETH_PROJECT_NAME

  Platform          M4_ETH_PLATFORM

  SWVersion         M4_ETH_RELEASE_VERSION

  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
*/
/*==================================================================================================
==================================================================================================*/

/*==================================================================================================
                                         MISRA VIOLATIONS
==================================================================================================*/
/*
* @page misra_violations MISRA-C:2004 violations
*
* @section mmu_c_REF_1
* Violates MISRA 2004 Advisory Rule 19.7, Function should be defined rather than macro.
* This macro is supposed to be used only with narrow set of input parameters or does not need type
* check. Each usage has been reviewed.
*
* @section mmu_c_REF_2
* Violates MISRA 2004 Required Rule 18.4, Unions shall not be used.
* Use of union to represent data is suitable here and significantly reduces code complexity. It
* has been proven that currently supported compiler accesses memory as expected.
*
* @section mmu_c_REF_3
* Violates MISRA 2004 Required Rule 6.4, Bit fields shall only be defined to be of type unsigned
* int or signed int.
* It is crucial to define long bit field here to cover full range of HW
* supported values.
*
* @section mmu_c_REF_4
* Violates MISRA 2004 Required Rule 1.2, No reliance shall be placed on undefined or unspecified
* behaviour.
* Address of near auto variable is needed to locate data by a sub-routine or to return
* a value via pointer.
*
* @section mmu_c_REF_5
* Violates MISRA 2004 Required Rule 14.7, A function shall have a single point of exit at the end
* of the function.
* Multiple exit points here decreases code complexity and improves readability.
*
* @section mmu_c_REF_6
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type.
* Cast is used to store address to the memory.
*
* @section mmu_c_REF_7
* Violates MISRA 2004 Advisory Rule 11.3, A cast should not be preformed between a pointer type and
* an integral type.
* Cast is used to wrap an output value.
*
* @section mmu_c_REF_8
* Violates MISRA 2004 Advisory Rule 12.11, Evaluation of constant unsigned integer expressions
* should not lead to wrap-around.
* Overflow here is desired and used to compute a bit mask.
*
* @section mmu_c_REF_9
* Violates MISRA 2004 Required Rule 9.1, All automatic variables shall have been assigned a value
* before being used.
* Variable is set in an __asm statement but usage is not detected.
*
*/



/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "common_types.h"
#include "mmu_mem_attr.h"
#include "mmu_cfg.h"
#include "mmu.h"

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
/** @cond dxINTERNAL_MACROS */
#ifndef TRUE
    #define TRUE    1U
#endif
#ifndef FALSE
    #define FALSE   0U
#endif

/*  Enables/Disables development errors detection */
#define MMU_CFG_DEV_ERROR_DETECTION         (FALSE)

/*  TCR_EL3 */
/*  Top Byte ignored */
#define TCR_TBI                             (1U << 20)
/*  Physical Address Size */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_PS(n)                           (((uint32_t)(n) & 0x7U) << 16)
#define TCR_PS_4GB                          0x0U    /* 0b000U */  /*  32 bits, 4GB */
#define TCR_PS_64GB                         0x1U    /* 0b001U */  /*  36 bits, 64GB */
#define TCR_PS_1TB                          0x2U    /* 0b010U */  /*  40 bits, 1TB */
#define TCR_PS_4TB                          0x3U    /* 0b011U */  /*  42 bits, 4TB */
#define TCR_PS_16TB                         0x4U    /* 0b100U */  /*  44 bits, 16TB */
#define TCR_PS_256TB                        0x5U    /* 0b101U */  /*  48 bits, 256TB */
/*  Granule size */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_TG0(n)                          (((uint32_t)(n) & 0x3U) << 14)
#define TCR_TG0_4KB                         0x0U    /* 0b00U */   /*   4KB */
#define TCR_TG0_64KB                        0x1U    /* 0b01U */   /*  64KB */
#define TCR_TG0_16KB                        0x2U    /* 0b10U */   /*  16KB */
/*  Shareability attribute (EL3) */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_SH0(n)                          (((uint32_t)(n) & 0x3U) << 12)
#define TCR_SH0_NOSHAREABLE                 0x0U    /* 0b00U */   /*  Non-shareable */
#define TCR_SH0_OUTER_SHAREABLE             0x2U    /* 0b10U */   /*  Outer Shareable */
#define TCR_SH0_INNER_SHAREABLE             0x3U    /* 0b11U */   /*  Inner Shareable */
/*  Outer cacheability attribute (EL3) */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_ORGN0(n)                        (((uint32_t)(n) & 0x3U) << 10)
#define TCR_ORGN0_NORMAL_O_NOCACHE          0x0U    /* 0b00U */   /*  Normal memory, Outer Non-chacheable */
#define TCR_ORGN0_NORMAL_OWBWAC             0x1U    /* 0b01U */   /*  Normal memory, Outer Write-Back Wite-Allocate Cacheable */
#define TCR_ORGN0_NORMAL_OWTC               0x2U    /* 0b10U */   /*  Normal memory, Outer Write-Through Cacheable */
#define TCR_ORGN0_NORMAL_OWBnWAC            0x3U    /* 0b11U */   /*  Normal memory, Outer Write-Back no Write-Allocate Cacheable */
/*  Inner cacheability attribute (EL3) */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_IRGN0(n)                        (((uint32_t)(n) & 0x3U) << 8)
#define TCR_IRGN0_NORMAL_I_NOCACHE          0x0U    /* 0b00U */   /*  Normal memory, Inner Non-cacheable */
#define TCR_IRGN0_NORMAL_IWBWAC             0x1U    /* 0b01U */   /*  Normal memory, Inner Write-Back Write-Allocate Cacheable */
#define TCR_IRGN0_NORMAL_IWTC               0x2U    /* 0b10U */   /*  Normal memory, Inner Write-Through Cacheable */
#define TCR_IRGN0_NORMAL_IWBnWAC            0x3U    /* 0b11U */   /*  Normal memory, Inner Write-Back no Write-Allocate Cacheable */
/*  T0SZ */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define TCR_T0SZ(n)                         ((uint32_t)(n) & 0x3fU)

/*  SCTLR_EL3 */
#define SCTLR_M                             (1U << 0)
#define SCTLR_I                             (1U << 12)
#define SCTLR_C                             (1U << 2)

/*  Platform setup */
#define MMU_PA_SIZE         32U
#define MMU_VA_SIZE         32U /*  Max VA (IA) range is 48-bit but currently just 32-bit addresses
                                    are being requested. This saves space for LVL1 table (instead
                                    of 2^(48-30) just 2^(32-30) entries are needed). This is like
                                    configuration parameter but not exposed via configuration file.
                                    When wider range of input addresses is needed just adjust this
                                    value to required number of bits. */

/*  Initial table size in number of descriptors */
#define MMU_INIT_TABLE_SIZE ((uint32_t)1U << (MMU_VA_SIZE - 30U))

/*  Setup for 4k granule: */
#define L0_LO_BIT_IDX       39U /*  For 4KB granule the L0 table is addressed by IA[47:39] */
#define L1_LO_BIT_IDX       30U /*  For 4KB granule the L1 table is addressed by IA[38:30] */
#define L2_LO_BIT_IDX       21U /*  For 4KB granule the L2 table is addressed by IA[29:21] */
#define L3_LO_BIT_IDX       12U /*  For 4KB granule the L3 table is addressed by IA[20:12] */

#if (MMU_VA_SIZE >= L0_LO_BIT_IDX)
    #define L0_BITS_CNT (MMU_VA_SIZE - L0_LO_BIT_IDX)
    #define START_LEVEL 0U
#else
    #define L0_BITS_CNT 0U
#endif
#if (MMU_VA_SIZE >= L1_LO_BIT_IDX)
    #define L1_BITS_CNT (MMU_VA_SIZE - (L0_BITS_CNT + L1_LO_BIT_IDX))
    #ifndef START_LEVEL
        #define START_LEVEL 1U
    #endif
#else
    #define L1_BITS_CNT 0U
#endif
#if (MMU_VA_SIZE >= L2_LO_BIT_IDX)
    #define L2_BITS_CNT (MMU_VA_SIZE - (L0_BITS_CNT + L1_BITS_CNT + L2_LO_BIT_IDX))
    #ifndef START_LEVEL
        #define START_LEVEL 2U
    #endif
#else
    #define L2_BITS_CNT 0U
#endif
#if (MMU_VA_SIZE >= L3_LO_BIT_IDX)
    #define L3_BITS_CNT (MMU_VA_SIZE - (L0_BITS_CNT + L1_BITS_CNT + L2_BITS_CNT + L3_LO_BIT_IDX))
    #ifndef START_LEVEL
        #define START_LEVEL 3U
    #endif
#else
    #define L3_BITS_CNT 0U
#endif

/*  Table size in number of elements */
/** @violates @ref mmu_c_REF_1 MISRA rule 19.7 */
#define GET_TBL_SIZE(lvl)       ((uint32_t)(1UL << au32BitsCnt[(lvl)]))

/*  Table length in number of entries */
#define L0_TBL_LENGTH       (1 << L0_BITS_CNT)
#define L1_TBL_LENGTH       (1 << L1_BITS_CNT)
#define L2_TBL_LENGTH       (1 << L2_BITS_CNT)
#define L3_TBL_LENGTH       (1 << L3_BITS_CNT)

/*  Addressable region sizes */
#define L0_REGION_SIZE      (1ULL << L0_LO_BIT_IDX)
#define L1_REGION_SIZE      (1ULL << L1_LO_BIT_IDX)
#define L2_REGION_SIZE      (1ULL << L2_LO_BIT_IDX)
#define L3_REGION_SIZE      (1ULL << L3_LO_BIT_IDX)

#define LEVELS_COUNT        4U
#define MAX_IA_RANGE        48U /* bits */
#define START_TABLE         (arInitialTable)
/** @endcond */

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
/*  Long descriptor */
/** @violates @ref mmu_c_REF_2 MISRA rule 18.4 */
typedef union {
    uint64_t u64Descriptor;
    
    struct {
        unsigned int : 2;           /*  Common */
        unsigned int attrindx : 3;  /*  Stage 1 memory attributes index */
        unsigned int ns : 1;        /*  Non-secure */
        unsigned int ap : 2;        /*  Data access permissions */
        unsigned int sh : 2;        /*  Shareability */
        unsigned int af : 1;        /*  Access */
        unsigned int ng : 1;        /*  Not global */
        /** @violates @ref mmu_c_REF_3 MISRA rule 6.4 */
        unsigned long oa : 36;      /*  Output address */
        unsigned int : 4;           /*  RES0 */
        unsigned int cont : 1;      /*  Contiguous */
        unsigned int pxn : 1;       /*  Privileged execute-never */
        unsigned int unx_xn : 1;    /*  Execute-never */
        unsigned int : 4;           /*  Reserved for software use, IGNORED */
        unsigned int : 5;           /*  IGNORED */
    } pageblock;   /* Page/Block descriptor */
    
    struct {
        unsigned int : 2;           /*  Common */
        /** @violates @ref mmu_c_REF_3 MISRA rule 6.4 */
        unsigned long : 46;         /*  Next-level table address */
        unsigned int : 4;           /*  RES0 */
        unsigned int : 7;           /*  IGNORED */
        unsigned int pxn : 1;       /*  PXN limit for subsequent levels of lookup */
        unsigned int uxn : 1;       /*  XN limit for subsequent levels of lookup */
        unsigned int apt : 2;       /*  Access permissions limit for subsequent levels of lookup */
        unsigned int nst : 1;       /*  NSTable */
    } table;  /* Next-Level Table descriptor */
    
    struct {
        unsigned int valid : 1;
        unsigned int bpt : 1;       /*  Block/Page/Table, L0-L1: 1=Table, 0=Block; L3: 1=page */
    } common;
} desc_t;

/*  Descriptor types */
typedef enum {
    DESC_BLOCK,
    DESC_TABLE,
    DESC_PAGE,
    DESC_RESERVED,
    DESC_INVALID
} desc_type_t;

/*  Table entry */
typedef desc_t tbl_entry_t;

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/
static const uint32_t
    au32LowBits[LEVELS_COUNT] = {L0_LO_BIT_IDX, L1_LO_BIT_IDX, L2_LO_BIT_IDX, L3_LO_BIT_IDX};
static const uint32_t
    au32BitsCnt[LEVELS_COUNT] = {L0_BITS_CNT, L1_BITS_CNT, L2_BITS_CNT, L3_BITS_CNT};
static const uint64_t
    au64RegionSizes[LEVELS_COUNT] = {L0_REGION_SIZE, L1_REGION_SIZE, L2_REGION_SIZE, L3_REGION_SIZE};

/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/
/*  Each table must be aligned to the actual size of the table or block of concatenated arTables */
static tbl_entry_t arTables[MMU_CFG_ENTRY_POOL_SIZE]
    __attribute__ ((aligned(sizeof(tbl_entry_t) * L3_TBL_LENGTH), section(".t_tables")));

/*  The initial table is aligned to its size if TxSZ < MaxIARange @ start-up level */
static tbl_entry_t arInitialTable[MMU_INIT_TABLE_SIZE]
    __attribute__ ((__aligned__(sizeof(tbl_entry_t) * MMU_INIT_TABLE_SIZE), section(".t_tables")));

static uint32_t au32CurrentTblidx = 0U;

/*  Flag indicating initialization state */
static boolean_t bInitialized = FALSE;

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
static tbl_entry_t *mmu_get_new_table(uint32_t u32Size);
static desc_type_t mmu_get_desc_type(const tbl_entry_t *const prEntry, const uint32_t u32Level);
static mmu_ret_t mmu_ll_check_mapping(va_t VA, pa_t PA, mlen_t Size, const tbl_entry_t prTable[], uint32_t u32Level, mlen_t *const CheckedLen);
static mmu_ret_t mmu_ll_add_mapping(va_t VA, pa_t PA, mlen_t Size, mem_attr_t Attr, tbl_entry_t prTable[], uint32_t u32Level, mlen_t *const MappedLen);
static mmu_ret_t mmu_desc_get_next_level_table(const tbl_entry_t *const prEntry, const uint32_t u32Level, tbl_entry_t **pprNextTable);
#if 0
static mmu_ret_t mmu_desc_set_output_address(tbl_entry_t *const prEntry, const uint32_t u32Level, const pa_t PA);
#endif /* 0 */
static mmu_ret_t mmu_desc_get_output_address(const tbl_entry_t *const prEntry, const uint32_t u32Level, pa_t *pOutAddr);
static mmu_ret_t mmu_get_mask(const tbl_entry_t *const prEntry, const uint32_t u32Level, addr_t *const pMask);
static boolean_t mmu_is_ia_valid(const va_t VA);
static mmu_ret_t mmu_set_attributes(tbl_entry_t *const prEntry, const uint32_t u32Level, const mem_attr_t AttrMask);
static uint32_t mmu_get_tbl_idx(va_t VA, uint32_t u32Level);

/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/
/*
    @brief Get index within a table corresponding to the given VA
    @param[in] VA Input address
    @param[in] u32Level The desired translation level
    @return Index of entry in table covering the given VA at given level
*/
static uint32_t mmu_get_tbl_idx(va_t VA, uint32_t u32Level)
{
    va_t tmpVA = VA >> au32LowBits[u32Level];
    uint32_t u32Mask = 32U - au32BitsCnt[u32Level];
    
    u32Mask = 0xffffffffU >> u32Mask;
    tmpVA = tmpVA & u32Mask;

    return (uint32_t)(tmpVA);
}

/*
    @brief Assign attributes to an entry
    @param[in] prEntry The entry
    @param[in] u32Level Current translation level
    @param[in] AttrMask Attribute Mask, use the mmu_get_attr() macro to get the Mask
    @retval MMU_E_OK Success
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
static mmu_ret_t mmu_set_attributes(tbl_entry_t *const prEntry, const uint32_t u32Level, const mem_attr_t AttrMask)
{
    uint64_t u64NewAttr = 0ULL;
    uint32_t u32Mask = mmu_attr_get_attr(AttrMask);
    mem_attr_t MemAttrTmp;
    mmu_ret_t eRetVal = MMU_E_OK;
    
    switch (mmu_get_desc_type(prEntry, u32Level))
    {
        case DESC_TABLE:
            /* Zero-out all attributes */
            prEntry->u64Descriptor &= ~MATTR_TA(-1);
            u64NewAttr = 0ULL; /* NS=0, AP=0, UXN/XN=0, PXN=0. Change this when needed. */
            
            break;
        case DESC_BLOCK:
            /* Zero-out all attributes */
            prEntry->u64Descriptor &= ~MATTR_BUA(-1);
            prEntry->u64Descriptor &= ~MATTR_BLA(-1);
            
            MemAttrTmp = mmu_attr_get_type(AttrMask);
            MemAttrTmp = (mem_attr_t)MATTR_LOA_ATTRIDX(MemAttrTmp);
            MemAttrTmp |= (mem_attr_t)MATTR_LOA_AF;
            MemAttrTmp |= (mem_attr_t)MATTR_LOA_SH_INNERSHAREABLE;
            
            u64NewAttr = MATTR_BLA(MemAttrTmp);
            u64NewAttr |= (0U != (u32Mask & MA_EL2_EL3_RO)) ? MATTR_BLA(MATTR_LOA_AP_EL2EL3_RO) : 0U;
            u64NewAttr |= (0U != (u32Mask & MA_NON_EXEC)) ? MATTR_BUA(MATTR_UPA_UXN_XN) : 0U;
            
            break;
        case DESC_PAGE:
            /* Zero-out all attributes */
            prEntry->u64Descriptor &= ~MATTR_PUA(-1);
            prEntry->u64Descriptor &= ~MATTR_PLA(-1);
            
            MemAttrTmp = mmu_attr_get_type(AttrMask);
            MemAttrTmp = MATTR_LOA_ATTRIDX(MemAttrTmp);
            MemAttrTmp |= (mem_attr_t)MATTR_LOA_AF;
            MemAttrTmp |= (mem_attr_t)MATTR_LOA_SH_INNERSHAREABLE;
            
            u64NewAttr = MATTR_PLA(MemAttrTmp);
            u64NewAttr |= (0U != (u32Mask & MA_EL2_EL3_RO)) ? MATTR_PLA(MATTR_LOA_AP_EL2EL3_RO) : 0U;
            u64NewAttr |= (0U != (u32Mask & MA_NON_EXEC)) ? MATTR_PUA(MATTR_UPA_UXN_XN) : 0U;
            
            break;
        case DESC_RESERVED:
        case DESC_INVALID:
        default:
            eRetVal = MMU_E_INVALID_DESCRIPTOR;
    }
    
    prEntry->u64Descriptor |= u64NewAttr;
    
    return eRetVal;
}

/*
    @brief Install new mapping
    @param[in] VA Virtual address
    @param[in] PA Physical address
    @param[in] Size Length of mapping
    @param[in] Attr Attributes
    @param[in] prTable Initial table
    @param[in] u32Level Initial level
    @param[out] MappedLen  Length of the allocated mapping
    @retval MMU_E_OK Success
    @retval MMU_E_CONFLICT Mapping conflict (output address or attributes)
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
    @retval MMU_E_CONFIGURATION Wrong configuration
*/
static mmu_ret_t mmu_ll_add_mapping(va_t VA, pa_t PA, mlen_t Size, mem_attr_t Attr, tbl_entry_t prTable[], uint32_t u32Level, mlen_t *const MappedLen)
{
    va_t curVA = VA;  /* Current VA */
    pa_t curPA = PA;  /* Current PA */
    addr_t Mask;
    mlen_t MapLength;
    mlen_t RemainingLen = Size;
    tbl_entry_t *prNewTable, *prEntry;
    desc_type_t DescType;
    mmu_ret_t eRetVal;
    uint32_t u32ii;
    
    /*  Get state of the mapping. If not fully mapped yet determine
        starting table, prEntry and level for further processing. */
    /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
    eRetVal = mmu_ll_check_mapping(VA, PA, Size, prTable, u32Level, &MapLength);
    if (MMU_E_NOT_MAPPED == eRetVal)
    {
        do
        {
            u32ii = mmu_get_tbl_idx(VA, u32Level);
            prEntry = &prTable[u32ii];
            DescType = mmu_get_desc_type(prEntry, u32Level);
            if (DESC_TABLE == DescType)
            {
                /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
                eRetVal = mmu_desc_get_next_level_table(prEntry, u32Level, &prTable);
                u32Level++;
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
                /*  This check is redundant and is done within mmu_ll_check_mapping(). The only
                    reason of this is detect page tables corruption done after the mmu_ll_check_mapping()
                    is called (MMU_E_INVALID_DESCRIPTOR). */
                if (MMU_E_OK != eRetVal)
                {
                    /** @ violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return eRetVal;
                }
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
            }
        } while (DESC_TABLE == DescType);
    }
    else
    {
        *MappedLen = Size;
        /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
        return eRetVal;
    }
    
    while (curVA < (VA + Size))
    {
        if ((RemainingLen >= au64RegionSizes[u32Level]) || ((0U != RemainingLen) && (u32Level == (LEVELS_COUNT-1U))))
        {
            prEntry = &prTable[mmu_get_tbl_idx(curVA, u32Level)];
            prEntry->u64Descriptor = 0U;
            prEntry->common.valid = 1U;
            /*  Create a Page/Block descriptor */
            if (u32Level < (LEVELS_COUNT-1U))
            {
                prEntry->common.bpt = 0U;
            }
            else
            {
                prEntry->common.bpt = 1U;
            }
            
            /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
            eRetVal = mmu_get_mask(prEntry, u32Level, &Mask);
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
            /*  This check is redundant and is done within mmu_ll_check_mapping(). The only
                reason of this is detect page tables corruption done after the mmu_ll_check_mapping()
                is called (MMU_E_INVALID_DESCRIPTOR). */
            if (MMU_E_OK != eRetVal)
            {
                /** @ violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                return eRetVal;
            }
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
            
            prEntry->u64Descriptor |= curPA & Mask;
            MapLength = au64RegionSizes[u32Level]; /* Remember mapped size */
        }
        else
        {
            /*  Create next-level table to decrease granule size */
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
            if ((u32Level + 1U) >= LEVELS_COUNT)
            {
                /** @v iolates @ref mmu_c_REF_5 MISRA rule 14.7 */
                return MMU_E_CONFIGURATION;
            }
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
            
            u32ii = GET_TBL_SIZE(u32Level+1U);
            prNewTable = mmu_get_new_table(u32ii);
            if ((tbl_entry_t *)0ULL == prNewTable)
            {
                /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                return MMU_E_OUT_OF_MEMORY;
            }
            
            /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
            eRetVal = mmu_ll_add_mapping(curVA, curPA, RemainingLen, Attr, prNewTable, u32Level+1U, &MapLength);
            if (MMU_E_OK != eRetVal)
            {
                /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                return eRetVal;
            }
            
            prEntry = &prTable[mmu_get_tbl_idx(curVA, u32Level)];
            /** @violates @ref mmu_c_REF_6 MISRA rule 11.3 */
            prEntry->u64Descriptor = (pa_t)prNewTable & 0x0000ffffffffffffULL; /* TODO: compute or use a macro... */
            prEntry->common.valid = 1U;
            prEntry->common.bpt = 1U; /* Table descriptor */
        }
        
        mmu_set_attributes(prEntry, u32Level, Attr);
        curVA += (va_t)MapLength;
        curPA += (pa_t)MapLength;
        RemainingLen -= MapLength;
        if (0U == mmu_get_tbl_idx(curVA, u32Level))
        {
            break;  /*  Continue with 0th element of next table */
        }
    }
    
    *MappedLen = curVA - VA;
    
    return MMU_E_OK;
}

/*
    @brief Helper function to get address Mask related to entry type and level.
    @param[in] prEntry Entry to be used for Mask calculation
    @param[in] u32Level Translation level
    @param[out] pMask The calculated Mask
    @retval MMU_E_OK Success
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
static mmu_ret_t mmu_get_mask(const tbl_entry_t *const prEntry, const uint32_t u32Level, addr_t *const pMask)
{
    addr_t Mask = 0U;
    mmu_ret_t eRetVal = MMU_E_OK;
    
    switch (mmu_get_desc_type(prEntry, u32Level))
    {
        case DESC_BLOCK:
            Mask = ((addr_t)(-1)) << au32LowBits[u32Level];
            Mask &= ((addr_t)(-1)) >> (64U - MMU_PA_SIZE);
            break;
        case DESC_TABLE:
            /*  With the 4KB granule size m is 12, with the 16KB granule size m is 14, and
                with the 64KB granule size, m is 16. Corresponds to L3 output length. */
        case DESC_PAGE:
            Mask = ((addr_t)(-1)) << au32LowBits[3];
            Mask &= ((addr_t)(-1)) >> (64U - MMU_PA_SIZE);
            break;
        case DESC_RESERVED:
        case DESC_INVALID:
        default:
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
            /*  Runtime sanity check */
            eRetVal = MMU_E_INVALID_DESCRIPTOR;
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
            break;
    }
    
    *pMask = Mask;
    return eRetVal;
}

/*
    @brief Allocate new table
    @param[in] u32Size Number of entries
    @retval !NULL Pointer to the allocated table
    @retval NULL Error
*/
static tbl_entry_t *mmu_get_new_table(uint32_t u32Size)
{
    tbl_entry_t *prRetVal = &arTables[au32CurrentTblidx];
    
    if ((au32CurrentTblidx + u32Size) > MMU_CFG_ENTRY_POOL_SIZE)
    {
        prRetVal = (tbl_entry_t *)0ULL; /* Out of memory */
    }
    
    au32CurrentTblidx += u32Size;
    
    return prRetVal;
}

/*
    @brief The descriptor type parser
    @details Based on entry and the translation level determines type of the entry
    @param[in] prEntry Descriptor to be parsed
    @param[in] u32Level Translation level within the descriptor exists
    @retval DESC_TABLE Table descriptor
    @retval DESC_BLOCK Block descriptor
    @retval DESC_PAGE Page descriptor
    @retval DESC_RESERVED Reserved
    @retval DESC_INVALID Invalid
*/
static desc_type_t mmu_get_desc_type(const tbl_entry_t *const prEntry, const uint32_t u32Level)
{
    desc_type_t tRetVal;
    
    if ((u32Level < 3U) && (0U != prEntry->common.valid))
    {
        if (prEntry->common.bpt)
        {
            tRetVal = DESC_TABLE;
        }
        else
        {
            tRetVal = DESC_BLOCK;
        }
    }
    else if ((u32Level == 3U) && (0U != prEntry->common.valid))
    {
        if (prEntry->common.valid)
        {
            tRetVal = DESC_PAGE;
        }
        else
        {
            tRetVal = DESC_RESERVED;
        }
    }
    else
    {
        tRetVal = DESC_INVALID;
    }
    
    return tRetVal;
}

/*
    @brief The output address parser
    @details Based on entry and the translation level determines output address or next-level
             table address respectively.
    @param[in] prEntry Descriptor to be parsed
    @param[in] u32Level Translation level within the descriptor exists
    @param[out] pOutAddr Pointer to where bit-Mask specifying output address at current level
                will be written
    @retval MMU_E_OK Success
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
static mmu_ret_t mmu_desc_get_output_address(const tbl_entry_t *const prEntry, const uint32_t u32Level, pa_t *pOutAddr)
{
    addr_t Mask = 0U;
    mmu_ret_t eRetVal;
    
    /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
    eRetVal = mmu_get_mask(prEntry, u32Level, &Mask);
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
    if (MMU_E_OK == eRetVal)
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
    {
        *pOutAddr = (pa_t)(prEntry->u64Descriptor & Mask);  /*  Return the output address / next table pointer */
    }
    
    return eRetVal;
}

/*
    @brief The output address setter
    @param[in] prEntry Descriptor to be updated
    @param[in] u32Level Translation level within the descriptor exists
    @param[in] PA The new output address to be set
    @retval MMU_E_OK Success
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
#if 0
static mmu_ret_t mmu_desc_set_output_address(tbl_entry_t *const prEntry, const uint32_t u32Level, const pa_t PA)
{
    addr_t Mask;
    mmu_ret_t eRetVal;
    
    /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
    eRetVal = mmu_get_mask(prEntry, u32Level, &Mask);
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
    if (MMU_E_OK == eRetVal)
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
    {
        prEntry->u64Descriptor |= PA & Mask; /*  Store upper "n" bits of the PA or upper "m" bits of next-level table address */
    }
    
    return eRetVal;
}
#endif /* 0 */

/*
    @brief Returns next-level table pointer
    @details Just a wrapper for the mmu_desc_get_output_address()
*/
static mmu_ret_t mmu_desc_get_next_level_table(const tbl_entry_t *const prEntry, const uint32_t u32Level, tbl_entry_t **pprNextTable)
{
    pa_t outAddr;
    mmu_ret_t eRetVal;
    
    /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
    eRetVal = mmu_desc_get_output_address(prEntry, u32Level, &outAddr);
    /** @violates @ref mmu_c_REF_7 MISRA rule 11.3 */
    *pprNextTable = (tbl_entry_t *)outAddr;
    
    return eRetVal;
}

/*
    @brief Helper function validating VA
    @retval TRUE The given VA is valid
    @retval FALSE The given VA is not valid
*/
static boolean_t mmu_is_ia_valid(const va_t VA) {
    /** @violates @ref mmu_c_REF_8 MISRA rule 12.11 */
    addr_t Mask = ((addr_t)(-1)) << MMU_VA_SIZE;
    boolean_t bRetVal = (0U != (VA & Mask)) ? FALSE : TRUE;
    
    return bRetVal;
}

/*
    @brief Verify that the requested mapping exists
    @param[in] VA Start of the input range to be checked
    @param[in] PA Output range start
    @param[in] Size Size of the range
    @param[in] prTable Current table
    @param[in] u32Level Current translation level
    @retval MMU_E_OK Given VA range already mapped to the given PA range
    @retval MMU_E_NOT_MAPPED Range not mapped yet
    @retval MMU_E_CONFLICT Given VA or a part of the given VA range mapped to different PA range
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
static mmu_ret_t mmu_ll_check_mapping(va_t VA, pa_t PA, mlen_t Size, const tbl_entry_t prTable[], uint32_t u32Level, mlen_t *const CheckedLen)
{
    pa_t OutAddr;
    addr_t Mask;
    mlen_t Remaining, CheckedLength;
    tbl_entry_t descriptor;
    tbl_entry_t *prNextTable;
    mmu_ret_t eRetVal;
    mmu_ret_t eStatus = MMU_E_OK;
    
    if (FALSE == mmu_is_ia_valid(VA))
    {
        /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
        return MMU_E_INVALID_ADDRESS;
    }
    
    Remaining = Size;
    
    while (Remaining)
    {
        CheckedLength = (mlen_t)(au64RegionSizes[u32Level]);
        descriptor = prTable[mmu_get_tbl_idx(VA, u32Level)]; /* Get descriptor from current table */
        /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
        switch (mmu_get_desc_type(&descriptor, u32Level))
        {
            case DESC_BLOCK:
            case DESC_PAGE:
                /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
                eRetVal = mmu_desc_get_output_address(&descriptor, u32Level, &OutAddr);
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
                if (MMU_E_OK != eRetVal)
                {
                    /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return eRetVal;
                }
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
                
                /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
                eRetVal = mmu_get_mask(&descriptor, u32Level, &Mask);
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
                if (MMU_E_OK != eRetVal)
                {
                    /** @ violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return eRetVal;
                }
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
                
                if (OutAddr ^ (PA & Mask)) /* Output address check */
                {
                    /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return MMU_E_CONFLICT;
                }
                
                /*  TODO: Attributes check */
                
                break;
            case DESC_TABLE:
                /*  Walk through next-level table */
                /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
                eRetVal = mmu_desc_get_next_level_table(&descriptor, u32Level, &prNextTable);
                if (MMU_E_OK != eRetVal)
                {
                    /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return eRetVal;
                }
                
                /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
                eStatus = mmu_ll_check_mapping(VA, PA, Remaining, prNextTable, u32Level+1U, &CheckedLength);
                if (MMU_E_CONFLICT == eStatus)
                {
                    /** @violates @ref mmu_c_REF_5 MISRA rule 14.7 */
                    return eStatus;
                }
                
                break;
            case DESC_RESERVED:
            case DESC_INVALID:
            default:
                eStatus = MMU_E_NOT_MAPPED;    /*  Range not (fully) mapped yet */
        }
        
        Remaining -= CheckedLength;
        VA += (va_t)CheckedLength;
        PA += (pa_t)CheckedLength;
        if (0U == mmu_get_tbl_idx(VA, u32Level))
        {
            break;  /*  Continue with 0th element of next table */
        }
    }
    
    *CheckedLen = Size - Remaining;
    
    return eStatus;
}

/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/
/**
    @brief Initialization function
    @retval MMU_E_OK Success
    @retval MMU_E_SYSTEM System error
    @retval MMU_E_CONFIGURATION Wrong configuration
*/
mmu_ret_t mmu_init(void)
{
    uint32_t u32ii = 0U;
    register uint32_t u32RegVal;
    mmu_ret_t eRetVal = MMU_E_OK;
    
    __asm("MRS %0, CurrentEL;" : "=r"(u32RegVal));
    /** @violates @ref mmu_c_REF_9 MISRA rule 9.1 */
    if ((u32RegVal >> 2) != 0x3U)
    {
        eRetVal = MMU_E_SYSTEM;
    }
    /*  Check whether configured count of entries within pool is a multiple of
        the biggest table in the system (the deepest one). */
    else if (0U != (MMU_CFG_ENTRY_POOL_SIZE % (1UL << au32BitsCnt[LEVELS_COUNT-1U])))
    {
        eRetVal = MMU_E_CONFIGURATION;
    }
    else if (0 != mmu_cfg_preconfigure())
    {
        eRetVal = MMU_E_CONFIGURATION;
    }
    else
    {
        for (u32ii=0U; u32ii<MMU_INIT_TABLE_SIZE; u32ii++)
        {
            START_TABLE[u32ii].u64Descriptor = 0x0ULL;
        }
        
        for (u32ii=0U; u32ii<MMU_CFG_ENTRY_POOL_SIZE; u32ii++)
        {
            arTables[u32ii].u64Descriptor = 0x0ULL;
        }
        
        bInitialized = TRUE;
    }
    
    au32CurrentTblidx = 0U;
    return eRetVal;
}

/**
    @brief Get mapping region size at given translation level
    @param[in] u32Level The desired translation level
    @param[out] RegionSize Pointer to memory where the size will be written
    @retval MMU_E_OK Success
    @retval MMU_E_INVALID_PARAMETER Invalid input parameter
*/
mmu_ret_t mmu_get_region_size(const uint32_t u32Level, mlen_t *const RegionSize)
{
    mmu_ret_t eRetVal = MMU_E_OK;
    
    if (u32Level < LEVELS_COUNT)
    {
        *RegionSize = (mlen_t)(au64RegionSizes[u32Level]);
    }
    else
    {
        eRetVal = MMU_E_INVALID_PARAMETER;
    }
    
    return eRetVal;
}

/**
    @brief API to check status of a mapping
    @param[in] VA Start of the input range to be checked
    @param[in] PA Output range start
    @param[in] Size Size of the range
    @retval MMU_E_OK Given VA range already mapped to the given PA range
    @retval MMU_E_NOT_MAPPED Range not mapped yet
    @retval MMU_E_CONFLICT Given VA range or a part of the given VA range is mapped to different PA range
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
*/
mmu_ret_t mmu_check_mapping(const va_t VA, const pa_t PA, const mlen_t Size)
{
    mlen_t CheckedLength;
    mmu_ret_t eRetVal = MMU_E_OK;
    
    if (FALSE == bInitialized)
    {
        eRetVal = MMU_E_NOT_INITIALIZED;
    }
    else
    {
        /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
        eRetVal = mmu_ll_check_mapping(VA, PA, Size, START_TABLE, START_LEVEL, &CheckedLength);
    }
    
    return eRetVal;
}

/**
    @brief API to install new mapping
    @param[in] VA Virtual address
    @param[in] PA Physical address
    @param[in] Size Length of mapping
    @param[in] Attr Attributes
    @retval MMU_E_OK Success
    @retval MMU_E_CONFLICT Mapping conflict (output address or attributes)
    @retval MMU_E_INVALID_DESCRIPTOR Invalid descriptor detected
    @retval MMU_E_CONFIGURATION Wrong configuration
*/
mmu_ret_t mmu_add_mapping(const va_t VA, const pa_t PA, const mlen_t Size, const mem_attr_t Attr)
{
    mlen_t mlen, asize;
    addr_t Mask;
    va_t aVA;
    pa_t aPA;
    mmu_ret_t eRetVal;
    
    /*  Adjust inputs to match minimum granule */
    Mask = ((addr_t)(-1)) << au32LowBits[LEVELS_COUNT-1U];
    aPA = PA & (pa_t)Mask;
    aVA = VA & (va_t)Mask;
    asize = Size + (VA - aVA);
    
    /*  Round the size up to the nearest multiple of minimum granule */
    asize = ((asize + au64RegionSizes[LEVELS_COUNT-1U]) - 1U);
    asize -= asize % au64RegionSizes[LEVELS_COUNT-1U];
    
    /*  Checks */
    if (FALSE == bInitialized)
    {
        eRetVal = MMU_E_NOT_INITIALIZED;
    }
    else if (0ULL == Size)
    {
        eRetVal = MMU_E_OK;
    }
    else if (FALSE == mmu_is_ia_valid(aVA))
    {
        eRetVal = MMU_E_INVALID_ADDRESS;
    }
    else if (FALSE == mmu_is_ia_valid((aVA + Size) - 1U))
    {
        eRetVal = MMU_E_INVALID_ADDRESS;
    }
    else
    {
        do
        {
            /** @violates @ref mmu_c_REF_4 MISRA rule 1.2 */
            eRetVal = mmu_ll_add_mapping(aVA, aPA, asize, Attr, START_TABLE, START_LEVEL, &mlen);
            aVA += (va_t)mlen;
            aPA += (pa_t)mlen;
#if (TRUE == MMU_CFG_DEV_ERROR_DETECTION)
            if (mlen > asize)
            {
                eRetVal = MMU_E_SIZE_MISMATCH; /*  This should never happen */
                break;
            }
            else
#endif /* MMU_CFG_DEV_ERROR_DETECTION */
            {
                asize -= mlen;
            }
        } while ((eRetVal == MMU_E_OK) && (asize > 0U));
    }
    
    return eRetVal;
}

/**
    @brief Start MMU within the EL3
    @retval MMU_E_OK Success
    @retval MMU_E_NOT_INITIALIZED Not initialized yet
*/
mmu_ret_t mmu_start(void)
{
    mmu_ret_t eRetVal = MMU_E_OK;
    register uint32_t u32RegVal;
    register uint64_t u64RegVal;
    
    if (FALSE == bInitialized)
    {
        eRetVal = MMU_E_NOT_INITIALIZED;
    }
    else
    {
        /*  1.) Configure TCR_EL3 */
        u32RegVal =  0U                             \
            | TCR_T0SZ(64U - MMU_VA_SIZE)         \
            | TCR_IRGN0(TCR_IRGN0_NORMAL_IWBWAC)  \
            | TCR_ORGN0(TCR_ORGN0_NORMAL_OWBWAC)  \
            | TCR_SH0(TCR_SH0_INNER_SHAREABLE)    \
            | TCR_TG0(TCR_TG0_4KB)                \
            | TCR_PS(TCR_PS_4GB);
        
        __asm("msr tcr_el3, %0" : : "r"(u32RegVal));
        
        /*  2.) Configure TTBR0_EL3 */
        /** @violates @ref mmu_c_REF_6 MISRA rule 11.3 */
        u64RegVal = (uint64_t)(START_TABLE);
        __asm("msr ttbr0_el3, %0" : : "r"(u64RegVal));
        __asm("dsb sy");
        __asm("isb");
        
        /*  3.) Ensure correct access in DACR32_EL2 */
        /*  TODO: Do we need this? */
        
        /*  4.) Ensure MMU is enabled in SCTLR_EL3 */
        __asm("mrs %0, sctlr_el3" : "=r"(u32RegVal));
        /*  Enable the MMU, Instruction cache and data cache */
        __asm("msr sctlr_el3, %0" : : "r"(u32RegVal | SCTLR_M | SCTLR_I | SCTLR_C));
        __asm("isb"); /* Flush the instruction pipeline */
        
        /*  5.) Invalidate the TLB */
        __asm("dsb sy");
        __asm("tlbi alle3"); /*  Invalidate all at EL3 */
        __asm("dsb sy");
        __asm("isb");
    }
    
    return eRetVal;
}

/**
    @brief Clean data cache by address to Point of Coherency
    @param[in] VA Start of the virtual address range to be processed
    @param[in] length Length of the range
*/
void cache_d_clean_by_va_range(va_t VA, mlen_t length)
{
    register uint64_t u64RegVal;
    uint32_t u32Mls;
    
    __asm("mrs %0, CTR_EL0" : "=r"(u64RegVal));
    /** @violates @ref mmu_c_REF_9 MISRA rule 9.1 */
    u32Mls = (uint32_t)(u64RegVal >> 16);
    u32Mls = (uint32_t)(2UL << (u32Mls & 0xfU));  /* Min line size in B */
    VA &= ~(((va_t)u32Mls - 1U)); /* Align VA to mls */
    u64RegVal = VA;
    for (u64RegVal = VA; u64RegVal < (VA + length); u64RegVal += u32Mls)
    {
        __asm("dc cvac, %0" : : "r"(u64RegVal));
    }
}

/**
    @brief Disable MMU within the EL3
    @brief Also disables caches
    @retval MMU_E_OK Success
*/
mmu_ret_t mmu_stop(void)
{
    register uint32_t u32RegVal;
    
    __asm("dsb sy");
    __asm("mrs %0, sctlr_el3" : "=r"(u32RegVal));
    __asm("msr sctlr_el3, %0" : : "r"(u32RegVal & ~SCTLR_M & ~SCTLR_I & ~SCTLR_C));
    __asm("dsb sy");
    __asm("isb");
    
    return MMU_E_OK;
}
/** @}*/
