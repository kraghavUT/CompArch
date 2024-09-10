module factorial(
    input wire [31:0] val,
    output wire[31:0] factorial,
    input clk, 
    output wire ready, 
    output wire overflow
);

    reg [31:0] result;
    reg [31:0] counter;
    reg ready = 0;    
    
    always@(posedge clk) begin
        if (val == 1'b0 || val == 1'b1) begin
            result <= 1;
        end else begin
            result <= in;
            for (counter = val -1; counter!=1; counter = counter-1) begin
                result <= result * counter;
            end
        end
    end

    assign factorial = result;
    assign overflow = (result > 32'b1100)? 1: 0;
    assign ready = 1;
endmodule




module average(
    input wire [31:0] in,
    input clk,
    output wire [31:0] avg,
    output ready,
    output overflow
);

reg [31:0] sum = 0;
reg [31:0] tempSum = 0;
reg [2:0] count = 0;
reg overflowReg = 0;
reg readyReg = 0;

always @(posedge clk) begin
    tempSum <= sum + in;
    count <= count + 1;
    if (count == 6) begin
        if(tempSum < sum || tempSum < in)begin
            overflowReg <= 1;
            readyReg <= 1;
            sum <=1;
        end
        else begin
            sum <= tempSum;
            overflowReg <= 0;
            readyReg <= 1;
        end
    end
end
    assign avg = sum/6;
    assign ready = readyReg;
    assign overflow = overflowReg;
endmodule


module stream(
    input wire[31:0] first_stream, 
    input wire[31:0] second_stream, 
    input clk, 
    output wire[31:0] running_sum
);

reg[31:0] sum = 0;

always @(posedge clk) begin
    sum <= running_sum;
end
    assign running_sum = sum + first_stream + second_stream;

endmodule