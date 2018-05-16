# for IPUS
# function:
# take one input and copy it to two outputs ("t")
# in0  :  IN
# out0 :  copy of IN
# out1 :  copy of IN
# parameters

.global tee_start
tee_start:
        //mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (14<<4) /*shr*/)
        done tee_loop,i    // load first pixel

tee_loop:
	asl ina0,ina0,gpr15
	mov out1,ina0
	dout ina0,tee_loop,ixo

        halt
.global tee_end
tee_end:
