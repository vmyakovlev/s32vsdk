# for IPUS no accelerators 4 inputs 1 output
# set shift_in/x/out to 4 pixels
# Inputs:	
# inA0-2:  Y Denoised and edge information
# in1   :  Y original
# Output:
# out0  :  Y enhanced denoised

.global denoise2_start
denoise2_start:
        mov confsort,(1 /*ina*/ | (1<<2) /*ignore mask*/ | (0<<3) /*unsigned*/)
        done denoise2_d1,i
denoise2_d1:
        done denoise2_d2,i
denoise2_d2:
        done denoise2_loop,i
denoise2_loop:
        sub zero,min,zero    // check for a zero in the neighborhood
        beq denoise2_orig
        //dout inb1, denoise2_loop, ixo
        dout ina4, denoise2_loop, ixo
denoise2_orig:
        //dout ina4, denoise2_loop, ixo
        dout inb1, denoise2_loop, ixo
        //dout zero, denoise2_loop, ixo

        //halt
.global denoise2_end
denoise2_end:
