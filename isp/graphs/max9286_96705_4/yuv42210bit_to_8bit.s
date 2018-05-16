#.org    0x00
.global YUV42210bit_to_8bit_ipus_start

YUV42210bit_to_8bit_ipus_start:

        done YUV42210bit_to_8bit_d0_3_6,i    // load first pixel
YUV42210bit_to_8bit_d0_3_6:

  mov out2, ina6
  mov out1, ina3 
	dout ina0,YUV42210bit_to_8bit_d0_3_6,ixo

        halt
.global YUV42210bit_to_8bit_ipus_end
YUV42210bit_to_8bit_ipus_end:
