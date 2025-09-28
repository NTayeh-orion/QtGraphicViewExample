module wishbone_bus (
    input clk,
    rst_n,
    input stb_i,
    cyc_i,
    we_i,
    input [31:0] adr_i,
    dat_i,
    input [3:0] sel_i,
    output reg ack_o,
    output reg [31:0] dat_o
);  // Wishbone B4 classic bus slave interface
endmodule
