`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/19 21:13:55
// Design Name: 
// Module Name: br
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

module br(
    input  wire [1:0]   branch,
    input  wire [11:0]  imm12_b,
    input  wire [19:0]  imm20_j,
    input  wire [31:0]  reg_rd1,
    input  wire [31:0]  reg_rd2,
   
    output wire         jump,
    output wire [31:0]  offset
);
    
    wire branch_eq = (reg_rd1 == reg_rd2) ? 1'b1 : 1'b0;
    assign jump = (branch == `BR_JAL) ? 1'b1 :
                  (branch == `BR_BEQ) ? branch_eq : 1'b0;

    wire [31:0] offset_b = {{19{imm12_b[11]}}, imm12_b, 1'b0};
    wire [31:0] offset_j = {{11{imm20_j[19]}}, imm20_j, 1'b0};
    assign offset = (branch == `BR_JAL) ? offset_j :
                    (branch == `BR_BEQ) ? offset_b : 32'b0;
    
endmodule
