// IPUS1/2/4/5
// 3 input lines top right starting with R
// 2 lines out
//   out 0: green with full resolution
//   out 1: blue or red with half length

//.org    0x00
.global debayer_g_start
debayer_g_start:
        done debayer_g_d0,i
debayer_g_d0:
        done debayer_g_d1,i
debayer_g_d1:
        done debayer_g_d2,i
debayer_g_d2:
        //mov confbest,0x3  // (ussigned <<1) | (keep_min)
        mov confalu,2
        //mov confsat,1
        and ypos,1
        //mov gpr8,1
        //and zero,ypos,gpr8
        //mov confsgn,0

        mov confaddt,(0/*w*/) | (0/*apply mask*/<<2) | (0 /* unsigned*/<<3) | (0/* shift right*/ <<5) | (64 /* factor*/ <<9)
        //mov maskv,0x82 // 9'b0 1000 0010
        bne debayer_g_greenfirst

// 7 instrcutions
  
debayer_g_loop0:  // R or B
        mov out1,ina4            # even line B, odd line R
        // interpolate GR
        abd gpr9,ina1,ina7       # vertical gradient
        abd gpr10,ina3,ina5      # horizontal gradient
        sub zero,gpr9,gpr10
        lsr w,ina,1
        bls debayer_g_vertical0
debayer_g_horizontal0:
        add out0,w3,w5           # horizontal interpolation
        done debayer_g_loop1,ixo
debayer_g_vertical0:
        add out0,w1,w7           # vertical interpolation
        done debayer_g_loop1,ixo
// 10 instrcutions per 2 pixles

debayer_g_greenfirst:
debayer_g_loop1:  //G
        mov out1,ina3            # take next R or B
        dout ina4,debayer_g_loop0,ixo      # take green as is
// 2 instrcutions per pixel
  
//        halt
.global debayer_g_end	
debayer_g_end:
