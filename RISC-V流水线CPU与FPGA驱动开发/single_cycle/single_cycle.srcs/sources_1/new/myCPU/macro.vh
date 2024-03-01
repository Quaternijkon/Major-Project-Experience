
// opcode
`define OPCODE_R    7'b0110011
`define OPCODE_I    7'b0010011
`define OPCODE_L    7'b0000011
`define OPCODE_S    7'b0100011
`define OPCODE_B    7'b1100011
`define OPCODE_JAL  7'b1101111
`define OPCODE_LUI  7'b0110111

// funct3
`define FUNCT3_ADDSUB   3'b000
`define FUNCT3_ORI      3'b110
`define FUNCT3_LW       3'b010
`define FUNCT3_SW       3'b010
`define FUNCT3_BEQ      3'b000

// funct7
`define FUNCT7_ADD  7'b0000000
`define FUNCT7_SUB  7'b0100000

// inst_id
`define ID_NULL     0
`define ID_ADD      1
`define ID_SUB      2
`define ID_ORI      3
`define ID_LUI      4
`define ID_SW       5
`define ID_LW       6
`define ID_BEQ      7
`define ID_JAL      8

// alu_op
`define ALU_NULL    3'b000
`define ALU_ADD     3'b001
`define ALU_SUB     3'b010
`define ALU_OR      3'b100

// branch
`define BR_NULL     2'b00
`define BR_BEQ      2'b01
`define BR_JAL      2'b10