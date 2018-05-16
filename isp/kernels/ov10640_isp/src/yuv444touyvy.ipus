# any IPUS engine
# in0  :  Y  16 bit
# in1  :  U  16 bit
# in2  :  V  16 bit
# out0 :  Y, U, Y, V

.global yuv444touyvy_start
yuv444touyvy_start:
        mov confalu,(0 /*unsigned*/ | (1<<1) /*saturate*/ | (0<<4) /*shr*/)
        done yuv444touyvy_d1,i
yuv444touyvy_d1:
        mov maskv,0b011011011 //0b011011011
        done yuv444touyvy_loop,i

yuv444touyvy_loop:
        mov w,ina         // instead of shift
        //asl w,ina,1
        sub zero,ina0,ina1    // ina1(y0) <= ina0(y1)
        bge yuv444touyvy_take_second
        dout w4,yuv444touyvy_take_first1,xo   // u1
yuv444touyvy_take_first1:
        dout w1,yuv444touyvy_take_first2,ixo   // y0
yuv444touyvy_take_first2:
        dout w7,yuv444touyvy_take_first3,xo  // v1
yuv444touyvy_take_first3:
        dout w0,yuv444touyvy_loop,ixo         // y1


yuv444touyvy_take_second:
        dout w3,yuv444touyvy_take_second1,xo  // u0
yuv444touyvy_take_second1:
        dout w1,yuv444touyvy_take_second2,ixo  // y0
yuv444touyvy_take_second2:
        dout w6,yuv444touyvy_take_second3,xo // v0
yuv444touyvy_take_second3:
        dout w0,yuv444touyvy_loop,ixo         // y1

        //halt
.global yuv444touyvy_end
yuv444touyvy_end:
