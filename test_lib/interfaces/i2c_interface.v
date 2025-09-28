module i2c_interface (
    input clk,
    rst_n,
    inout sda,
    inout scl,
    input [7:0] data_out,
    output reg [7:0] data_in,
    input start,
    output reg done
);  // I2C master interface with tri-state SDA/SCL
endmodule
