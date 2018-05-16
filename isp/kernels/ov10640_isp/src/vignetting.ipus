// for IPUS2 or IPUS3 (incl LUT)
.global vignetting_start
vignetting_start:
// in line A0: input data
// out line0: output data
//
// configuration registers:
// gpr0 X-center position
// gpr1 Y-center position -> sqr diff to y-center
// gpr2 distance scale
// gpr3 max LUT not used
//	
// registers used:
// gpr5 y-distance^2 (per line)
// grp4 x-distance^2 and distance^2

	
        mov  confalu,(0 /* unsigned*/ | (1 << 1) /* saturate*/ | (8<<4) /* shr =8*/ ) 
        //mov confshr,8
        //mov gpr3,0x3ff
        // srq diff for y... constant over line
        abd gpr5,gpr1,ypos
        mulh gpr5,gpr5,gpr5
        done vignetting_loop,i

vignetting_loop:
        // compute distance/index
	//nop
        abd gpr4,gpr0,xpos
	//nop
        mulh gpr4,gpr4,gpr4
	nop
        add  gpr4,gpr4,gpr5
	mulh luta,gpr4,gpr2
	nop
	nop
        //min  luta,gpr4,gpr3
        //mov  luta,gpr3

        //mulh out1,ina3,lut
        mulh out0,ina0,lut
	nop

        done vignetting_loop,ixo
.global vignetting_end
vignetting_end:
