#include "mmap.h"

static uint64_t tf_xlat_tables[MAX_XLAT_TABLES][XLAT_TABLE_ENTRIES]
__attribute__((aligned(XLAT_TABLE_SIZE))) __attribute__((section(".xlat_table")));

static uint64_t tf_base_xlat_table[NUM_BASE_LEVEL_ENTRIES]
__attribute__((aligned(NUM_BASE_LEVEL_ENTRIES * sizeof(uint64_t))));

static mmap_region_t tf_mmap[MAX_MMAP_REGIONS + 1];        

#if PLAT_XLAT_TABLES_DYNAMIC
static int xlat_tables_mapped_regions[MAX_XLAT_TABLES];
#endif /* PLAT_XLAT_TABLES_DYNAMIC */

void write_ttbr0_el3(uint64_t);
void write_tcr_el3(uint64_t);
void write_sctlr_el3(uint64_t);
void write_mair_el3(uint64_t);
void tlbialle3(void);
uint64_t read_sctlr_el3(void);

xlat_ctx_t tf_xlat_ctx = {

	.pa_max_address = PLAT_PHY_ADDR_SPACE_SIZE - 1,
	.va_max_address = PLAT_VIRT_ADDR_SPACE_SIZE - 1,

	.mmap = tf_mmap,
	.mmap_num = MAX_MMAP_REGIONS,

	.tables = tf_xlat_tables,
	.tables_num = MAX_XLAT_TABLES,
#if PLAT_XLAT_TABLES_DYNAMIC
	.tables_mapped_regions = xlat_tables_mapped_regions,
#endif /* PLAT_XLAT_TABLES_DYNAMIC */

	.base_table = tf_base_xlat_table,
	.base_table_entries = NUM_BASE_LEVEL_ENTRIES,

	.max_pa = 0,
	.max_va = 0,

	.next_table = 0,

	.base_level = XLAT_TABLE_LEVEL_BASE,

	.initialized = 0
};

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



void enable_mmu_el3(unsigned int flags,	uint64_t *base_table)                   
{

        uint64_t mair, tcr, ttbr;		
		uint32_t sctlr;	                     
                
        /* Invalidate TLBs at the current exception level */	
		tlbialle3();
        
        /* Set attributes in the right indices of the MAIR */   
        mair = MAIR_ATTR_SET(ATTR_DEVICE, ATTR_DEVICE_INDEX);	
		mair |= MAIR_ATTR_SET(ATTR_IWBWA_OWBWA_NTR,		
				ATTR_IWBWA_OWBWA_NTR_INDEX);		
		mair |= MAIR_ATTR_SET(ATTR_NON_CACHEABLE,		
				ATTR_NON_CACHEABLE_INDEX);		

        write_mair_el3(mair);               
                                            
        /* Set TCR bits as well. */             
        /* Set T0SZ to (64 - width of virtual address space) */	
        if (flags & XLAT_TABLE_NC) {				
			/* Inner & outer non-cacheable non-shareable. */
			tcr = TCR_SH_NON_SHAREABLE |			
				TCR_RGN_OUTER_NC | TCR_RGN_INNER_NC |	
				(64 - __builtin_ctzl(PLAT_VIRT_ADDR_SPACE_SIZE));
		} else {						
			/* Inner & outer WBWA & shareable. */		
			tcr = TCR_SH_INNER_SHAREABLE |			
				TCR_RGN_OUTER_WBA | TCR_RGN_INNER_WBA |	
				(64 - __builtin_ctzl(PLAT_VIRT_ADDR_SPACE_SIZE));
		}							
		
        write_tcr_el3(tcr);                 

        /* Set TTBR bits as well */             
        ttbr = (uint64_t) base_table;
        write_ttbr0_el3(ttbr);              
                                    
        /* Ensure all translation table writes have drained */  
        /* into memory, the TLB invalidation is complete, */    
        /* and translation register writes are committed */ 
        /* before enabling the MMU */               
        __asm("dsb ish");                            
        __asm("isb");                            
              
        sctlr = read_sctlr_el3();               
        //sctlr |= SCTLR_WXN_BIT | SCTLR_M_BIT;           
        sctlr |= /*SCTLR_WXN_BIT |*/ SCTLR_M_BIT | SCTLR_I_BIT; 
        //sctlr |= SCTLR_A_BIT | SCTLR_C_BIT;           
        
        if (flags & DISABLE_DCACHE)				
			sctlr &= ~SCTLR_C_BIT;				
		else							
			sctlr |= SCTLR_C_BIT;				
    
        write_sctlr_el3(sctlr);             
                                    
        /* Ensure the MMU enable takes effect immediately */    
        __asm("isb");
}

