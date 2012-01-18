vs_1_1              // version instruction
dcl_position v0     // define position data in register v0
m4x4 oPos, v0, c0   // transform vertices by view/projection matrix
mov oD0, c4         // load constant color

