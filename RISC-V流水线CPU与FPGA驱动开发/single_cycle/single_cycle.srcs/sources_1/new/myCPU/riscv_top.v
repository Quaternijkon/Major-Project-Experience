`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/05/12 01:41:49
// Design Name: 
// Module Name: riscv_top
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


module mycpu(
    input         rstn,
    input         clk,

    output [31:0] inst_rom_addr,
    input  [31:0] inst_rom_rdata,

    output [31:0] data_ram_addr,
    output [31:0] data_ram_wdata,
    output        data_ram_wen,
    input  [31:0] data_ram_rdata
);
    
    wire [31:0] pc_in;
    wire [31:0] pc_out;

    wire [31:0] instr;
            
    wire [6:0]  opcode;
    wire [2:0]  funct3;
    wire [6:0]  funct7;
    wire        c1;
    wire        c2;
    wire [1:0]  c3;
    wire [2:0]  alu_op;
    wire [1:0]  branch;
    wire        dmem_we;
    wire        reg_we;
    
    wire [4:0]  reg_ra1;
    wire [4:0]  reg_ra2;
    wire [31:0] reg_rd1;
    wire [31:0] reg_rd2;
    wire [4:0]  reg_wa;
    wire [19:0] imm20_u;
    wire [31:0] imm20_e32;
    wire [31:0] pc_plus4;
    wire [31:0] reg_wd;
    
    wire [11:0] imm12_s;
    wire [11:0] imm12_i;
    wire [31:0] imm12_e32;
    wire [31:0] alu_in1;
    wire [31:0] alu_in2;
    wire [31:0] alu_out;

    wire [31:0] dmem_rd;
    
    wire [11:0] imm12_b;
    wire [19:0] imm20_j;
    wire [31:0] offset;
    
    assign pc_in = pc_out + (jump ? offset : 32'h4);
    
    assign inst_rom_addr = pc_out;
    assign instr = inst_rom_rdata;

    assign funct7 = instr[31:25];
    assign funct3 = instr[14:12];
    assign opcode = instr[6:0];

    assign reg_ra1 = instr[19:15];
    assign reg_ra2 = instr[24:20];
    assign reg_wa = instr[11:7];
    
    assign imm20_u = instr[31:12];
    assign imm20_e32 = {imm20_u, 12'b0};
    assign pc_plus4 = pc_out + 32'h4;
    assign reg_wd = sel_2to4(c3, imm20_e32, pc_plus4, alu_out, dmem_rd);
    function [31:0] sel_2to4(input [1:0] sel,
                             input [31:0] in0, input [31:0] in1,
                             input [31:0] in2, input [31:0] in3);
    begin
        case (sel)
            2'b00 : sel_2to4 = in0;
            2'b01 : sel_2to4 = in1;
            2'b10 : sel_2to4 = in2;
            2'b11 : sel_2to4 = in3;
        endcase
    end
    endfunction
    
    assign alu_in1 = reg_rd1;
    assign imm12_s = {instr[31:25], instr[11:7]};
    assign imm12_i = instr[31:20];
    assign imm12_e32 = c1 ? {{20{imm12_s[11]}}, imm12_s} : {{20{imm12_i[11]}}, imm12_i};
    assign alu_in2 = c2 ? reg_rd2 : imm12_e32;
    
    assign data_ram_addr = alu_out;
    assign data_ram_wen = dmem_we;
    assign data_ram_wdata = reg_rd2;
    assign dmem_rd = data_ram_rdata;

    assign imm12_b = {instr[31], instr[7], instr[30:25], instr[11:8]};
    assign imm20_j = {instr[31], instr[19:12], instr[20], instr[30:21]};

    pc _pc(
        .clk(clk),
        .rst(rstn),
        .pc_in(pc_in),
        .pc_out(pc_out)
    );

    control _cu(
        .opcode(opcode),
        .funct3(funct3),
        .funct7(funct7),
        .c1(c1),
        .c2(c2),
        .c3(c3),
        .alu_op(alu_op),
        .branch(branch),
        .dmem_we(dmem_we),
        .reg_we(reg_we)
    );
    
    regfile _regfile(
        .clk(clk),
        .rst(rstn),
        .reg_we(reg_we),
        .reg_ra1(reg_ra1),
        .reg_ra2(reg_ra2),
        .reg_wa(reg_wa),
        .reg_wd(reg_wd),
        .reg_rd1(reg_rd1),
        .reg_rd2(reg_rd2)
    );

    alu _alu(
        .alu_op(alu_op),
        .alu_in1(alu_in1),
        .alu_in2(alu_in2),
        .alu_out(alu_out)
    );
    
    br _br(
        .branch(branch),
        .imm12_b(imm12_b),
        .imm20_j(imm20_j),
        .reg_rd1(reg_rd1),
        .reg_rd2(reg_rd2),
        .jump(jump),
        .offset(offset)
    );

endmodule