static int mmap_add_region_check(xlat_ctx_t *ctx, uint64_t base_pa,
				 uintptr_t base_va, size_t size,
				 mmap_attr_t attr)
{
	mmap_region_t *mm = ctx->mmap;
	uint64_t end_pa = base_pa + size - 1;
	uintptr_t end_va = base_va + size - 1;

	if (!IS_PAGE_ALIGNED(base_pa) || !IS_PAGE_ALIGNED(base_va) ||
			!IS_PAGE_ALIGNED(size))
		return -EINVAL;

	/* Check for overflows */
	if ((base_pa > end_pa) || (base_va > end_va))
		return -ERANGE;

	if ((base_va + (uintptr_t)size - (uintptr_t)1) > ctx->va_max_address)
		return -ERANGE;

	if ((base_pa + (uint64_t)size - 1ULL) > ctx->pa_max_address)
		return -ERANGE;

	/* Check that there is space in the mmap array */
	if (ctx->mmap[ctx->mmap_num - 1].size != 0)
		return -ENOMEM;

	/* Check for PAs and VAs overlaps with all other regions */
	for (mm = ctx->mmap; mm->size; ++mm) {

		uintptr_t mm_end_va = mm->base_va + mm->size - 1;

		/*
		 * Check if one of the regions is completely inside the other
		 * one.
		 */
		int fully_overlapped_va =
			((base_va >= mm->base_va) && (end_va <= mm_end_va)) ||
			((mm->base_va >= base_va) && (mm_end_va <= end_va));

		/*
		 * Full VA overlaps are only allowed if both regions are
		 * identity mapped (zero offset) or have the same VA to PA
		 * offset. Also, make sure that it's not the exact same area.
		 * This can only be done with static regions.
		 */
		if (fully_overlapped_va) {

#if PLAT_XLAT_TABLES_DYNAMIC
			if ((attr & MT_DYNAMIC) || (mm->attr & MT_DYNAMIC))
				return -EPERM;
#endif /* PLAT_XLAT_TABLES_DYNAMIC */
			if ((mm->base_va - mm->base_pa) != (base_va - base_pa))
				return -EPERM;

			if ((base_va == mm->base_va) && (size == mm->size))
				return -EPERM;

		} else {
			/*
			 * If the regions do not have fully overlapping VAs,
			 * then they must have fully separated VAs and PAs.
			 * Partial overlaps are not allowed
			 */

			uint64_t mm_end_pa =
						     mm->base_pa + mm->size - 1;

			int separated_pa =
				(end_pa < mm->base_pa) || (base_pa > mm_end_pa);
			int separated_va =
				(end_va < mm->base_va) || (base_va > mm_end_va);

			//if (!(separated_va && separated_pa))
				//return -EPERM;
		}
	}

	return 0;
}

void mmap_add_region_ctx(xlat_ctx_t *ctx, mmap_region_t *mm)
{
	mmap_region_t *mm_cursor = ctx->mmap;
	mmap_region_t *mm_last = mm_cursor + ctx->mmap_num;
	uint64_t end_pa = mm->base_pa + mm->size - 1;
	uintptr_t end_va = mm->base_va + mm->size - 1;
	int ret;

	/* Ignore empty regions */
	if (!mm->size)
		return;

	/* Static regions must be added before initializing the xlat tables. */
	
	ret = mmap_add_region_check(ctx, mm->base_pa, mm->base_va, mm->size,
				    mm->attr);
	if (ret != 0) {
		return;
	}

	/*
	 * Find correct place in mmap to insert new region.
	 *
	 * 1 - Lower region VA end first.
	 * 2 - Smaller region size first.
	 *
	 * VA  0                                   0xFF
	 *
	 * 1st |------|
	 * 2nd |------------|
	 * 3rd                 |------|
	 * 4th                            |---|
	 * 5th                                   |---|
	 * 6th                            |----------|
	 * 7th |-------------------------------------|
	 *
	 * This is required for overlapping regions only. It simplifies adding
	 * regions with the loop in xlat_tables_init_internal because the outer
	 * ones won't overwrite block or page descriptors of regions added
	 * previously.
	 *
	 * Overlapping is only allowed for static regions.
	 */

	while ((mm_cursor->base_va + mm_cursor->size - 1) < end_va
	       && mm_cursor->size)
		++mm_cursor;

	while ((mm_cursor->base_va + mm_cursor->size - 1 == end_va)
	       && (mm_cursor->size < mm->size))
		++mm_cursor;

	/* Make room for new region by moving other regions up by one place */
	memmove(mm_cursor + 1, mm_cursor,
		(uintptr_t)mm_last - (uintptr_t)mm_cursor);

	/*
	 * Check we haven't lost the empty sentinel from the end of the array.
	 * This shouldn't happen as we have checked in mmap_add_region_check
	 * that there is free space.
	 */
	mm_cursor->base_pa = mm->base_pa;
	mm_cursor->base_va = mm->base_va;
	mm_cursor->size = mm->size;
	mm_cursor->attr = mm->attr;

	if (end_pa > ctx->max_pa)
		ctx->max_pa = end_pa;
	if (end_va > ctx->max_va)
		ctx->max_va = end_va;
}

void mmap_add_region(uint64_t base_pa, uintptr_t base_va,
			size_t size, mmap_attr_t attr)
{
	mmap_region_t mm = {
		.base_va = base_va,
		.base_pa = base_pa,
		.size = size,
		.attr = attr,
	};
	mmap_add_region_ctx(&tf_xlat_ctx, (mmap_region_t *)&mm);
}

/*
 * Enumeration of actions that can be made when mapping table entries depending
 * on the previous value in that entry and information about the region being
 * mapped.
 */
typedef enum {

	/* Do nothing */
	ACTION_NONE,

	/* Write a block (or page, if in level 3) entry. */
	ACTION_WRITE_BLOCK_ENTRY,

	/*
	 * Create a new table and write a table entry pointing to it. Recurse
	 * into it for further processing.
	 */
	ACTION_CREATE_NEW_TABLE,

	/*
	 * There is a table descriptor in this entry, read it and recurse into
	 * that table for further processing.
	 */
	ACTION_RECURSE_INTO_TABLE,

} action_t;

static action_t xlat_tables_map_region_action(const mmap_region_t *mm,
		const int desc_type, const unsigned long long dest_pa,
		const uintptr_t table_entry_base_va, const int level)
{
	uintptr_t mm_end_va = mm->base_va + mm->size - 1;
	uintptr_t table_entry_end_va =
			table_entry_base_va + XLAT_BLOCK_SIZE(level) - 1;

	/*
	 * The descriptor types allowed depend on the current table level.
	 */

	if ((mm->base_va <= table_entry_base_va) &&
	    (mm_end_va >= table_entry_end_va)) {

		/*
		 * Table entry is covered by region
		 * --------------------------------
		 *
		 * This means that this table entry can describe the whole
		 * translation with this granularity in principle.
		 */

		if (level == 3) {
			/*
			 * Last level, only page descriptors are allowed.
			 */
			if (desc_type == PAGE_DESC) {
				/*
				 * There's another region mapped here, don't
				 * overwrite.
				 */
				return ACTION_NONE;
			} else {
				return ACTION_WRITE_BLOCK_ENTRY;
			}

		} else {

			/*
			 * Other levels. Table descriptors are allowed. Block
			 * descriptors too, but they have some limitations.
			 */

			if (desc_type == TABLE_DESC) {
				/* There's already a table, recurse into it. */
				return ACTION_RECURSE_INTO_TABLE;

			} else if (desc_type == INVALID_DESC) {
				/*
				 * There's nothing mapped here, create a new
				 * entry.
				 *
				 * Check if the destination granularity allows
				 * us to use a block descriptor or we need a
				 * finer table for it.
				 *
				 * Also, check if the current level allows block
				 * descriptors. If not, create a table instead.
				 */
				if ((dest_pa & XLAT_BLOCK_MASK(level)) ||
				    (level < MIN_LVL_BLOCK_DESC))
					return ACTION_CREATE_NEW_TABLE;
				else
					return ACTION_WRITE_BLOCK_ENTRY;

			} else {
				/*
				 * There's another region mapped here, don't
				 * overwrite.
				 */
				
				return ACTION_NONE;
			}
		}

	} else if ((mm->base_va <= table_entry_end_va) ||
		   (mm_end_va >= table_entry_base_va)) {

		/*
		 * Region partially covers table entry
		 * -----------------------------------
		 *
		 * This means that this table entry can't describe the whole
		 * translation, a finer table is needed.

		 * There cannot be partial block overlaps in level 3. If that
		 * happens, some of the preliminary checks when adding the
		 * mmap region failed to detect that PA and VA must at least be
		 * aligned to PAGE_SIZE.
		 */
		
		if (desc_type == INVALID_DESC) {
			/*
			 * The block is not fully covered by the region. Create
			 * a new table, recurse into it and try to map the
			 * region with finer granularity.
			 */
			return ACTION_CREATE_NEW_TABLE;

		} else {
			/*
			 * The block is not fully covered by the region, but
			 * there is already a table here. Recurse into it and
			 * try to map with finer granularity.
			 *
			 * PAGE_DESC for level 3 has the same value as
			 * TABLE_DESC, but this code can't run on a level 3
			 * table because there can't be overlaps in level 3.
			 */
			return ACTION_RECURSE_INTO_TABLE;
		}
	}

	/*
	 * This table entry is outside of the region specified in the arguments,
	 * don't write anything to it.
	 */
	return ACTION_NONE;
}

