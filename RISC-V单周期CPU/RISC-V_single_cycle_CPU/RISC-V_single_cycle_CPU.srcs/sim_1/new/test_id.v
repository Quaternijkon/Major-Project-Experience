`timescale 1ns / 1ps

module test_id;
    
    reg         rst_n;
    reg [31:0]  inst;
    
    wire [4:0]  rw_addr;
    wire [4:0]  ra_addr;
    wire [4:0]  rb_addr;    

    
    wire  branch;       
    wire  jump;         
    wire  mem_to_reg;   
    wire  reg_wr;       
    wire  mem_wr;       
    wire  alu_asrc;     
    
    
    wire [1:0] alu_bsrc;   
    wire [2:0] ext_op;     
    wire [3:0] alu_ctr;    

    initial begin

        rst_n = 0;
        
        #50 
        rst_n = 1'b1;   
        inst =  32'b0000_0001_1111_1111_1000_1111_1011_0011;
        
        
        
        
        
        
        
        
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b0000_0001_1111_1111_1010_1111_1011_0011;
        
        
        
        
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b0000_0001_1111_1111_1011_1111_1011_0011;
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1110_1111_1001_0011;
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1111_1111_1011_0111;
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1010_1111_1000_0011;
        
        
        
        
        
        
        
        

        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1010_1111_1010_0011;
        
        
        
        

        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1010_1111_1110_0011;
        
        
        
        
        
        #50 
        rst_n = 1'b1;   
        inst = 32'b1111_1111_1111_1111_1111_1111_1110_1111;
        
        
        
        
        
    end
    
    id id0(
        .rst_n(rst_n),
        .inst_i(inst),
    
        .reg1_addr_o(ra_addr),
        .reg2_addr_o(rb_addr),
        .wd_o(rw_addr),  
        
        .branch_o(branch),       
        .jump_o(jump),         
        .mem_to_reg_o(mem_to_reg),   
        .reg_wr_o(reg_wr),       
        .mem_wr_o(mem_wr),       
        .alu_asrc_o(alu_asrc),     
        
        
        .alu_bsrc_o(alu_bsrc),   
        .ext_op_o(ext_op),     
        .alu_ctr_o(alu_ctr)     
        );
    
endmodule
