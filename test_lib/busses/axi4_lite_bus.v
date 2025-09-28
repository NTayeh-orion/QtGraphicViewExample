module axi4_lite_slave (
    input aclk,
    input aresetn,
    input [31:0] awaddr,
    input awvalid,
    output reg awready,
    input [31:0] wdata,
    input wvalid,
    output reg wready,
    output reg [1:0] bresp,
    output reg bvalid,
    input bready,
    input [31:0] araddr,
    input arvalid,
    output reg arready,
    output reg [31:0] rdata,
    output reg rvalid,
    output reg [1:0] rresp,
    input rready
);
endmodule
