`timescale 1ns / 1ps

module test_pc;
    wire [31:0]    cur_pc;
    reg  [31:0]    imm;
    reg           branch;
    reg           zero;

    wire [31:0]    next_pc;

    reg              ce;
    
    wire [31:0]     inst;

    reg clk;    
    reg rst_n;
    
        
    initial begin
        clk = 0;
        rst_n = 0;
    
        #100
        rst_n = 1'b1;
        ce = 1'b1;       
    
        branch = 1'b0;
        zero = 1'b0;
    
    end
    always #100 clk = ~clk;

    next_pc next_pc0(
        .pc(cur_pc),
        .imm(imm),
        .branch(branch),
        .zero(zero),
        .next_pc(next_pc)
        );
    
    inst_rom inst_rom0(
        .ce(ce),
        .addr(cur_pc),
        .inst(inst)
        );
    
    pc_reg pc_reg0(
        .clk(clk),
        .rst_n(rst_n),
        
        .next_pc(next_pc),
        .cur_pc(cur_pc)
        );

endmodule
