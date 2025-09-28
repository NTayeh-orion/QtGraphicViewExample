module async_register  (
    input [WIDTH-1:0] d,
    input load,
    output reg [WIDTH-1:0] q
);  /* Asynchronous loadable register */
  always @(posedge load) q <= d;
endmodule
