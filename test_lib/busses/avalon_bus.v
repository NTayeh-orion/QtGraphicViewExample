module avalon_mm_bus (
    input clk,
    reset_n,
    input read,
    write,
    input [31:0] address,
    writedata,
    input [3:0] byteenable,
    output reg [31:0] readdata,
    output reg waitrequest,
    readdatavalid
);
endmodule
