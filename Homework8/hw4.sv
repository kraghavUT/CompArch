module hw4(
    integer test_num,
    reg clk,
    input reg[2:0] a,
    input reg[2:0] b, 
    input reg[2:0] c,
    //reg[2:0] expected,
    output reg [2:0] out_a,
    output reg [2:0] out_b,
    output reg [2:0] out_c
);

    assign out_a = (a > b) && (a > c);
    assign out_b = (b > a) && (b > c);
    assign out_c = (c > a) && (c > b);

endmodule