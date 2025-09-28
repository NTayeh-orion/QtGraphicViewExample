module custom_simple_bus (
    input clk,
    rst_n,
    input req,
    input wr,
    input [15:0] addr,
    input [31:0] wdata,
    output reg [31:0] rdata,
    output reg ack
);  // Minimal custom bus for educational use
endmodule
