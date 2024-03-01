`timescale 1ns / 1ps

module test_reg_file;

    reg clk;
    reg rst_n;
    
    reg [4:0]        rw;       
    wire [31:0]       bus_w;   
    reg              reg_wr;  

    reg [4:0]        ra;      
    reg              re1;     
    wire [31:0] bus_a;          

    reg [4:0]        rb;      
    reg              re2;     
    wire [31:0] bus_b;          
    
    reg [3:0]          alu_ctr;
    
    
    wire [31:0]        res_o;
    wire               zero;
        
    initial begin
        clk = 0;
        rst_n = 0;
        #100;
        rst_n = 1'b1;
        reg_wr = 1'b1;
        
        
        
        #100
        re1 = 1'b1;
        re2 = 1'b1;
        
        alu_ctr = 4'b0000;
        rw = 5'b00010;
        ra = 5'b00001;
        rb = 5'b00001;
        
        #100
        ra = 5'b00010;
        rb = 5'b00010;
        
    end
    always #20 clk = ~clk;
    
    reg_file reg_file0(
        .rst_n(rst_n),
        .clk(clk),
        
        .rw(rw),
        .bus_w(bus_w),
        .reg_wr(reg_wr),
        
        .ra(ra),
        .re1(re1),
        .bus_a(bus_a),
        
        .rb(rb),
        .re2(re2),
        .bus_b(bus_b)
        );
        
    alu_top alu_top0(
        .regA_i(bus_a), 
        .regB_i(bus_b), 
        .alu_ctr(alu_ctr), 
        .res_o(bus_w), 
        .zero(zero)
        );
    
endmodule
