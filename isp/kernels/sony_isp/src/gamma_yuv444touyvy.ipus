# any IPUS engine
# in0  :  Y
# in1  :  U
# in2  :  V
# out0 :  Y, U, Y, V

.global gamma_yuv444touyvy_start
gamma_yuv444touyvy_start:
        mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (0<<4) /*shr*/)
        done gamma_yuv444touyvy_d1,i
gamma_yuv444touyvy_d1:
        mov maskv,0b011011000
        done gamma_yuv444touyvy_loop,i

gamma_yuv444touyvy_loop:
	//lsr luta,ina1,4   // 15bit -> 11 bit
	lsr luta,ina1,5   // 16bit -> 11 bit
	//max gpr2,gpr2,luta  // sth for debugging
        //asl w,ina,1       // u and v from 15 bit -> 16 bit
        mov w,ina         // instead of shift
	mov gpr1,lut      // read 16 bit value from LUT
	//lsr luta,ina0,4   // 15bit -> 11 bit
	lsr luta,ina0,5   // 16bit -> 11 bit
	//max gpr2,gpr2,luta  // sth for debugging
        sub zero,ina0,ina1    // ina1(y0) <= ina0(y1)
	mov gpr0,lut      // read 16 bit value from LUT
        bge gamma_yuv444touyvy_take_second
        dout w4,gamma_yuv444touyvy_take_first1,xo     // u1
gamma_yuv444touyvy_take_first1:
        dout gpr1,gamma_yuv444touyvy_take_first2,xo   // y0
gamma_yuv444touyvy_take_first2:
        dout w7,gamma_yuv444touyvy_take_first3,ixo    // v1
gamma_yuv444touyvy_take_first3:
        dout gpr0,gamma_yuv444touyvy_loop,ixo         // y1


gamma_yuv444touyvy_take_second:
        dout w3,gamma_yuv444touyvy_take_second1,xo    // u0
gamma_yuv444touyvy_take_second1:
        dout gpr1,gamma_yuv444touyvy_take_second2,xo  // y0
gamma_yuv444touyvy_take_second2:
        dout w6,gamma_yuv444touyvy_take_second3,ixo   // v0
gamma_yuv444touyvy_take_second3:
        dout gpr0,gamma_yuv444touyvy_loop,ixo         // y1

        //halt
.global gamma_yuv444touyvy_end
gamma_yuv444touyvy_end:
