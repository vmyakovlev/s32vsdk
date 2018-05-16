#Engine: IPUS2, IPUS3 (LUT)
#Function:
#LUT with 12 bit in 16 out
# extract max and min
#turn on off via GPR0
#
#step_x, step_in, step_out=1
# no neighborhood used
#input:
#line0: Y 16 bit
#output:
#line0: Y 16 bit
#
# parameters:
.global lut4k_start

lut4k_start:
//        mov lock,0
//        mov  confalu,(0 /* unsigned*/ | (1 << 1) /* saturate*/ | (8<<4) /* shr =8*/ ) 
        done lut4k_d1,i
lut4k_d1:

lut4k_loop:
        lsr  luta,ina0,4      // 16bit to 12bit
	dout lut,lut4k_loop,ixo
//      halt

.global lut4k_end
lut4k_end:
