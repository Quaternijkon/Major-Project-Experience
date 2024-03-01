`timescale 1ns / 1ps

module ie(
    input [31:0]             instr,
    input [2:0]              ext_op,
    output reg [31:0]        imm
    ); 

always @ (*) begin

    case(ext_op)
        3'b000: begin 
            imm <= {{20{instr[31]}} , instr[31:20]};
        end
        3'b001: begin 
            imm <= {instr[31:12], 12'b0};
        end
        3'b010: begin 
            imm <= {{20{instr[31]}}, instr[31:25], instr[11:7]};
        end
        3'b011: begin 
            imm <= {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
        end
        3'b100: begin 
            imm <= {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
        end
        default: begin
        end
    endcase
    
end

endmodule
