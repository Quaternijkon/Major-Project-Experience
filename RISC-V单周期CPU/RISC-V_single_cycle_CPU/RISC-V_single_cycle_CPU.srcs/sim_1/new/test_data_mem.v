`timescale 1ns / 1ps

module test_data_mem;

    reg clk;
    reg rst_n;
    
    reg [31:0]       addr;      
    reg [31:0]       data_in;   
    reg              mem_wr;    
    
    wire [31:0]      data_out;  

    initial begin
        clk = 0;
        rst_n = 0;
        #100;
        rst_n = 1'b1;
        mem_wr = 1'b1;
        
        addr = 32'b0000_0000_0000_0000_0000_0000_0000_0001;
        data_in = 32'b0000_0000_0000_0000_0000_0000_0000_0001;

        #100
        mem_wr = 1'b0;
        addr = 32'b0000_0000_0000_0000_0000_0000_0000_0001;

        
    end
    always #20 clk = ~clk;
    
    data_mem data_mem0(
        .rst_n(rst_n),
        .clk(clk),
    
        .addr(addr),      
        .data_in(data_in),   
        .mem_wr(mem_wr),    
        
        .data_out(data_out)  
    );

endmodule
