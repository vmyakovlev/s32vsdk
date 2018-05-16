
typedef enum  {
	MT_DEVICE	= 0 << 0,
	MT_MEMORY	= 1 << 0,

	MT_RO		= 0 << 1,
	MT_RW		= 1 << 1,

	MT_SECURE	= 0 << 2,
	MT_NS		= 1 << 2,
	
	MT_NONCACHED = 1 << 3
} mmap_attr_t;

typedef struct mmap_region {
	unsigned long	base_pa;
	unsigned long	base_va;
	unsigned long	size;
	mmap_attr_t	attr;
} mmap_region_t;

typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef	unsigned long size_t;

#define MAX_MMAP_REGIONS		32
#define MAX_XLAT_TABLES			2

#define XLAT_TABLE_SIZE_SHIFT	PAGE_SIZE_SHIFT
#define XLAT_TABLE_SIZE		(1 << XLAT_TABLE_SIZE_SHIFT)

#define ADDR_MASK_48_TO_63	0xFFFF000000000000UL
#define ADDR_MASK_44_TO_47	0x0000F00000000000UL
#define ADDR_MASK_42_TO_43	0x00000C0000000000UL
#define ADDR_MASK_40_TO_41	0x0000030000000000UL
#define ADDR_MASK_36_TO_39	0x000000F000000000UL
#define ADDR_MASK_32_TO_35	0x0000000F00000000UL

/* (internal) physical address size bits in EL3/EL1 */
#define TCR_PS_BITS_4GB		(0x0)
#define TCR_PS_BITS_64GB	(0x1)
#define TCR_PS_BITS_1TB		(0x2)
#define TCR_PS_BITS_4TB		(0x3)
#define TCR_PS_BITS_16TB	(0x4)
#define TCR_PS_BITS_256TB	(0x5)

#define TCR_RGN_INNER_NC	(0x0 << 8)
#define TCR_RGN_INNER_WBA	(0x1 << 8)
#define TCR_RGN_INNER_WT	(0x2 << 8)
#define TCR_RGN_INNER_WBNA	(0x3 << 8)

#define TCR_RGN_OUTER_NC	(0x0 << 10)
#define TCR_RGN_OUTER_WBA	(0x1 << 10)
#define TCR_RGN_OUTER_WT	(0x2 << 10)
#define TCR_RGN_OUTER_WBNA	(0x3 << 10)

#define TCR_SH_NON_SHAREABLE	(0x0 << 12)
#define TCR_SH_OUTER_SHAREABLE	(0x2 << 12)
#define TCR_SH_INNER_SHAREABLE	(0x3 << 12)

#define TCR_EL3_RES1		((1UL << 31) | (1UL << 23))
#define TCR_EL1_IPS_SHIFT	32
#define TCR_EL3_PS_SHIFT	16

#define FOUR_KB_SHIFT		12
#define PAGE_SIZE_SHIFT		FOUR_KB_SHIFT
#define PAGE_SIZE		(1 << PAGE_SIZE_SHIFT)
#define PAGE_SIZE_MASK		(PAGE_SIZE - 1)
#define IS_PAGE_ALIGNED(addr)	(((addr) & PAGE_SIZE_MASK) == 0)

#define XLAT_ENTRY_SIZE_SHIFT	3 /* Each MMU table entry is 8 bytes (1 << 3) */

#define XLAT_TABLE_SIZE_SHIFT	PAGE_SIZE_SHIFT

#define XLAT_TABLE_ENTRIES_SHIFT (XLAT_TABLE_SIZE_SHIFT - XLAT_ENTRY_SIZE_SHIFT)

#define L3_XLAT_ADDRESS_SHIFT	PAGE_SIZE_SHIFT
#define L2_XLAT_ADDRESS_SHIFT	(L3_XLAT_ADDRESS_SHIFT + XLAT_TABLE_ENTRIES_SHIFT)
#define L1_XLAT_ADDRESS_SHIFT	(L2_XLAT_ADDRESS_SHIFT + XLAT_TABLE_ENTRIES_SHIFT)

#define XLAT_TABLE_ENTRIES	(1 << XLAT_TABLE_ENTRIES_SHIFT)
#define XLAT_TABLE_ENTRIES_MASK	(XLAT_TABLE_ENTRIES - 1) 

#define SCTLR_M_BIT		(1 << 0)
#define SCTLR_A_BIT		(1 << 1)
#define SCTLR_C_BIT		(1 << 2)
#define SCTLR_SA_BIT		(1 << 3)
#define SCTLR_B_BIT		(1 << 7)
#define SCTLR_Z_BIT		(1 << 11)
#define SCTLR_I_BIT		(1 << 12)
#define SCTLR_WXN_BIT		(1 << 19)
#define SCTLR_EXCEPTION_BITS	(0x3 << 6)
#define SCTLR_EE_BIT		(1 << 25) 

#define XN			(1ull << 2) 

#define DEFINE_SYSOP_TYPE_FUNC(_op, _type)		\
static inline void _op ## _type(void)			\
{							\
	__asm__ (#_op " " #_type);			\
}

#define DEFINE_SYSOP_FUNC(_op)				\
static inline void _op(void)				\
{							\
	__asm__ (#_op);					\
}


#define _DEFINE_SYSREG_READ_FUNC(_name, _reg_name)	\
static inline uint64_t read_ ## _name(void)		\
{							\
	uint64_t v;					\
	__asm__ ("mrs %0, " #_reg_name : "=r" (v));	\
	return v;					\
}



#define _DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)		\
static inline void write_ ## _name(uint64_t v)		\
{							\
	__asm__ ("msr " #_reg_name ", %0" : : "r" (v));	\
}

#define _DEFINE_SYSREG_WRITE_CONST_FUNC(_name, _reg_name)		\
static inline void write_ ## _name(const uint64_t v)	\
{							\
	__asm__ ("msr " #_reg_name ", %0" : : "i" (v));	\
}

/* Define read function for system register */
#define DEFINE_SYSREG_READ_FUNC(_name) 			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)

/* Define read & write function for system register */
#define DEFINE_SYSREG_RW_FUNCS(_name)			\
	_DEFINE_SYSREG_READ_FUNC(_name, _name)		\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _name)

/* Define read & write function for renamed system register */
#define DEFINE_RENAME_SYSREG_RW_FUNCS(_name, _reg_name)	\
	_DEFINE_SYSREG_READ_FUNC(_name, _reg_name)	\
	_DEFINE_SYSREG_WRITE_FUNC(_name, _reg_name)

/* Define write function for special system registers */
#define DEFINE_SYSREG_WRITE_CONST_FUNC(_name)		\
	_DEFINE_SYSREG_WRITE_CONST_FUNC(_name, _name)


//DEFINE_SYSOP_TYPE_FUNC(dsb, sy)
//DEFINE_SYSOP_FUNC(isb)
//#define dsb()			dsbsy() 

//DEFINE_SYSOP_TYPE_FUNC(tlbi, alle3)
//DEFINE_SYSREG_RW_FUNCS(ttbr0_el3)
//DEFINE_SYSREG_RW_FUNCS(mair_el3)
//DEFINE_SYSREG_RW_FUNCS(sctlr_el3)
//DEFINE_SYSREG_RW_FUNCS(tcr_el3)


#define NS				(0x1 << 3)
#define ATTR_MEMNC_INDEX        0x3
#define ATTR_SO_INDEX			0x2
#define ATTR_DEVICE_INDEX		0x1
#define ATTR_IWBWA_OWBWA_NTR_INDEX	0x0
#define LOWER_ATTRS(x)			(((x) & 0xfff) << 2)
#define ATTR_SO				(0x0)
#define ATTR_DEVICE			(0x4)
#define ATTR_IWBWA_OWBWA_NTR		(0xff)
#define ATTR_MEMNC          (0x44)
#define MAIR_ATTR_SET(attr, index)	(attr << (index << 3)) 

#define TCR_SH_NON_SHAREABLE	(0x0 << 12)
#define TCR_SH_OUTER_SHAREABLE	(0x2 << 12)
#define TCR_SH_INNER_SHAREABLE	(0x3 << 12) 

#define ADDR_SPACE_SIZE			(1ull << 32)

#define NUM_L1_ENTRIES (ADDR_SPACE_SIZE >> L1_XLAT_ADDRESS_SHIFT) 


#define	__aligned(x)	__attribute__((__aligned__(x)))
#define LOWER_ATTRS(x)			(((x) & 0xfff) << 2)
#define AP_RO			(0x1 << 5)
#define AP_RW			(0x0 << 5)
#define UPPER_ATTRS(x)		(x & 0x7) << 52
#define NON_GLOBAL		(1 << 9)
#define ACCESS_FLAG		(1 << 8)
#define NSH			(0x0 << 6)
#define OSH			(0x2 << 6)
#define ISH			(0x3 << 6) 

#define INVALID_DESC		0x0
#define BLOCK_DESC		0x1
#define TABLE_DESC		0x3 
#define UNSET_DESC	~0ul
