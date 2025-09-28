module spi_master_interface (
    input clk,
    rst_n,
    input sck,
    cs_n,
    input [7:0] mosi,
    output reg [7:0] miso,
    output reg sck_out,
    cs_n_out,
    mosi_out
);  // SPI master interface signals
endmodule
