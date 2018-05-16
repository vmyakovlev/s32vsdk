/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef MMAP_H
#define MMAP_H


typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef	uint64_t size_t;
typedef unsigned long uintptr_t;
typedef uint64_t u_register_t;

#define MAX_XLAT_TABLES 24
#define MAX_MMAP_REGIONS 32
#define	EPERM		1		/* Operation not permitted */
#define	EINVAL		22		/* Invalid argument */
#define	ERANGE		34		/* Result too large */
#define	ENOMEM		12		/* Cannot allocate memory */

#define MT_TYPE_MASK		0x7
#define MT_TYPE(_attr)		((_attr) & MT_TYPE_MASK)
#define MT_PERM_SHIFT		3
#define MT_SEC_SHIFT		4
#define MT_EXECUTE_SHIFT	5

#define MAIR_ATTR_SET(attr, index)	((attr) << ((index) << 3))
#define ATTR_NON_CACHEABLE_INDEX	0x2
#define ATTR_NON_CACHEABLE	        0x44
#define ATTR_DEVICE_INDEX	        0x1
#define ATTR_DEVICE			        0x4
#define ATTR_IWBWA_OWBWA_NTR_INDEX	0x0
#define ATTR_IWBWA_OWBWA_NTR	    0xff
#define DISABLE_DCACHE			(1 << 0)

#define TCR_RGN_INNER_NC	(0x0 << 8)
#define TCR_RGN_OUTER_NC	(0x0 << 10)
#define TCR_SH_NON_SHAREABLE	(0x0 << 12)
#define TCR_RGN_INNER_WBA	(0x1 << 8)
#define TCR_RGN_OUTER_WBA	(0x1 << 10)
#define SCTLR_WXN_BIT		(1 << 19)
#define SCTLR_I_BIT		(1 << 12)
#define SCTLR_M_BIT		(1 << 0)
#define SCTLR_C_BIT		(1 << 2)
#define TCR_SH_INNER_SHAREABLE	(0x3 << 12)
#define XLAT_TABLE_NC			(1 << 1)

#define PLAT_PHY_ADDR_SPACE_SIZE (1UL << 32)
#define PLAT_VIRT_ADDR_SPACE_SIZE (1UL << 34)

#define TCR_TxSZ_MIN		16
#define TCR_TxSZ_MAX		39

#define INVALID_DESC	0x0
#define BLOCK_DESC		0x1 /* Table levels 0-2 */
#define TABLE_DESC		0x3 /* Table levels 0-2 */
#define PAGE_DESC		0x3 /* Table level 3 */
#define DESC_MASK		0x3
#define PLAT_XLAT_TABLES_DYNAMIC 1
#define PAGE_SIZE_SHIFT (12)
#define PAGE_SIZE		(1 << PAGE_SIZE_SHIFT)
#define PAGE_SIZE_MASK		(PAGE_SIZE - 1)
#define IS_PAGE_ALIGNED(addr)	(((addr) & PAGE_SIZE_MASK) == 0)
#define TABLE_ADDR_MASK		(0x0000FFFFFFFFF000)
#define MT_DYN_SHIFT		30 
typedef enum  {
	/*
	 * Regions mapped before the MMU can't be unmapped dynamically (they are
	 * static) and regions mapped with MMU enabled can be unmapped. This
	 * behaviour can't be overridden.
	 *
	 * Static regions can overlap each other, dynamic regions can't.
	 */
	MT_STATIC	= 0 << MT_DYN_SHIFT,
	MT_DYNAMIC	= 1 << MT_DYN_SHIFT
} mmap_priv_attr_t;

#define NS				((0x1) << 3)
#define AP_RO				((0x1) << 5)
#define AP_RW				((0x0) << 5)
#define ACCESS_FLAG		((1) << 8)
#define LOWER_ATTRS(x)			(((x) & (0xfff)) << 2)
#define OSH			((0x2) << 6)
#define ISH			((0x3) << 6)


#define XLAT_TABLE_SIZE_SHIFT	PAGE_SIZE_SHIFT
#define XLAT_TABLE_SIZE		(1 << XLAT_TABLE_SIZE_SHIFT)
#define XLAT_ENTRY_SIZE_SHIFT	3 
#define XLAT_ENTRY_SIZE		(1 << XLAT_ENTRY_SIZE_SHIFT)
#define XLAT_TABLE_ENTRIES_SHIFT (XLAT_TABLE_SIZE_SHIFT - XLAT_ENTRY_SIZE_SHIFT)

#define XLAT_TABLE_LEVEL_MAX	3
#define L3_XLAT_ADDRESS_SHIFT	(PAGE_SIZE_SHIFT)
#define L2_XLAT_ADDRESS_SHIFT	(L3_XLAT_ADDRESS_SHIFT + XLAT_TABLE_ENTRIES_SHIFT)
#define L1_XLAT_ADDRESS_SHIFT	(L2_XLAT_ADDRESS_SHIFT + XLAT_TABLE_ENTRIES_SHIFT)
#define L0_XLAT_ADDRESS_SHIFT	(L1_XLAT_ADDRESS_SHIFT + XLAT_TABLE_ENTRIES_SHIFT)
# define MIN_LVL_BLOCK_DESC 1
#define XLAT_ADDR_SHIFT(level)	(PAGE_SIZE_SHIFT + \
		  ((XLAT_TABLE_LEVEL_MAX - (level)) * XLAT_TABLE_ENTRIES_SHIFT))

#define XLAT_BLOCK_SIZE(level)	((u_register_t)1 << XLAT_ADDR_SHIFT(level))
#define XLAT_BLOCK_MASK(level)	(XLAT_BLOCK_SIZE(level) - 1)

#if PLAT_VIRT_ADDR_SPACE_SIZE > (1 << (64 - TCR_TxSZ_MIN))

# error "PLAT_VIRT_ADDR_SPACE_SIZE is too big."

#elif PLAT_VIRT_ADDR_SPACE_SIZE > (1 << L0_XLAT_ADDRESS_SHIFT)

# define XLAT_TABLE_LEVEL_BASE	0
# define NUM_BASE_LEVEL_ENTRIES	(PLAT_VIRT_ADDR_SPACE_SIZE >> L0_XLAT_ADDRESS_SHIFT)
        
#elif PLAT_VIRT_ADDR_SPACE_SIZE > (1 << L1_XLAT_ADDRESS_SHIFT)

# define XLAT_TABLE_LEVEL_BASE	1
# define NUM_BASE_LEVEL_ENTRIES	(PLAT_VIRT_ADDR_SPACE_SIZE >> L1_XLAT_ADDRESS_SHIFT)
        
#elif PLAT_VIRT_ADDR_SPACE_SIZE >= (1 << (64 - TCR_TxSZ_MAX))

# define XLAT_TABLE_LEVEL_BASE	2
# define NUM_BASE_LEVEL_ENTRIES	(PLAT_VIRT_ADDR_SPACE_SIZE >> L2_XLAT_ADDRESS_SHIFT)
        
#else
# error "PLAT_VIRT_ADDR_SPACE_SIZE is too small."
#endif

#define XLAT_TABLE_ENTRIES	(1 << XLAT_TABLE_ENTRIES_SHIFT)

/*
 * Memory mapping attributes
 */
typedef enum  {
	/*
	 * Memory types supported.
	 * These are organised so that, going down the list, the memory types
	 * are getting weaker; conversely going up the list the memory types are
	 * getting stronger.
	 */
	MT_DEVICE,
	MT_NON_CACHEABLE,
	MT_MEMORY,
	/* Values up to 7 are reserved to add new memory types in the future */

	MT_RO		= 0 << MT_PERM_SHIFT,
	MT_RW		= 1 << MT_PERM_SHIFT,

	MT_SECURE	= 0 << MT_SEC_SHIFT,
	MT_NS		= 1 << MT_SEC_SHIFT,

	/*
	 * Access permissions for instruction execution are only relevant for
	 * normal read-only memory, i.e. MT_MEMORY | MT_RO. They are ignored
	 * (and potentially overridden) otherwise:
	 *  - Device memory is always marked as execute-never.
	 *  - Read-write normal memory is always marked as execute-never.
	 */
	MT_EXECUTE		= 0 << MT_EXECUTE_SHIFT,
	MT_EXECUTE_NEVER	= 1 << MT_EXECUTE_SHIFT,
} mmap_attr_t;

/*
 * Structure for specifying a single region of memory.
 */
typedef struct mmap_region {
	uint64_t	base_pa;
	uintptr_t		base_va;
	size_t			size;
	mmap_attr_t		attr;
} mmap_region_t;

/* Struct that holds all information about the translation tables. */
typedef struct {

	/*
	 * Max allowed Virtual and Physical Addresses.
	 */
	uint64_t pa_max_address;
	uintptr_t va_max_address;

	/*
	 * Array of all memory regions stored in order of ascending end address
	 * and ascending size to simplify the code that allows overlapping
	 * regions. The list is terminated by the first entry with size == 0.
	 * The max size of the list is stored in `mmap_num`. `mmap` points to an
	 * array of mmap_num + 1 elements, so that there is space for the final
	 * null entry.
	 */
	mmap_region_t *mmap;
	unsigned int mmap_num;

	/*
	 * Array of finer-grain translation tables.
	 * For example, if the initial lookup level is 1 then this array would
	 * contain both level-2 and level-3 entries.
	 */
	uint64_t (*tables)[XLAT_TABLE_ENTRIES];
	unsigned int tables_num;
	/*
	 * Keep track of how many regions are mapped in each table. The base
	 * table can't be unmapped so it isn't needed to keep track of it.
	 */
#if PLAT_XLAT_TABLES_DYNAMIC
	int *tables_mapped_regions;
#endif /* PLAT_XLAT_TABLES_DYNAMIC */

	unsigned int next_table;

	/*
	 * Base translation table. It doesn't need to have the same amount of
	 * entries as the ones used for other levels.
	 */
	uint64_t *base_table;
	unsigned int base_table_entries;

	/*
	 * Max Physical and Virtual addresses currently in use by the
	 * translation tables. These might get updated as we map/unmap memory
	 * regions but they will never go beyond pa/va_max_address.
	 */
	uint64_t max_pa;
	uintptr_t max_va;

	/* Level of the base translation table. */
	unsigned int base_level;

	/* Set to 1 when the translation tables are initialized. */
	unsigned int initialized;

	/*
	 * Bit mask that has to be ORed to the rest of a translation table
	 * descriptor in order to prohibit execution of code at the exception
	 * level of this translation context.
	 */
	uint64_t execute_never_mask;

} xlat_ctx_t;




#endif /* MMAP_H */
