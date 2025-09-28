module apb3_bus (
    input pclk,
    presetn,
    input psel,
    penable,
    pwrite,
    input [31:0] paddr,
    prdata,
    input [31:0] pwdata,
    output reg [31:0] prdata_out,
    output reg pready,
    pslverr
);  // APB3 bus slave interface
endmodule
