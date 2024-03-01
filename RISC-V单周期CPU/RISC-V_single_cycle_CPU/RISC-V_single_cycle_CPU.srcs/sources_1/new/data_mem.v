`timescale 1ns / 1ps

module data_mem(
    input              rst_n,
    input              clk,

    input [31:0]       addr,      
    input [31:0]       data_in,   
    input              mem_wr,    
    
    output reg [31:0] data_out  
);

reg [31:0] mem_r [0:255];


initial $readmemh ("C:/Users/DOCTORY/Downloads/Compressed/CPU-Design-Based-on-RISC-V-master/project_single_cycle/project_single_cycle.srcs/sources_1/new/number.data", mem_r);

always @ (posedge clk or negedge rst_n) begin      
    if(rst_n) begin
        if((addr != 32'b0) && (mem_wr))
        
            mem_r[addr[9:2]] <= data_in;
    end
end

always @ (*) begin                                 
    if(~rst_n || addr == 32'b0)
        data_out <= 32'b0;
    else
        
        data_out <= mem_r[addr[9:2]];
end

endmodule
