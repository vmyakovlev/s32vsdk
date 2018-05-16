// IPUV kernel
// INPUT:
// INA0: R
// INA1: G
// INA2: B
// OUTPUT:
// OUT0: Y[0..14]
// OUT1: U[0..14]
// OUT2: V[0..14]
// no subsampling
// REGISTERS:
// SET BY SEQUENCER
// SET BY KERNEL
// GPR0 - GPR6 Matrix and Offset
	
# for IPUV1
#.org    0x00
.global rgb2yuv_planar_start
# set shift_in/x/out to 4 pixels
# in0  :  R
# in1  :  G
# in2  :  B
# out0 :  Y
# out1 :  U
# out2 :  V

        
rgb2yuv_planar_start:
        // scaled to 32768 = 15 bits to allow to deal with overflows
        // Y= 0,299xR + 0,587xG + 0,114xB
        mov gpr0,9797  //R->Y :  32768 * 0,299
        //mov gpr1,9617 //G->Y :  32768 * 0,587 / 2
        mov gpr1,19234  //G->Y :  32768 * 0,587
        mov gpr2,3735   //B->Y :  32768 * 0,114
        mov gpr3,16154  //(B-Y)->U :  32768 * 0,493
        mov gpr4,28737  //(R-Y)->V :  32768 * 0,877
        mov gpr5,0x4000 // U and V offset for 0 position
        mov gpr6,1      // << 1
  
        mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (16<<4) /*shr*/)
        done rgb2yuv_planar_loop,i

rgb2yuv_planar_loop:    
        //Y
        mulh vsacc0,vh0,gpr0  // R->Y
        mulh vacc0,vh5,gpr1   // G->Y
        mulh vacc0,vh10,gpr2  // B->Y
        lsl vout0,vacc0,gpr6  // Y
        // U
        mulh vout1,vh10,gpr3  // B
        mulh vgpr2,vout0,gpr3 // Y
        add vout1,vout1,gpr5  // offset
        sub vout1,vout1,vgpr2
        // V
        mulh vout2,vh0,gpr4   // R
        mulh vgpr2,vout0,gpr4 // Y
        add vout2,vout2,gpr5  // offset
        sub vout2,vout2,vgpr2

        lsl vout1,vout1,gpr6
        lsl vout2,vout2,gpr6
        done rgb2yuv_planar_loop,ixo

        //halt
.global rgb2yuv_planar_end
rgb2yuv_planar_end:
