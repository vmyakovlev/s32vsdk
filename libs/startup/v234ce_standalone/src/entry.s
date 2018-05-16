.section ".entry_pt","ax",@progbits

.global _entry

.equ DMA_REG_1, 0x40003020
.equ DMA_REG_2, 0x40003024
.equ DMA_REG_3, 0x40003028
.equ DMA_REG_4, 0x40003030
.equ DMA_REG_5, 0x40003034
.equ DMA_REG_6, 0x4000303C

.equ DMA_VAL_1, 0x80000000
.equ DMA_VAL_2, 0x03030008
.equ DMA_VAL_3, 0x01000000
.equ DMA_VAL_4, 0xC0000000
.equ DMA_VAL_5, 0x00010008
.equ DMA_VAL_6, 0x00010001

_entry:
	mov x0, #0x80000000
	mov x1, #0x10000000
	
	mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
dflush_loop:
  dc  civac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    dflush_loop
  dsb sy
	   
	mov x5, #0x0
	mov x6, #0x0
	ldr w5, =DMA_REG_1
	ldr w6, =DMA_VAL_1
	str w6, [x5]
	ldr w5, =DMA_REG_2
	ldr w6, =DMA_VAL_2
	str w6, [x5]
	ldr w5, =DMA_REG_3
	ldr w6, =DMA_VAL_3
	str w6, [x5]
	ldr w5, =DMA_REG_4
	ldr w6, =DMA_VAL_4
	str w6, [x5]
	ldr w5, =DMA_REG_5
	ldr w6, =DMA_VAL_5
	str w6, [x5]
	ldr w5, =DMA_REG_6
	ldr w6, =DMA_VAL_6
	str w6, [x5]
dma_loop_1:
	ldr w6, [x5]
	and w6, w6, 0x0080
	cbz w6, dma_loop_1
	
    adrp	x16, __startup
	mov x17, #0x40000000
	add x16, x16, x17
	br	x16
	

	