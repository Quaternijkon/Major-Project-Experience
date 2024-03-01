module inst_rom(
    input              ce,
    input [31:0]       addr,
    output reg [31:0]  inst
);

reg [31:0] inst_mem [0:255];


initial $readmemh ("C:/Users/DOCTORY/Downloads/Compressed/CPU-Design-Based-on-RISC-V-master/project_single_cycle/project_single_cycle.srcs/sources_1/new/bubble.data", inst_mem);

always @ (*) begin
    if(~ce)
        inst <= 0;  
    else
        inst <= inst_mem[addr[9:2]];
end

endmodule