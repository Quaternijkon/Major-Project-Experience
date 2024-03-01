`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/19 21:13:55
// Design Name: 
// Module Name: alu
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`include "macro.vh"

module alu(
//    input  wire        clk,
//    input  wire        rst,
    
    input  wire [2:0]  alu_op,
    input  wire [31:0] alu_in1,
    input  wire [31:0] alu_in2,
   
    output wire [31:0] alu_out
);
    
    reg err;
    reg zero;
    
    wire [31:0] in1   = alu_in1;
    wire [31:0] in2   = alu_in2;
    
    wire [32:0] in1_e = {alu_in1[31], alu_in1};
    wire [32:0] in2_e = {alu_in2[31], alu_in2};
    
    wire [32:0] o_add = in1_e + in2_e;
    wire [32:0] o_sub = in1_e - in2_e;
    
    wire [31:0] o_or  = in1 | in2;

    assign alu_out = (alu_op == `ALU_ADD) ? o_add[31:0] :
                     (alu_op == `ALU_SUB) ? o_sub[31:0] :
                     (alu_op == `ALU_OR ) ? o_or : 32'b0;
    
endmodule
