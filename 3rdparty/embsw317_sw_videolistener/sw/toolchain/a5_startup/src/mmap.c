#include "mmap.h"

static mmap_region_t mmap[MAX_MMAP_REGIONS + 1]; 

static unsigned long l1_xlation_table[NUM_L1_ENTRIES]
__aligned(NUM_L1_ENTRIES * sizeof(unsigned long));

static uint64_t xlat_tables[MAX_XLAT_TABLES][XLAT_TABLE_ENTRIES]
__aligned(XLAT_TABLE_SIZE) __attribute__((section(".xlat_table"))); 


static unsigned next_xlat = 0;
static unsigned long max_pa = 0;
static unsigned long max_va = 0;
static unsigned long tcr_ps_bits = 0;

void write_ttbr0_el3(uint64_t);
void write_tcr_el3(uint64_t);
void write_sctlr_el3(uint64_t);
void write_mair_el3(uint64_t);
void tlbialle3(void);
uint64_t read_sctlr_el3(void);
void flush_dcache_range(void *, uint64_t);
void flush_icache_range(void *, uint64_t);

void *memcpy(void *dst, const void *src, size_t len)
{
	const char *s = src;
	char *d = dst;

	while (len--)
		*d++ = *s++;

	return dst;
} 

void *memmove(void *dst, const void *src, size_t len)
{
	/*
	 * The following test makes use of unsigned arithmetic overflow to
	 * more efficiently test the condition !(src <= dst && dst < str+len).
	 * It also avoids the situation where the more explicit test would give
	 * incorrect results were the calculation str+len to overflow (though
	 * that issue is probably moot as such usage is probably undefined
	 * behaviour and a bug anyway.
	 */
	if ((size_t)dst - (size_t)src >= len) {
		/* destination not in source data, so can safely use memcpy */
		return memcpy(dst, src, len);
	} else {
		/* copy backwards... */
		const char *end = dst;
		const char *s = (const char *)src + len;
		char *d = (char *)dst + len;
		while (d != end)
			*--d = *--s;
	}
	return dst;
} 

void mmap_add_region(unsigned long base_pa, unsigned long base_va,
			unsigned long size, unsigned attr)
{
	mmap_region_t *mm = mmap;
	mmap_region_t *mm_last = mm + sizeof(mmap) / sizeof(mmap[0]) - 1;
	unsigned long pa_end = base_pa + size - 1;
	unsigned long va_end = base_va + size - 1;


	if (!size)
		return;

	/* Find correct place in mmap to insert new region */
	while (mm->base_va < base_va && mm->size)
		++mm;

	/* Make room for new region by moving other regions up by one place */
	memmove(mm + 1, mm, (unsigned int *)mm_last - (unsigned int *)mm);

	mm->base_pa = base_pa;
	mm->base_va = base_va;
	mm->size = size;
	mm->attr = attr;

	if (pa_end > max_pa)
		max_pa = pa_end;
	if (va_end > max_va)
		max_va = va_end;
}


static unsigned int calc_physical_addr_size_bits(unsigned long max_addr)
{

	/* 48 bits address */
	if (max_addr & ADDR_MASK_44_TO_47)
		return TCR_PS_BITS_256TB;

	/* 44 bits address */
	if (max_addr & ADDR_MASK_42_TO_43)
		return TCR_PS_BITS_16TB;

	/* 42 bits address */
	if (max_addr & ADDR_MASK_40_TO_41)
		return TCR_PS_BITS_4TB;

	/* 40 bits address */
	if (max_addr & ADDR_MASK_36_TO_39)
		return TCR_PS_BITS_1TB;

	/* 36 bits address */
	if (max_addr & ADDR_MASK_32_TO_35)
		return TCR_PS_BITS_64GB;

	return TCR_PS_BITS_4GB;
}
 


static int mmap_region_attr(mmap_region_t *mm, unsigned long base_va,
					unsigned long size)
{
	int attr = mm->attr;

	for (;;) {
		++mm;

		if (!mm->size)
			return attr; /* Reached end of list */

		if (mm->base_va >= base_va + size)
			return attr; /* Next region is after area so end */

		if (mm->base_va + mm->size <= base_va)
			continue; /* Next region has already been overtaken */

		if ((mm->attr & attr) == attr)
			continue; /* Region doesn't override attribs so skip */

		attr &= mm->attr;

		if (mm->base_va > base_va ||
			mm->base_va + mm->size < base_va + size)
			return -1; /* Region doesn't fully cover our area */
	}
}  

static unsigned long mmap_desc(unsigned attr, unsigned long addr_pa,
					unsigned level)
{
	unsigned long desc = addr_pa;

	desc |= level == 3 ? TABLE_DESC : BLOCK_DESC;

	desc |= attr & MT_NS ? LOWER_ATTRS(NS) : 0;

	desc |= attr & MT_RW ? LOWER_ATTRS(AP_RW) : LOWER_ATTRS(AP_RO);

	desc |= LOWER_ATTRS(ACCESS_FLAG);

	if ((attr & MT_MEMORY) == MT_MEMORY) 
	{
		if ((attr & MT_NONCACHED) == MT_NONCACHED)
		{
			desc |= LOWER_ATTRS(ATTR_MEMNC_INDEX | ISH);
    		if (attr & MT_RW)
		    	desc |= 0;
		}
		else
		{
	    	desc |= LOWER_ATTRS(ATTR_IWBWA_OWBWA_NTR_INDEX | ISH);
    		if (attr & MT_RW)
		    	desc |= 0;
		}
	} else {
		desc |= LOWER_ATTRS(ATTR_DEVICE_INDEX | ISH);
		desc |= UPPER_ATTRS(XN);
	}

	return desc;
}

static mmap_region_t *init_xlation_table(mmap_region_t *mm,
					unsigned long base_va,
					unsigned long *table, unsigned level)
{
	unsigned level_size_shift = L1_XLAT_ADDRESS_SHIFT - (level - 1) *
						XLAT_TABLE_ENTRIES_SHIFT;
	unsigned level_size = 1 << level_size_shift;
	unsigned long level_index_mask = XLAT_TABLE_ENTRIES_MASK << level_size_shift;

	do  {
		unsigned long desc = UNSET_DESC;

		if (mm->base_va + mm->size <= base_va) {
			/* Area now after the region so skip it */
			++mm;
			continue;
		}

		if (mm->base_va >= base_va + level_size) {
			/* Next region is after area so nothing to map yet */
			desc = INVALID_DESC;
		} else if (mm->base_va <= base_va && mm->base_va + mm->size >=
				base_va + level_size) {
			/* Next region covers all of area */
			int attr = mmap_region_attr(mm, base_va, level_size);
			if (attr >= 0)
			{
				if ((attr & MT_MEMORY) == MT_MEMORY)
				    desc = mmap_desc(attr, base_va - mm->base_va, level);
				else
					desc = mmap_desc(attr, base_va - mm->base_va + mm->base_pa,	level);

			}
		}
		/* else Next region only partially covers area, so need */

		if (desc == UNSET_DESC) {
			/* Area not covered by a region so need finer table */
			unsigned long *new_table = xlat_tables[next_xlat++];
			desc = TABLE_DESC | (unsigned long)new_table;

			/* Recurse to fill in new table */
			mm = init_xlation_table(mm, base_va,
						new_table, level+1);
		}

		*table++ = desc;
		base_va += level_size;
	} while (mm->size && (base_va & level_index_mask));

	return mm;
}

void enable_mmu_el3(void)					
{

		uint64_t mair, tcr, ttbr;				
		uint32_t sctlr;						
														
		/* Set attributes in the right indices of the MAIR */	
		mair = MAIR_ATTR_SET(ATTR_DEVICE, ATTR_DEVICE_INDEX);	
		mair |= MAIR_ATTR_SET(ATTR_IWBWA_OWBWA_NTR,		
				ATTR_IWBWA_OWBWA_NTR_INDEX);
		mair |= MAIR_ATTR_SET(ATTR_MEMNC, ATTR_MEMNC_INDEX);

	/*b14716*/
		write_mair_el3(mair);				
								
		/* Invalidate TLBs at the current exception level */	
		tlbialle3();
									
		/* Set TCR bits as well. */				
		/* Inner & outer WBWA & shareable + T0SZ = 32 */	
	/*b1476 */
		tcr = TCR_SH_INNER_SHAREABLE | TCR_RGN_OUTER_WBA |	
			TCR_RGN_INNER_WBA | 32;
		tcr |= TCR_EL3_RES1 | (tcr_ps_bits << TCR_EL3_PS_SHIFT);					
	/*b14716*/
		write_tcr_el3(tcr);					

		/* Set TTBR bits as well */				
		ttbr = (uint64_t) l1_xlation_table;
		write_ttbr0_el3(ttbr);				
									
		/* Ensure all translation table writes have drained */	
		/* into memory, the TLB invalidation is complete, */	
		/* and translation register writes are committed */	
		/* before enabling the MMU */				
		__asm("dsb sy");							
		__asm("isb");							
									
	/*b14716*/
		sctlr = read_sctlr_el3();				
		sctlr |= /*SCTLR_WXN_BIT |*/ SCTLR_M_BIT | SCTLR_I_BIT;	
		//sctlr |= SCTLR_A_BIT | SCTLR_C_BIT;			
		sctlr |= SCTLR_C_BIT;			
	/*b14716*/
		write_sctlr_el3(sctlr);				
									
		/* Ensure the MMU enable takes effect immediately */	
		__asm("isb");

		flush_dcache_range(&flush_dcache_range, 0x30);
		flush_icache_range(&flush_icache_range, 0x30);

}

void mmu_init(void)
{
    //mmap_add_region(0, 0, 0x100000000, MT_MEMORY | MT_NONCACHED | MT_RW | MT_SECURE);
	mmap_add_region(0x00000000, 0x00000000, 0x100000000, MT_MEMORY | MT_NONCACHED | MT_RW | MT_SECURE);
    /*mmap_add_region(0x3B000000, 0x3B000000, 0x1000, MT_DEVICE | MT_RW | MT_SECURE);
    mmap_add_region(0x7D001000, 0x7D001000, 0x5000, MT_DEVICE | MT_RW | MT_SECURE);
	//mmap_add_region(0x80000000, 0x80000000, 0x20000000, MT_DEVICE | MT_RW | MT_SECURE);
	mmap_add_region(0x74000000, 0x74000000, 0x4000000, MT_DEVICE | MT_RW | MT_SECURE);
	mmap_add_region(0x78000000, 0x78000000, 0x4000000, MT_DEVICE | MT_RW | MT_SECURE);
	//mmap_add_region(0x90000000, 0x90000000, 0x10000000, MT_DEVICE | MT_RW | MT_SECURE);*/
	
	mmap_add_region(0x00000000, 0x00000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //96 Kbytes  Boot ROM  
    mmap_add_region(0x1E000000, 0x1E000000, 0x1E800000, MT_DEVICE | MT_RW | MT_SECURE);  //32 Kbytes CM4 TCML (code)
    mmap_add_region(0x20000000, 0x20000000, 0x10000000, MT_DEVICE | MT_RW | MT_SECURE);  //256 (depending on external flash size) QuadSPI0
    mmap_add_region(0x3E000000, 0x3E000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //32 KBytes CM4 TCMU (data)
    mmap_add_region(0x3E800000, 0x3E800000, 0x00800000, MT_MEMORY | MT_NONCACHED | MT_RW | MT_SECURE);  //4 Mbytes System RAM
    mmap_add_region(0x40000000, 0x40000000, 0x00080000, MT_DEVICE | MT_RW | MT_SECURE);  //0.5  IPS0 Peripherals  
    mmap_add_region(0x40080000, 0x40080000, 0x00080000, MT_DEVICE | MT_RW | MT_SECURE);  //0.5  IPS1 Peripherals  
    mmap_add_region(0x71000000, 0x71000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //8 (depending on the external flash size) QuadSPI0 Rx Buffer  
    mmap_add_region(0x72000000, 0x72000000, 0x01000000, MT_DEVICE | MT_RW | MT_SECURE);  //16 PCIe
    mmap_add_region(0x74000000, 0x74000000, 0x02000000, MT_DEVICE | MT_RW | MT_SECURE);  //32 APEX0
    mmap_add_region(0x78000000, 0x78000000, 0x02000000, MT_DEVICE | MT_RW | MT_SECURE);  //32 APEX1
    mmap_add_region(0x7C000000, 0x7C000000, 0x00100000, MT_DEVICE | MT_NONCACHED | MT_RW | MT_SECURE);  //1 Sequencer
    mmap_add_region(0x7D000000, 0x7D000000, 0x00100000, MT_DEVICE | MT_RW | MT_SECURE);  //1 (sparsely used , up to 0x7fff) GIC
    mmap_add_region(0x7E000000, 0x7E000000, 0x00100000, MT_DEVICE | MT_RW | MT_SECURE);  //1 CCI-400
    mmap_add_region(0x80000000, 0x80000000, 0x40000000, MT_MEMORY | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
    mmap_add_region(0xC0000000, 0xC0000000, 0x10000000, MT_MEMORY | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
	mmap_add_region(0xD0000000, 0xD0000000, 0x10000000, MT_MEMORY | MT_RW | MT_SECURE);  //4 Mbytes System RAM
	mmap_add_region(0xE0000000, 0xE0000000, 0x20000000, MT_MEMORY | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
	

	
    init_xlation_table(mmap, 0, l1_xlation_table, 1);
    tcr_ps_bits = calc_physical_addr_size_bits(max_pa);
    enable_mmu_el3();
}

