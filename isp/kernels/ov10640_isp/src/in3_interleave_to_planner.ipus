// IPUS kernel
// 2 input line: line 0, line 1
// starting with Low exposure, then short and very short
// no subsampling
// out 0: L
// out 1: S
// out 2: VS

.global in3_interleave_to_planner_start

in3_interleave_to_planner_start:
        done in3_interleave_to_planner_d0,i
in3_interleave_to_planner_d0:
        done in3_interleave_to_planner_d1,i
in3_interleave_to_planner_d1:
        done in3_interleave_to_planner_d2,i
in3_interleave_to_planner_d2:

//in3_interleave_to_planner_loop:

mov out1,ina1        //short
mov out2,ina0        //very short
 
dout ina2, in3_interleave_to_planner_d0, ixo   //long

.global in3_interleave_to_planner_end
in3_interleave_to_planner_end:
