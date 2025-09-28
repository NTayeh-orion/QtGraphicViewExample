module async_adder  (
    input [WIDTH-1:0] a,
    b,
    input start,
    output reg [WIDTH-1:0] sum,
    output reg done
);  /* Asynchronous ripple-carry adder */
  reg carry;
  reg [WIDTH-1:0] temp_sum;
  integer i;
  always @(posedge start) begin
    carry = 0;
    done  = 0;
    for (i = 0; i < WIDTH; i = i + 1) begin
      temp_sum[i] = a[i] ^ b[i] ^ carry;
      carry = (a[i] & b[i]) | (a[i] & carry) | (b[i] & carry);
    end
    sum  = temp_sum;
    done = 1;
  end
endmodule
