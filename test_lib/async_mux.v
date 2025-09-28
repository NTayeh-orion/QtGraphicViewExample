module async_mux  (
    input [WIDTH-1:0] in0,
    in1,
    input sel,
    output reg [WIDTH-1:0] out
);  /* Asynchronous multiplexer with completion */
  always @(in0 or in1 or sel) out = sel ? in1 : in0;
endmodule
