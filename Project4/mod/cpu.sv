`ifndef cpu_module
`define cpu_module

`include "mod/ram.sv"

/*
* CPU module
*
* This module is the CPU of the system. It is responsible for wiring together the different components of the system.
*
* @input clk: The clock signal
* @input reset: The reset signal
* @output halt: The halt signal
* @output in_signal: The input present signal. Set to 1 when reading from the input port
* @input in_data: The input data
* @output out_signal: The output present signal. Set to 1 when writing to the output port
* @output out_data: The output data
*/
module cpu (
    input clk,
    input reset,
    output logic halt,
    output logic error,
    output logic in_signal,
    input logic[63:0] in_data,
    output logic out_signal,
    output logic[63:0] out_data   
);  
    // RAM
    logic [63:0] r_addr;
    logic [63:0] rw_addr;
    logic [63:0] rw_data_in;
    logic rw_write_en;
    wire [31:0] r_data_out;
    wire r_error;
    wire [63:0] rw_data_out; 
    wire rw_error;
    
    ram memory (
        .clk(clk),
        .reset(reset),
        .r_addr(r_addr),
        .rw_addr(rw_addr),
        .rw_data_in(rw_data_in),
        .rw_write_en(rw_write_en),
        .r_data_out(r_data_out),
        .r_error(r_error),
        .rw_data_out(rw_data_out),
        .rw_error(rw_error)
    );

   
    input [63:0] out_of_order_seq,
//    output [31:0] instruction,
    output error;

    fetch fet(
        .clk(clk),
        .reset(reset),
        .out_of_order_seq(out_of_order_seq),
        .instruction(instruction),
        .error(error)
    );

    input [31:0]instruction,
    output [4:0] opcode,
    output [4:0] rd,
    output [4:0] rs,
    output [4:0] rt,
    output [11:0] imm;
    decode dec(
        .instruction(instruction),
        .opcode(opcode),
        .rd(rd),
        .rs(rs),
        .rt(rt),
        .imm(imm)
    );


    input [1:0] write_enable,

    input [4:0] selector1_rs,
    input [4:0] selector2_rt,
    input [4:0] writeAddr,
    input [63:0] writeData,

    output[4:0] rs_val,
    output[4:0] rt_val;


    regfile registersFile(
        .write_enable(write_enable),
        .selector1_rs(selector1_rs),
        .selector2_rt(selector2_rt),
        .writeAddr(writeAddr),
        .writeData(writeData),
        .rs_val(rs_val),
        .rt_val(rt_val)
    );

    input[63:0] rd_val,
    input[63:0] rs_val,
    input[63:0] rt_val,
    input[11:0] imm,
    input [4:0]opcode,
    output [63:0] result,
    output [1:0] ready_bit;

    alu a(
        .rd_val(rd_val),
        .rs_val(rs_val),
        .rt_val(rt_val),
        .imm(imm),
        .opcode(opcode),
        .result(result),
        .ready_bit(ready_bit)
    );









endmodule

`endif