`timescale 1ns / 1ps

module alu_top(
    input[31:0] regA_i,
    input[31:0] regB_i,
    input[3:0]  alu_ctr,
    
    output[31:0]    res_o,
    output          zero
    );
    
    
    
     
    wire        sub_ctr;
    wire        sig_ctr;
    wire [1:0]  op_ctr;
    
    wire   zf; 
    wire   sf; 
    wire   cf; 
    wire   of; 
    
    wire    cout;
    
    
    alu_ctr alu_ctr0(
        .alu_ctr(alu_ctr), 
        .sub_ctr(sub_ctr), 
        .sig_ctr(sig_ctr), 
        .op_ctr(op_ctr)
        );
        
    alu alu0(
        .regA_i(regA_i), 
        .regB_i(regB_i), 
        .sub_ctr_i(sub_ctr), 
        .sig_ctr_i(sig_ctr), 
        .op_ctr_i(op_ctr), 
        .zf(zf), 
        .sf(sf), 
        .cf(cf), 
        .of(of), 
        .cout(cout), 
        .result_o(res_o)
        );
    
    assign zero = zf;   
    
endmodule

