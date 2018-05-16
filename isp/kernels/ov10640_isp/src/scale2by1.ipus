.global scale2by1_start
scale2by1_start:
        mov  confalu, (0 /*unsigned*/ | (1<<1) /*saturate*/)
        mov  confaddt,(0 /*w*/ | (0<<3) /*unsigned*/ | (0<<5) /*shift*/| (0x40 << 9) /* scale*/)
        done d0,i
d0: 
        done d1,i
d1:
        done d2,i
d2:
        // prepare shift matrix
        // 4 3 4                              1 2 1
        // 3 2 3   => FIR coefficients 1/16 * 2 4 2
        // 4 3 4                              1 2 1
        mset 0b101000101
        mov  w,4             // to w
        mset 0b010101010
        mov  w,3             // to w
        mov  w4,2
        swp               // 
        mset 0b111111111
loop:
        lsr  w,ina,ww
        dout sum,odd,ixo
odd:
        done loop,i
.global scale2by1_end
scale2by1_end:
