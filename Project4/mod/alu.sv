
module alu(
    input clk,
    input reset,

    input[63:0] rd_val,
    input[63:0] rs_val,
    input[63:0] rt_val,
    input[11:0] imm,
    input [4:0]opcode,
    output [63:0] result,
    output [1:0] ready_bit
)
    reg[1:0] ready;
    reg[63:0] temp;
    reg[63:0] temp2;

    real A;
    real B;



localparam ADD = 5'b00000; 
localparam ADDI = 5'b00001;   
localparam SUB = 5'b00010;  
localparam SUBI  = 5'b00011;
localparam MUL  = 5'b00100;
localparam DIV  = 5'b00101;

localparam AND = 5'b00110;
localparam OR  = 5'b00111;
localparam XOR = 5'b01000;
localparam NOT = 5'b01001;

localparam SHFTR  = 5'b01010;
localparam SHFTRI = 5'b01011;
localparam SHFTL  = 5'b01100;
localparam SHFTLI = 5'b01101;

localparam ADDF = 5'b11001;
localparam SUBF = 5'b11010;  
localparam MULF = 5'b11011;
localparam DIVF  = 5'b11100;



    always @(posedge clk) begin      
    case (opcode)
        ADD: 
            temp <= rs_val + rt_val;
            ready <= 1;
        ADDI: 
            temp <= rd_val + imm;
            ready <= 1;
        SUB: 
            temp <= rs_val - rt_val;
            ready <= 1;
        SUBI: 
            temp <= rd_val - imm;
            ready <= 1;
        MUL: 
            temp <= rs_val * rt_val;
            ready <= 1;
        DIV: 
            temp <= rs_val / rt_val;
            ready <= 1;
        AND: 
            temp <= rs_val & rt_val;
            ready <= 1;
        OR:  
            temp <= rs_val | rt_val;
            ready <= 1;
        XOR:    
            temp <= rs_val ^ rt_val;
            ready <= 1;
        NOT:    
            temp <= ~rs_val;
            ready <= 1;

        SHFTR: 
            temp <= rs_val >> rt_val;
            ready <= 1;
        SHFTRI: 
            temp <= rs_val >> imm;
            ready <= 1;
        SHFTL: 
            temp <= rs_val << rt_val;
            ready <= 1;
        SHFTLI: 
            temp <= rs_val << imm;
            ready <= 1;

        ADDF: 
            temp <= rs_val;
            temp2 <= rt_val;
            A <= $bitstoreal(temp);
            B <= $bitstoreal(temp2);
            A <= A + B;
            temp <= $realtobits(A);
            ready <= 1;

        SUBF:
            temp <= rs_val;
            temp2 <= rt_val;
            A <= $bitstoreal(temp);
            B <= $bitstoreal(temp2);
            A <= A - B;
            temp <= $realtobits(A);
            ready <= 1;
        MULF:
            temp <= rs_val;
            temp2 <= rt_val;
            A <= $bitstoreal(temp);
            B <= $bitstoreal(temp2);
            A <= A * B;
            temp <= $realtobits(A);
            ready <= 1;
        DIVF:
            temp <= rs_val;
            temp2 <= rt_val;
            A <= $bitstoreal(temp);
            B <= $bitstoreal(temp2);
            A <= A / B;
            temp <= $realtobits(A);
            ready <= 1;
    endcase
    assign result = temp;

end





endmodule