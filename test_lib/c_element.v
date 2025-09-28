module c_element (
    input a,
    b,
    output reg c
);  /* Muller C-element for asynchronous control */
  always @(*)
    case ({
      a, b
    })
      2'b00:   c <= 0;
      2'b11:   c <= 1;
      default: ;
    endcase
endmodule
