module uart_interface (
    input clk,
    rst_n,
    input tx_start,
    input [7:0] data_in,
    output reg tx,
    output reg tx_done,
    input rx,
    output reg [7:0] rx_data,
    output reg rx_valid
);  // Simplified UART interface with basic TX/RX
endmodule
