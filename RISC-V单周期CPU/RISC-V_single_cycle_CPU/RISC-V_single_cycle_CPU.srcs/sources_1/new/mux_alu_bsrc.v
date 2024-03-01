`timescale 1ns / 1ps

module mux_alu_bsrc(
    input [1:0]     alu_bsrc,
    input [31:0]    bus_b,
    input [31:0]    imm,
    
    output reg[31:0]    bus_bo
    );
    
    always @ (*) begin 
    
        case(alu_bsrc)
            2'b00 : begin   
                bus_bo <= bus_b;
            end
            
            2'b01 : begin   
                bus_bo <= 4'h4;
            end
            
            2'b10 : begin   
                bus_bo <= imm;
            end
            
        endcase
        
    end
    
endmodule
