module decode(
    input clk,
    input reset,

    input [31:0]instruction,
    
    output [4:0] opcode,
    output [4:0] rd,
    output [4:0] rs,
    output [4:0] rt,
    output [11:0] imm,
)

    fetch f1(
        .clk(clk),
        .reset(reset),
        .instruction(instruction)
    )


    assign opcode = instruction[31:27];
    assign rd = instruction[26:22];
    assign rs = instruction[21:17];
    assign rt = instruction[16:12];
    assign imm = instruction[11:0];

endmodule
