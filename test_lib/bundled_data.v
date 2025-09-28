module bundled_data_interface (
    input data_valid,
    input [7:0] data_in,
    output reg data_ack,
    output reg [7:0] data_out
);  /* Bundled-data interface with request/acknowledge */
  reg internal_valid;
  always @(posedge data_valid) begin
    data_out <= data_in;
    internal_valid <= 1;
  end
  always @(negedge data_valid)
    if (internal_valid) begin
      data_ack <= 1;
      internal_valid <= 0;
    end
  always @(posedge data_ack) data_ack <= 0;
endmodule
