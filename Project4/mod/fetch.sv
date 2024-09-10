module fetch(
    input clk,
    input reset,
    input [63:0] out_of_order_seq,
    input ctrl_signal,
    output [31:0] instruction,
    output error
)

    ram r1(
        .clk(clk),
        .reset(reset),
        .rw_write_en = 0,
        .r_data_out(instruction),
        .r_error(error),
    );

    assign instruction = r_data_out;
    assign error = r_error;
endmodule


