`timescale 1ns / 1ps

module test_alu_ctr;

    reg[3:0]    alu_ctr;
    
    wire        sub_ctr;
    wire        sig_ctr;
    wire [1:0]  op_ctr;
    
    alu_ctr alu_ctr0(.alu_ctr(alu_ctr), .sub_ctr(sub_ctr), .sig_ctr(sig_ctr), .op_ctr(op_ctr));
    
    initial begin

        alu_ctr = 4'b0000;
        
        #100
        
        alu_ctr = 4'b1111;
        

    end
    
endmodule
