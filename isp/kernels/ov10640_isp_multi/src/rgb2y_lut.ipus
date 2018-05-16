# for IPUS with LUT
# function:
# convert RGB to Y and get the compression factor from LUT
# in0  :  R
# in1  :  G
# in2  :  B
# out0 :  LUT[Y]
# parameters
# gpr0-2 RGB-> Y coefficients 15 bit


.global rgb2y_lut_start
rgb2y_lut_start:
  // scaled to 32768 = 15 bits to allow to deal with overflows
  // Y= 0,299xR + 0,587xG + 0,114xB
  mov gpr0,9797   //R*gpr0->Y     :  32768 * 0,299
  mov gpr1,19234  //G*gpr1->Y     :  32768 * 0,587
  mov gpr2,3735   //B*gpr2->Y     :  32768 * 0,114
  mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (15<<4) /*shr*/)
  done rgb2y_lut_loop,i

rgb2y_lut_loop:	

  //Y
  mulh sacc0,ina0,gpr0  // R->Y
  mulh acc0,ina3,gpr1   // G->Y
  mulh acc0,ina6,gpr2   // B->Y
  lsr luta,acc0,4       // 16 to 12 bit
  dout lut,rgb2y_lut_loop,ixo

  //halt
.global rgb2y_lut_end
rgb2y_lut_end:
