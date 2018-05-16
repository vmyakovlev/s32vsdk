#.org    0x00
#Eninge: IPUS3, IPUS4
#step_x, step_in, step_out=1
# no neighborhood used
#input:
#line0: Y 15 bit
#output:
#line0: Y 15 bit
# parameters:
#gpr0 enable or disable
#gpr1 table offset
#gpr2 table bits: 15-bits
#gpr3 computed lsb mask

.global gamma_start

gamma_start:
        lsl gpr3,gpr0,15
        mov gpr3,15
        done gamma_d1,i
gamma_d1:
        beq gamma_copy

        mov  confalu,(0 /* unsigned*/ | (1 << 1) /* saturate*/ | (8<<4) /* shr =8*/ ) 
        //mov gpr1,0x800   // 2K offset
        //mov gpr2,4       // 2K LUT
        sub gpr3,gpr3,gpr2
        lsr gpr3,one,gpr3

gamma_loop:
        and ina0,0x7fff      // should not be required
        lsr gpr15,ina0,gpr2  // down to a 10  bit value
        add luta,gpr15,gpr1
        and ina0,ina0,gpr3   // keep LSBs not going into LUT
        add out0,lut,ina0    // add LSBs to lut value
//        mov loopcnt,2
//gamma_delay:
//        nop
//        loop gamma_delay
        done gamma_loop,ixo
//        halt
gamma_copy:
        dout ina0,gamma_copy,ixo

.global gamma_end
gamma_end:
