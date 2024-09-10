// module divider(
//     input wire[31:0] dividend,
//     input wire[31:0] divisor, 
//     input clk, 
//     output wire[31:0] quotient, 
//     output wire[31:0] remainder
//     output reg divZeroFlag
// );

// always @(*) begin
//         if (divisor == 0) begin
//             quotient = 32'hxxxx_xxxx; // Set quotient to all 'x's to indicate undefined value
//             remainder = 32'hxxxx_xxxx; // Set remainder to all 'x's to indicate undefined value
//             divZeroFlag = 1; // Set division_by_zero flag
//         end 
//         else begin
//             divZeroFlag = 0; // Clear division_by_zero flag
//             // Initialize dividend and quotient registers
//             reg [63:0] temp_dividend;
//             reg [31:0] temp_quotient;
//             temp_dividend = {dividend, 32'h0}; // Left-extend dividend to 64 bits
//             temp_quotient = 32'h0; // Initialize quotient to 0

//             // Perform division algorithm
//             for (int i = 31; i >= 0; i = i - 1) begin
//                 temp_dividend = temp_dividend << 1; // Left shift dividend
//                 temp_quotient = temp_quotient << 1; // Left shift quotient
//                 if (temp_dividend[63:32] >= divisor) begin
//                     temp_dividend = temp_dividend - divisor; // Subtract divisor from dividend
//                     temp_quotient[i] = 1; // Set quotient bit
//                 end
//             end

//             // Assign final quotient and remainder values
//             quotient = temp_quotient;
//             remainder = temp_dividend[63:32];
//         end
//     end

// endmodule


module factorial(
    input wire [31:0] val, 
    output wire[31:0] factorial, 
    input clk, 
    output wire ready, 
    output wire overflow);

    reg [31:0] temp_result;
    reg [31:0] i;
    
    // Define states
    parameter IDLE = 2'd0;
    parameter COMPUTE = 2'd1;
    parameter DONE = 2'd2;
    reg [1:0] state;


    // State machine
    always @(posedge clk) begin
        case (state)
            IDLE: begin
                if (val == 0) begin
                    assign wire factorial <= 1; // Factorial of 0 is 1
                    assign wire overflow <= 0;
                    assign wire ready <= 1;
                    state <s= DONE;
                end else begin
                    temp_result <= 1;
                    i <= 1;
                    ready <= 0;
                    state <= COMPUTE;
                end
            end
            COMPUTE: begin
                if (i <= val) begin
                    temp_result <= temp_result * i;
                    i <= i + 1;
                    if (temp_result < i) begin
                        overflow <= 1; // Overflow detected
                        ready <= 1;
                        state <= DONE;
                    end
                end else begin
                    factorial <= temp_result;
                    ready <= 1;
                    state <= DONE;
                end
            end
            DONE: begin
                // Do nothing
            end
        endcase
    end

endmodule


module average(input wire [31:0] in, 
input clk,
output wire [31:0] avg, 
output ready, 
output overflow);

endmodule


module stream(input wire [31:0] first_stream, 
input wire [31:0] second_stream, 
input clk, 
output wire[31:0] running_sum);



endmodule



