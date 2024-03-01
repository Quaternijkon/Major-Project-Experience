`timescale 1ns / 1ps

module id(

    input             rst_n,
    input [31:0]      inst_i,
    
    
    output reg [4:0]  reg1_addr_o,
    output reg [4:0]  reg2_addr_o,
    output reg [4:0]  wd_o,
    
    
    output reg  branch_o,       
    output reg  jump_o,         
    output reg  mem_to_reg_o,   
    output reg  reg_wr_o,       
    output reg  mem_wr_o,       
    output reg  alu_asrc_o,     
    
    
    output reg [1:0]  alu_bsrc_o,   
    output reg [2:0]  ext_op_o,     
    output reg [3:0]  alu_ctr_o     

    );
    
    wire [6:0] op  = inst_i[6:0];            
    wire [2:0] fn = inst_i[14:12];          
        
    always @ (*) begin
        if(~rst_n) begin    
            wd_o <= 0;
            reg1_addr_o <= 0;
            reg2_addr_o <= 0;
        end
        else begin      
            wd_o <= inst_i[11:7];                     
            reg1_addr_o <= inst_i[19:15];             
            reg2_addr_o <= inst_i[24:20];             
            
            
            branch_o <= op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0];        
            jump_o <= op[6]&op[5]&~op[4]&op[3]&op[2]&op[1]&op[0];            
            mem_to_reg_o <= ~op[6]&~op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0];  
            reg_wr_o <= (~op[6]&op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])       
                        | (~op[6]&~op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])   
                        | (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0])     
                        | (~op[6]&~op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0])  
                        | (op[6]&op[5]&~op[4]&op[3]&op[2]&op[1]&op[0]);     
            mem_wr_o <= ~op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0];       
            alu_asrc_o <= op[6]&op[5]&~op[4]&op[3]&op[2]&op[1]&op[0];        
            
            
            alu_bsrc_o[1] <= (~op[6]&~op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])   
                        | (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0])       
                        | (~op[6]&~op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0])    
                        | (~op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0]);    
            alu_bsrc_o[0] <= op[6]&op[5]&~op[4]&op[3]&op[2]&op[1]&op[0];       
            
            ext_op_o[2] <= op[6]&op[5]&~op[4]&op[3]&op[2]&op[1]&op[0];       
            ext_op_o[1] <= (op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0])    
                        | (~op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0]);  
            ext_op_o[0] <= (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0])     
                        | (op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0]);   
           
            
            alu_ctr_o[3] <= (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0])    
                        | (op[6]&op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0]);   
            alu_ctr_o[2] <= ((~op[6]&op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])  
                        | (~op[6]&~op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0]))  
                        & fn[2]
                        | (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0]);    
            alu_ctr_o[1] <= ((~op[6]&op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])  
                        | (~op[6]&~op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0]))  
                        & fn[1]
                        | (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0]);    
            alu_ctr_o[0] <= ((~op[6]&op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0])  
                        | (~op[6]&~op[5]&op[4]&~op[3]&~op[2]&op[1]&op[0]))  
                        & fn[0]
                        | (~op[6]&op[5]&op[4]&~op[3]&op[2]&op[1]&op[0]);    
                        
            
        end
    end
    
endmodule