static uint64_t *xlat_table_get_empty(xlat_ctx_t *ctx)
{
	for (unsigned int i = 0; i < ctx->tables_num; i++)
		if (ctx->tables_mapped_regions[i] == 0)
			return ctx->tables[i];

	return 0;
}

static uint64_t xlat_desc(mmap_attr_t attr, unsigned long long addr_pa,
			  int level, uint64_t execute_never_mask)
{
	uint64_t desc;
	int mem_type;


	desc = addr_pa;
	/*
	 * There are different translation table descriptors for level 3 and the
	 * rest.
	 */
	desc |= (level == XLAT_TABLE_LEVEL_MAX) ? PAGE_DESC : BLOCK_DESC;
	/*
	 * Always set the access flag, as TF doesn't manage access flag faults.
	 * Deduce other fields of the descriptor based on the MT_NS and MT_RW
	 * memory region attributes.
	 */
	desc |= (attr & MT_NS) ? LOWER_ATTRS(NS) : 0;
	desc |= (attr & MT_RW) ? LOWER_ATTRS(AP_RW) : LOWER_ATTRS(AP_RO);
	desc |= LOWER_ATTRS(ACCESS_FLAG);

	/*
	 * Deduce shareability domain and executability of the memory region
	 * from the memory type of the attributes (MT_TYPE).
	 *
	 * Data accesses to device memory and non-cacheable normal memory are
	 * coherent for all observers in the system, and correspondingly are
	 * always treated as being Outer Shareable. Therefore, for these 2 types
	 * of memory, it is not strictly needed to set the shareability field
	 * in the translation tables.
	 */
	mem_type = MT_TYPE(attr);
	if (mem_type == MT_DEVICE) {
		desc |= LOWER_ATTRS(ATTR_DEVICE_INDEX | OSH);
		/*
		 * Always map device memory as execute-never.
		 * This is to avoid the possibility of a speculative instruction
		 * fetch, which could be an issue if this memory region
		 * corresponds to a read-sensitive peripheral.
		 */
		desc |= execute_never_mask;

	} else { /* Normal memory */
		/*
		 * Always map read-write normal memory as execute-never.
		 * (Trusted Firmware doesn't self-modify its code, therefore
		 * R/W memory is reserved for data storage, which must not be
		 * executable.)
		 * Note that setting the XN bit here is for consistency only.
		 * The function that enables the MMU sets the SCTLR_ELx.WXN bit,
		 * which makes any writable memory region to be treated as
		 * execute-never, regardless of the value of the XN bit in the
		 * translation table.
		 *
		 * For read-only memory, rely on the MT_EXECUTE/MT_EXECUTE_NEVER
		 * attribute to figure out the value of the XN bit.
		 */
		if ((attr & MT_RW) || (attr & MT_EXECUTE_NEVER)) {
			desc |= execute_never_mask;
		}

		if (mem_type == MT_MEMORY) {
			desc |= LOWER_ATTRS(ATTR_IWBWA_OWBWA_NTR_INDEX | ISH);
		} else {
			desc |= LOWER_ATTRS(ATTR_NON_CACHEABLE_INDEX | OSH);
		}
	}

	return desc;
}

static int xlat_table_get_index(xlat_ctx_t *ctx, const uint64_t *table)
{
	for (unsigned int i = 0; i < ctx->tables_num; i++)
		if (ctx->tables[i] == table)
			return i;

	return -1;
}

/* Increments region count for a given table. */
static void xlat_table_inc_regions_count(xlat_ctx_t *ctx, const uint64_t *table)
{
	ctx->tables_mapped_regions[xlat_table_get_index(ctx, table)]++;
}

static uintptr_t xlat_tables_map_region(xlat_ctx_t *ctx, mmap_region_t *mm,
				   const uintptr_t table_base_va,
				   uint64_t *const table_base,
				   const int table_entries,
				   const unsigned int level)
{
	uintptr_t mm_end_va = mm->base_va + mm->size - 1;

	uintptr_t table_idx_va;
	unsigned long long table_idx_pa;

	uint64_t *subtable;
	uint64_t desc;

	int table_idx;

	if (mm->base_va > table_base_va) {
		/* Find the first index of the table affected by the region. */
		table_idx_va = mm->base_va & ~XLAT_BLOCK_MASK(level);

		table_idx = (table_idx_va - table_base_va) >>
			    XLAT_ADDR_SHIFT(level);

		
	} else {
		/* Start from the beginning of the table. */
		table_idx_va = table_base_va;
		table_idx = 0;
	}

#if PLAT_XLAT_TABLES_DYNAMIC
	if (level > ctx->base_level)
		xlat_table_inc_regions_count(ctx, table_base);
#endif

	while (table_idx < table_entries) {

		desc = table_base[table_idx];

		table_idx_pa = mm->base_pa + table_idx_va - mm->base_va;

		action_t action = xlat_tables_map_region_action(mm,
			desc & DESC_MASK, table_idx_pa, table_idx_va, level);

		if (action == ACTION_WRITE_BLOCK_ENTRY) {

			table_base[table_idx] =
				xlat_desc(mm->attr, table_idx_pa, level,
					  ctx->execute_never_mask);

		} else if (action == ACTION_CREATE_NEW_TABLE) {

			subtable = xlat_table_get_empty(ctx);
			if (subtable == 0) {
				/* Not enough free tables to map this region */
				return table_idx_va;
			}

			/* Point to new subtable from this one. */
			table_base[table_idx] = TABLE_DESC | (unsigned long)subtable;

			/* Recurse to write into subtable */
			uintptr_t end_va = xlat_tables_map_region(ctx, mm, table_idx_va,
					       subtable, XLAT_TABLE_ENTRIES,
					       level + 1);
			if (end_va != table_idx_va + XLAT_BLOCK_SIZE(level) - 1)
				return end_va;

		} else if (action == ACTION_RECURSE_INTO_TABLE) {

			subtable = (uint64_t *)(uintptr_t)(desc & TABLE_ADDR_MASK);
			/* Recurse to write into subtable */
			uintptr_t end_va =  xlat_tables_map_region(ctx, mm, table_idx_va,
					       subtable, XLAT_TABLE_ENTRIES,
					       level + 1);
			if (end_va != table_idx_va + XLAT_BLOCK_SIZE(level) - 1)
				return end_va;

		} else {


		}

		table_idx++;
		table_idx_va += XLAT_BLOCK_SIZE(level);

		/* If reached the end of the region, exit */
		if (mm_end_va <= table_idx_va)
			break;
	}

	return table_idx_va - 1;
}

void init_xlation_table(xlat_ctx_t *ctx)
{
	mmap_region_t *mm = ctx->mmap;

	/* All tables must be zeroed before mapping any region. */

	for (unsigned int i = 0; i < ctx->base_table_entries; i++)
		ctx->base_table[i] = INVALID_DESC;

	for (unsigned int j = 0; j < ctx->tables_num; j++) {
#if PLAT_XLAT_TABLES_DYNAMIC
		ctx->tables_mapped_regions[j] = 0;
#endif
		for (unsigned int i = 0; i < XLAT_TABLE_ENTRIES; i++)
			ctx->tables[j][i] = INVALID_DESC;
	}

	while (mm->size) {
		uintptr_t end_va = xlat_tables_map_region(ctx, mm, 0, ctx->base_table,
				ctx->base_table_entries, ctx->base_level);

		if (end_va != mm->base_va + mm->size - 1) {
			return;
		}

		mm++;
	}

	ctx->initialized = 1;
}

