module testbench_ALU32;
    reg [31:0] in0, in1;
    reg [10:0] op;
    wire [31:0] out;
    wire CF,OF,ZF,PF,SF;
  
    ALU32 dut(.in0(in0), .in1(in1), .op(op), .out(out), .CF(CF), .OF(OF), .ZF(ZF), .PF(PF), .SF(SF));
    
    initial begin
        $display("ALU32 Testbench Started");
        #10;
        in0 = 32'h20000000;
        in1 = 32'h20000000;
        op = 11'b00000100000; // add
        #10;
        $display("Add: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100001; // addu
        #10;
        $display("Addu: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100010; // sub
        #10;
        $display("Sub: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100011; // subu
        #10;
        $display("Subu: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100100; // and
        #10;
        $display("And: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100101; // or
        #10;
        $display("Or: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100110; // xor
        #10;
        $display("Xor: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 32'h20000001;
        op = 11'b00000100111; // nor
        #10;
        $display("Nor: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h80000000;
        in1 = 32'h7FFFFFFF;
        op = 11'b00000101010; // slt
        #10;
        $display("SLT: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h80000000;
        in1 = 32'h7FFFFFFF;
        op = 11'b00000101011; // sltu
        #10;
        $display("SLTU: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'h20000000;
        in1 = 5;
        op = 11'b00000000100; // shl
        #10;
        $display("SHL: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'hA0000000;
        in1 = 3;
        op = 11'b00000000110; // shr
        #10;
        $display("SHR: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        in0 = 32'hA0000000;
        in1 = 3;
        op = 11'b00000000111; // sar
        #10;
        $display("SAR: in0=%h, in1=%h, out=%h, CF=%b, OF=%b, ZF=%b, PF=%b, SF=%b", in0, in1, out, CF, OF, ZF, PF, SF);
        
        $display("ALU32 Testbench Finished");
        $finish;
    end
endmodule