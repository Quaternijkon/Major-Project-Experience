`timescale 1ns / 1ps

module test_cpu_top;
    
    reg         clk;
    reg         rst_n;
    
    wire [31:0]    inst;    
    wire [31:0]    cur_pc;
    wire [31:0]    next_pc;
    
    wire [4:0]  rw_addr;
    wire [4:0]  ra_addr;
    wire [4:0]  rb_addr; 
    
    wire [31:0]      imm;
    wire             zero;
    wire             ce;
    
    wire [31:0]      bus_w;   
    wire [31:0]      bus_a;          
    wire [31:0]      bus_b;     
    wire [31:0]      bus_bi;   
    wire [31:0]      bus_ai;   
    wire [31:0]      bus_wo;   
    wire [31:0]      data_out; 

    
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
        clk = 0;
        rst_n = 0;
        
        #50
        rst_n = 1'b1;
        
    end
    always #50 clk = ~clk;
    
    
    reg_file reg_file0(
        .rst_n(rst_n),
        .clk(clk),
        
        .rw(rw_addr),
        .bus_w(bus_wo),
        .reg_wr(reg_wr),
        
        .ra(ra_addr),
        .bus_a(bus_a),
        
        .rb(rb_addr),
        .bus_b(bus_b)
        );
        
    
    alu_top alu_top0(
        .regA_i(bus_ai), 
        .regB_i(bus_bi), 
        .alu_ctr(alu_ctr), 
        .res_o(bus_w), 
        .zero(zero)
        );
    
    
    ie ie0(
        .instr(inst),
        .ext_op(ext_op),
        .imm(imm)
    );
    
    
    data_mem data_mem0(
        .rst_n(rst_n),
        .clk(clk),
    
        .addr(bus_w),      
        .data_in(bus_b),   
        .mem_wr(mem_wr),    
        
        .data_out(data_out)  
    );
    
    
    next_pc next_pc0(
        .ce(ce),
        .pc(cur_pc),
        .imm(imm),
        .branch(branch),
        .zero(zero),
        .jump(jump),
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
        .cur_pc(cur_pc),
        .ce(ce)
        );
    
    
    mux_alu_bsrc mux_alu_bsrc0(
        .alu_bsrc(alu_bsrc),
        .bus_b(bus_b),
        .imm(imm),
        
        .bus_bo(bus_bi)
        );
    
    
    mux_alu_asrc mux_alu_asrc0(
        .alu_asrc(alu_asrc),
        .bus_a(bus_a),
        .pc(cur_pc),
        
        .bus_ao(bus_ai)
        );
        
    
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
    
    
    assign bus_wo = mem_to_reg ? data_out : bus_w;

endmodule
