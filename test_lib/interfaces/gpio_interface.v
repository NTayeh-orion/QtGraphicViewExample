module gpio_interface (
    input clk,
    rst_n,
    input [31:0] out_val,
    input [31:0] oe,
    output reg [31:0] gpio_out,
    input [31:0] gpio_in
);  // General-purpose I/O interface with output enable
endmodule
