`timescale 1ns / 1ps

module alu(
    
    input [31:0]         regA_i,
    input [31:0]         regB_i,
    
    input                sub_ctr_i,
    input                sig_ctr_i,
    input [1:0]          op_ctr_i,
    
    output   zf, 
    output   sf, 
    output   cf, 
    output   of, 
    
    output reg           cout,
    output reg [31:0]    result_o
    
    );

reg [31:0] result;
reg [31:0] regB_ii;

    
always @ (*) begin

    case(op_ctr_i)
        
        2'b00, 2'b11: begin    
            
            
            if (sub_ctr_i) begin
                regB_ii <= ~regB_i;
            end       
            else begin
                regB_ii <= regB_i;
            end
            
            {cout, result} <= regA_i + regB_ii + sub_ctr_i;  

                        
            if (op_ctr_i == 2'b00) begin
                result_o <= result;
            end
            
            else if(op_ctr_i == 2'b11) begin
                
                if (sig_ctr_i) begin 
                    result_o <= {31'b0, (of ^ sf)}; 
                end
                
                else begin
                    result_o <= {31'b0, (sub_ctr_i ^ cf)};
                end                
            
            end
                         
        end 
        
        2'b01: begin    
            result_o <= regA_i | regB_i;
        end
        
        2'b10: begin    
            result_o <= regB_i;
        end
        
        
        
        
    endcase
    
end

assign zf = (result == 32'b0 ? 1'b1 : 1'b0); 
assign sf = result[31];   
assign cf = (sub_ctr_i ? ~cout : cout); 


assign of = ((regA_i[31] == regB_ii[31]) && (regA_i[31] != result[31]) ? 1'b1 : 1'b0 ); 
    
    
endmodule
