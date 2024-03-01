`timescale 1ns / 1ps

module test_j_inst;
 
    reg clk;
    reg rst_n;
    
    
    reg [4:0]        rw;       
    wire [31:0]      bus_w;   
    reg              reg_wr;  

    reg [4:0]        ra;      
    reg              re1;     
    wire [31:0]      bus_a;          

    reg [4:0]        rb;      
    reg              re2;     
    wire [31:0]      bus_b;          
    
    
    reg [3:0]          alu_ctr;
    
    
    
    wire               zero;
    
    
    
    reg [31:0]             instr;
    reg [2:0]              ext_op;
    wire [31:0]            imm;
    
    reg [1:0]              alu_bsrc; 
    wire [31:0]            bus_bi;   
    
    reg                    alu_asrc; 
    wire [31:0]            bus_ai;   
    
    
    
    
    reg              mem_wr;    
    
    wire [31:0]      data_out;  
    
    reg             mem_to_reg; 
    wire [31:0]     bus_wo;     
    
    
    
    wire [31:0]    cur_pc;
    
    reg           branch;
    reg           jump;
    

    wire [31:0]    next_pc;

    reg              ce;
    
    wire [31:0]     inst;
    
        
    initial begin
        clk = 0;
        rst_n = 0;
        
        #100;
        rst_n = 1'b1;
        mem_wr = 1'b0;  
        re1 = 1'b1;
        re2 = 1'b1;
        
        #50  

        alu_ctr = 4'b0000;  
        ext_op = 3'b100;    
        branch = 1'b1;      
        
        reg_wr = 1'b1;      
        rw = 5'b00011;
        mem_to_reg = 1'b0;  

        jump = 1'b1;        
        alu_asrc = 1'b1;    
        alu_bsrc = 2'b01;   
        
        ra = 5'b00001;      
        rb = 5'b00001;      
        
        instr = 32'b0000_0000_1000_0000_0000_0000_0000_0000; 
        
        #50
        ra = 5'b00011;      
        
    end
    always #20 clk = ~clk;
    
    
    reg_file reg_file0(
        .rst_n(rst_n),
        .clk(clk),
        
        .rw(rw),
        .bus_w(bus_wo),
        .reg_wr(reg_wr),
        
        .ra(ra),
        .re1(re1),
        .bus_a(bus_a),
        
        .rb(rb),
        .re2(re2),
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
        .instr(instr),
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
        .cur_pc(cur_pc)
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
    
    
    
    assign bus_wo = mem_to_reg ? data_out : bus_w;
    

    
endmodule
