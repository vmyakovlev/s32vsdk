# IPUV
# set shift_in/x/out to 4 pixels
# in0  :  Y
# in1  :  Y
# in2  :  Y
# in2  :  Y
# in2  :  Y
# out0 :  Y denoised / or original
# out1 :  Y denoised with edge indication

# gaussian taps GPR0 - GPR5
# 1 4  6  4  1
# 4 16 24 16 4
# 6 24 36 24 6
# 4 16 24 16 4
# 1 4  6  4  1

# sum = 256

# GPR6 threshold
  
.global denoise_start
denoise_start:
        mov gpr0,1
        mov gpr1,4
        mov gpr2,6
        mov gpr3,16
        mov gpr4,24
        mov gpr5,36
        mov gpr6,0x0300  // threshold
        //mov gpr7,0x1000  // for debugging
  
        mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (8<<4) /*shr*/)
        done denoise_d1,i
denoise_d1:
        done denoise_loop,i
denoise_loop:
        // FIR
        mov maskv,0xf
        mulh vsacc0,vh0,gpr0
        mulh vacc0,vh4 ,gpr0
        mulh vacc0,vh20,gpr0
        mulh vacc0,vh24,gpr0
        mulh vacc0,vh1 ,gpr1
        mulh vacc0,vh3 ,gpr1
        mulh vacc0,vh5 ,gpr1
        mulh vacc0,vh9 ,gpr1
        mulh vacc0,vh15,gpr1
        mulh vacc0,vh19,gpr1
        mulh vacc0,vh21,gpr1
        mulh vacc0,vh23,gpr1
        mulh vacc0,vh2 ,gpr2
        mulh vacc0,vh10,gpr2
        mulh vacc0,vh14,gpr2
        mulh vacc0,vh22,gpr2
        mulh vacc0,vh6 ,gpr3
        mulh vacc0,vh8 ,gpr3
        mulh vacc0,vh16,gpr3
        mulh vacc0,vh18,gpr3
        mulh vacc0,vh7 ,gpr4
        mulh vacc0,vh11,gpr4
        mulh vacc0,vh13,gpr4
        mulh vacc0,vh17,gpr4
        mulh vacc0,vh12,gpr5

        // compare difference with threshold
        abd vgpr2,vacc0,vh12
        sub vgpr2,vgpr2,gpr6

        // select filtered if lower than threshold
        mov maskv, vflag_c
        mov vout0,vacc0
        // for enhancment using dialation
        mov vout1,vacc0

        // keep input signal when higher than threshold
        xor maskv,maskv,one
        mov vout0,vh12
        // for enhancment using dialation
        // for debugging
        //add vout0,vout0,gpr7
        mov vout1,zero
        //mov vout1,one
 
        //done denoise_loop,ixo     // output filtered
        dvot vh12,denoise_loop,ixo  // output original input

        //halt
.global denoise_end
denoise_end:
