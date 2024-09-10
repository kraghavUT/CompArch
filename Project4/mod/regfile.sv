module regfile(
    input clk,
    input reset,
    
    input [1:0] write_enable,

    input [4:0] selector1_rs,
    input [4:0] selector2_rt,
    input [4:0] writeAddr,
    input [63:0] writeData,

    output[4:0] rs_val,
    output[4:0] rt_val
)

    reg [31:0] registers [63:0];

    always @(posedge clk) begin
    if (reset) begin
      // Reset all registers to 0
      for (int i = 0; i < 32; i = i + 1)
        registers[i] <= 64'b0;
    end else begin
      assign rs_val = registers[selector1_rs];
      assign rd_val = registers[selector2_rt];
      
      // Write operation
      if(write_enable == 0)begin
        if (writeAddr != 0) begin
        registers[writeAddr] <= writeData;
        end
      end

    end
  end

endmodule
