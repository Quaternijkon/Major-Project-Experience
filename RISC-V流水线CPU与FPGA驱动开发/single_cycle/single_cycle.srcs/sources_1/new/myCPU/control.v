`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2020/11/19 21:13:55
// Design Name: 
// Module Name: control
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`include "macro.vh"

module control(
//    input           clk,
//    input           rst,

    input   [6:0]   opcode,
    input   [2:0]   funct3,
    input   [6:0]   funct7,
    
    output          c1,         // 
    output          c2,         // 
    output  [1:0]   c3,         // 
    output  [2:0]   alu_op,     // 
    output  [1:0]   branch,
    output          dmem_we,
    output          reg_we
);

    wire [3:0] inst_id = get_inst_id(opcode, funct3, funct7);
    function [3:0] get_inst_id(input [6:0] opcode, input [2:0] funct3, input [6:0] funct7);
    begin
        case (opcode)
            `OPCODE_R   : begin
                case (funct3)
                    `FUNCT3_ADDSUB  : begin
                        case (funct7)
                            `FUNCT7_ADD : get_inst_id = `ID_ADD;
                            `FUNCT7_SUB : get_inst_id = `ID_SUB;
                            default     : get_inst_id = `ID_NULL;
                        endcase
                    end
                    default     : get_inst_id = `ID_NULL;
                endcase
            end
            `OPCODE_I   : begin
                case (funct3)
                    `FUNCT3_ORI : get_inst_id = `ID_ORI;
                    default     : get_inst_id = `ID_NULL;
                endcase
            end
            `OPCODE_L   : begin
                case (funct3)
                    `FUNCT3_LW  : get_inst_id = `ID_LW;
                    default     : get_inst_id = `ID_NULL;
                endcase
            end
            `OPCODE_S   : begin
                case (funct3)
                    `FUNCT3_SW  : get_inst_id = `ID_SW;
                    default     : get_inst_id = `ID_NULL;
                endcase
            end
            `OPCODE_B   : begin
                case (funct3)
                    `FUNCT3_BEQ : get_inst_id = `ID_BEQ;
                    default     : get_inst_id = `ID_NULL;
                endcase
            end
            `OPCODE_JAL : get_inst_id = `ID_JAL;
            `OPCODE_LUI : get_inst_id = `ID_LUI;
            default     : get_inst_id = `ID_NULL;
        endcase
    end
    endfunction

    assign c1 = (inst_id == `ID_SW ) ? 1 : 0;
    
    reg [8:0] mask_c2 = 9'b000000110;
    assign c2 = mask_c2[inst_id];
    
    assign c3 = get_c3(inst_id);
    function [1:0] get_c3(input [3:0] inst_id);
    begin
        case (inst_id)
            `ID_LUI, `ID_SW, `ID_BEQ  : get_c3 = 2'b00;
            `ID_JAL                   : get_c3 = 2'b01;
            `ID_ADD, `ID_SUB, `ID_ORI : get_c3 = 2'b10;
            `ID_LW                    : get_c3 = 2'b11;
            default                   : get_c3 = 2'b00;
        endcase
    end
    endfunction
    
    assign alu_op = get_alu_op(inst_id);
    function [3:0] get_alu_op(input [3:0] inst_id);
    begin
        case (inst_id)
            `ID_SW, `ID_LW, `ID_ADD : get_alu_op = `ALU_ADD;
            `ID_SUB                 : get_alu_op = `ALU_SUB;
            `ID_ORI                 : get_alu_op = `ALU_OR;
            default                 : get_alu_op = `ALU_NULL;
        endcase
    end
    endfunction
                   
    assign branch = get_branch(inst_id);
    function [1:0] get_branch(input [3:0] inst_id);
    begin
        case (inst_id)
            `ID_BEQ                 : get_branch = `BR_BEQ;
            `ID_JAL                 : get_branch = `BR_JAL;
            default                 : get_branch = `BR_NULL;
        endcase
    end
    endfunction
                   
    assign dmem_we = (inst_id == `ID_SW ) ? 1 : 0;
    
    reg [8:0] mask_reg_we = 9'b101011110;
    assign reg_we = mask_reg_we[inst_id];
    
endmodule
