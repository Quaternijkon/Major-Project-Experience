`timescale 1ns / 1ps

module alu_ctr(
    input[3:0]    alu_ctr,

    output reg        sub_ctr,
    output reg        sig_ctr,
    output reg [1:0]  op_ctr
    );

always @ (*) begin

    sub_ctr <= (~alu_ctr[3] & ~alu_ctr[2] & alu_ctr[1]) | alu_ctr[3];
    sig_ctr <= ~alu_ctr[0];
    op_ctr[1]  <= (~alu_ctr[3] & ~alu_ctr[2] & alu_ctr[1]) | (alu_ctr[3] & alu_ctr[2] & alu_ctr[1] & alu_ctr[0]);
    op_ctr[0]  <= (~alu_ctr[3] & ~alu_ctr[2] & alu_ctr[1]) | (~alu_ctr[3] & alu_ctr[2] & alu_ctr[1] & ~alu_ctr[0]);

end    

endmodule
