`timescale 1ns / 1ps

module reg_file(
    input              rst_n,
    input              clk,

    input [4:0]        rw,      
    input [31:0]       bus_w,   
    input              reg_wr,  

    input [4:0]        ra,      
    
    output reg [31:0] bus_a,   

    input [4:0]        rb,      
    
    output reg [31:0] bus_b    
);

    reg [31:0] mem_r [0:31];
    
initial begin
    mem_r[5'b00001] = 32'b0000_0000_0000_0000_0000_0000_0000_0100;
    
end

    
always @ (posedge clk or negedge rst_n) begin      
    if(rst_n) begin
        
        if((rw != 5'b0) && (reg_wr))
            mem_r[rw] <= bus_w;
    end
end

always @ (*) begin                                 
    if(~rst_n)
        bus_a <= 32'b0;
    
    
    else if(ra == 5'b0)
        bus_a <= 32'b0;




    else
        bus_a <= mem_r[ra];

    end

always @ (*) begin                                 
    if(~rst_n)
        bus_b <= 32'b0;
    else if(rb == 5'b0)
        bus_b <= 32'b0;




    else
        bus_b <= mem_r[rb];

    end

endmodule