void mmu_init(void)
{
    //mmap_add_region(0, 0, 0x100000000, MT_MEMORY | MT_NONCACHED | MT_RW | MT_SECURE);
    //mmap_add_region(0x00000000, 0x00000000, 0x200000000, MT_MEMORY | MT_NON_CACHEABLE | MT_RW | MT_SECURE);
    /*mmap_add_region(0x3B000000, 0x3B000000, 0x1000, MT_DEVICE | MT_RW | MT_SECURE);
    mmap_add_region(0x7D001000, 0x7D001000, 0x5000, MT_DEVICE | MT_RW | MT_SECURE);
    //mmap_add_region(0x80000000, 0x80000000, 0x20000000, MT_DEVICE | MT_RW | MT_SECURE);
    mmap_add_region(0x74000000, 0x74000000, 0x4000000, MT_DEVICE | MT_RW | MT_SECURE);
    mmap_add_region(0x78000000, 0x78000000, 0x4000000, MT_DEVICE | MT_RW | MT_SECURE);
    //mmap_add_region(0x90000000, 0x90000000, 0x10000000, MT_DEVICE | MT_RW | MT_SECURE);*/
    
    mmap_add_region(0x00000000, 0x00000000, 0x80000000, MT_DEVICE | MT_RW | MT_SECURE);  //96 Kbytes  Boot ROM  
//     mmap_add_region(0x00000000, 0x00000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //96 Kbytes  Boot ROM  
//     mmap_add_region(0x1E000000, 0x1E000000, 0x1E800000, MT_DEVICE | MT_RW | MT_SECURE);  //32 Kbytes CM4 TCML (code)
//     mmap_add_region(0x20000000, 0x20000000, 0x10000000, MT_DEVICE | MT_RW | MT_SECURE);  //256 (depending on external flash size) QuadSPI0
//     mmap_add_region(0x3E000000, 0x3E000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //32 KBytes CM4 TCMU (data)
//     //mmap_add_region(0x3E800000, 0x3E800000, 0x00800000, MT_MEMORY | MT_RW | MT_SECURE);  //4 Mbytes System RAM
//     mmap_add_region(0x3E800000, 0x3E800000, 0x00800000, MT_MEMORY | MT_NON_CACHEABLE | MT_RW | MT_SECURE);  //4 Mbytes System RAM
//     mmap_add_region(0x40000000, 0x40000000, 0x00080000, MT_DEVICE | MT_RW | MT_SECURE);  //0.5  IPS0 Peripherals  
//     mmap_add_region(0x40080000, 0x40080000, 0x00080000, MT_DEVICE | MT_RW | MT_SECURE);  //0.5  IPS1 Peripherals  
//     
//     mmap_add_region(0x71000000, 0x71000000, 0x00800000, MT_DEVICE | MT_RW | MT_SECURE);  //8 (depending on the external flash size) QuadSPI0 Rx Buffer  
//     mmap_add_region(0x72000000, 0x72000000, 0x01000000, MT_DEVICE | MT_RW | MT_SECURE);  //16 PCIe
//     mmap_add_region(0x74000000, 0x74000000, 0x02000000, MT_DEVICE | MT_RW | MT_SECURE);  //32 APEX0
//     mmap_add_region(0x78000000, 0x78000000, 0x02000000, MT_DEVICE | MT_RW | MT_SECURE);  //32 APEX1
//     mmap_add_region(0x7C000000, 0x7C000000, 0x00100000, MT_DEVICE | MT_NONCACHED | MT_RW | MT_SECURE);  //1 Sequencer
//     mmap_add_region(0x7D000000, 0x7D000000, 0x00100000, MT_DEVICE | MT_RW | MT_SECURE);  //1 (sparsely used , up to 0x7fff) GIC
//     mmap_add_region(0x7E000000, 0x7E000000, 0x00100000, MT_DEVICE | MT_RW | MT_SECURE);  //1 CCI-400
    mmap_add_region(0x80000000, 0x80000000, 0x80000000, MT_MEMORY | MT_RW | MT_SECURE);  //1024 (depending on external DRAM    size)  DDR  
    //mmap_add_region(0xC0000000, 0xC0000000, 0x40000000, MT_MEMORY | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
    
    
    mmap_add_region(0x80000000, 0x180000000, 0x80000000, MT_MEMORY | MT_NON_CACHEABLE | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
    //mmap_add_region(0xC0000000, 0x1C0000000, 0x40000000, MT_MEMORY | MT_NON_CACHEABLE | MT_RW | MT_SECURE);  //1024 (depending on external DRAM size)  DDR  
 
    
    init_xlation_table(&tf_xlat_ctx);
    enable_mmu_el3(0, &tf_base_xlat_table);
}

