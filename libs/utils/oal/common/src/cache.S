.global flush_and_invalidate_dcache_range
flush_and_invalidate_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
dflushinvalidate_loop:
  dc  civac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    dflushinvalidate_loop
  dsb sy
  ret
  
.global invalidate_dcache_range
invalidate_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
inv_dflush_loop:
  dc  ivac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    inv_dflush_loop
  dsb ish
  ret
  
.global flush_dcache_range
flush_dcache_range:
  mrs x3, CTR_EL0
  ubfx  x3, x3, #16, #4
  mov x2, #4
  lsl x2, x2, x3
  add x1, x0, x1
  sub x3, x2, #1
  bic x0, x0, x3
dflush_loop:
  dc  cvac, x0
  add x0, x0, x2
  cmp x0, x1
  b.lo    dflush_loop
  dsb sy
  ret