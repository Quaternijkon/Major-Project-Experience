module alu(
op,in0,in1,
CF,OF,ZF,PF,SF, out
    );
    input [31:0] in0,in1;
    input [10:0] op;
    output reg [31:0] out;
    output reg CF,OF,ZF,PF,SF;
   
always@(*)
begin
    case(op)
        //add
        11'b00000100000:
            begin
            out=in0+in1;
            OF=((in0[31]==in1[31])&&(~out[31]==in0[31]))?1:0;
            ZF=(out==0)?1:0;
            CF=0;
            end
        //addu
        11'b00000100001:
            begin
            {CF,out}=in0+in1;
            ZF=(out==0)?1:0;
            OF=0;
            end
        //sub
        11'b00000100010:
            begin
            out=in0-in1;
            OF=((in0[31]==0&&in1[31]==1&&out[31]==1)||(in0[31]==1&&in1[31]==0&&out[31]==0))?1:0;
            ZF=(in0==in1)?1:0;
            CF=0;
            end
        //subu
        11'b00000100011:
            begin
            {CF,out}=in0-in1;
            ZF=(out==0)?1:0;
            OF=0;
            end
        //and
        11'b00000100100:
            begin
            out=in0&in1;
            ZF=(out==0)?1:0;
            CF=0;
            OF=0;
            end
        //or
        11'b00000100101:
            begin
            out=in0|in1;
            ZF=(out==0)?1:0;
            CF=0;
            OF=0;
            end
        //xor
        11'b00000100110:
            begin
            out=in0^in1;
            ZF=(out==0)?1:0;
            CF=0;
            OF=0;
            end
        //nor
        11'b00000100111:
            begin
            out=~(in0|in1);
            ZF=(out==0)?1:0;
            CF=0;
            OF=0;
            end
        //slt
        11'b00000101010:
            begin                        
            if(in0[31]==1&&in1[31]==0)
                out=1;
            else if(in0[31]==0&&in1[31]==1)
                out=0;
            else 
                out=(in0<in1)?1:0;
           OF=out; 
           ZF=(out==0)?1:0;
           CF=0;     
           end
        //sltu
        11'b00000101011:
            begin
                out=(in0<in1)?1:0;
                CF=out;
                ZF=(out==0)?1:0;
                OF=0;
            end
        //shl
        11'b00000000100:
            begin
            {CF,out}=in0<<in1;
            OF=0;
            ZF=(out==0)?1:0;
            end
        //shr
        11'b00000000110:
            begin
            out=in0>>in1;
            CF=in0[in1-1];
            OF=0;
            ZF=(out==0)?1:0;
            end
        //sar
        11'b00000000111:
            begin
            out=($signed(in0))>>>in1;
            CF=in0[in1-1];
            OF=0;
            ZF=(out==0)?1:0;
            end
        
    endcase

    assign PF = ~^out[7:0];
    assign SF = out[31];
end
endmodule