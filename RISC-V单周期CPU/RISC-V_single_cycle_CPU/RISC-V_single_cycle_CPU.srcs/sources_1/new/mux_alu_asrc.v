`timescale 1ns / 1ps

module mux_alu_asrc(
    input           alu_asrc,
    input [31:0]    bus_a,
    input [31:0]    pc,

    output reg[31:0]    bus_ao
    );
    
    always @ (*) begin 
    
        case(alu_asrc)
            1'b0 : begin   
                bus_ao <= bus_a;
            end
            
            1'b1 : begin   
                bus_ao <= pc;
            end
            
        endcase
        
    end
    
endmodule
