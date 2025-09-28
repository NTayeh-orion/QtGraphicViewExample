module dual_rail_encoder (
    input data,
    output wire p,
    n
);  /* Dual-rail encoding for delay-insensitive circuits */
  assign p = data;
  assign n = ~data;
endmodule
