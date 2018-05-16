// IPUS
// one chennel bayer pattern in
// 3 channels out
// out0: r
// out1: g
// out2: b



.global debayer_rgb_subsample_planar_start
debayer_rgb_subsample_planar_start:
	mov gpr0,400 // gain r, fixed point 8.8, (400/2) for 15 bit output
	//mov gpr1,256 // gain g, 256 for 16 bit output always 1
	mov gpr2,490 // gain b, (490/2) for 15 bit output

        done debayer_rgb_subsample_planar_d0_g,i
debayer_rgb_subsample_planar_d0_g:
        done debayer_rgb_subsample_planar_d1_g,i
debayer_rgb_subsample_planar_d1_g:

	mov confalu, (0 /*unsigned*/ | (1<<1) /*saturate*/ | (8<< 4) /* SHR: 8.8*/)
	mov confaddt,(1 /*ina*/ | (0<<2) /*apply mask*/ | (0<<3) /*unsgined*/ | (1<<5) /* shr=1 */ | (0x40<<9) /* neutral scale*/)
	mov maskv,0b000010001

debayer_rgb_subsample_planar_loop0:
	mulh out0,ina1,gpr0 // r
	//mulh out1,ina0,gpr1 // g
	mulh out2,ina3,gpr2 // b
	mov out1,scaled //g

	done debayer_rgb_subsample_planar_evenlodd,ixo
debayer_rgb_subsample_planar_evenlodd:
	done debayer_rgb_subsample_planar_loop0,i
	
//	halt

.global debayer_rgb_subsample_planar_end
debayer_rgb_subsample_planar_end:
