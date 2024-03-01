`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/19 21:13:55
// Design Name: 
// Module Name: regfile
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


module regfile(
    input  wire       clk,
    input  wire       rst,

    input  wire[4:0]  reg_ra1,
    input  wire[4:0]  reg_ra2,
    
    input  wire       reg_we,
    input  wire[4:0]  reg_wa,
    input  wire[31:0] reg_wd,
    
    output wire[31:0] reg_rd1,
    output wire[31:0] reg_rd2
);
    
    reg [31:0] regs[31:0];
    
    assign reg_rd1 = reg_ra1 == 5'b0 ? 32'b0 : regs[reg_ra1];
    assign reg_rd2 = reg_ra2 == 5'b0 ? 32'b0 : regs[reg_ra2];
    
    integer i;
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            for (i = 0; i <= 31; i = i + 1) 
                regs[i] <= 32'b0;
        end
        else if (reg_we == 1'b1) 
            regs[reg_wa] <= reg_wd;
    end
        
endmodule
