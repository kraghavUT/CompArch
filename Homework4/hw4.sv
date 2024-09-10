module find_max(
    input logic [2:0] a, 
    input logic [2:0] b, 
    input logic [2:0] c, 
    output logic [2:0] max
);

reg clk;
initial
begin
    clk = 1'b0;
    forever begin
        # 1 clk = !clk; // forever, toggle the clock every ns
    end
end

always @(posedge clk) begin
    
    if(a >=b && a >= c)begin
        max[2] = 1;
        max[1] = 0;
        max[0] = 0;
    end
    else if(b >= a && b >= c) begin
        max[2] = 0;
        max[1] = 1;
        max[0] = 0;
    end
    else begin
        max[2] = 0;
        max[1] = 0;
        max[0] = 1;
    end
end


endmodule
