module ALU20 (
    input clk, input rst_n, 
    input alu_en, input carry_in, input cmp_signed, input   [3:0] alu_op, input   [4:0] shamt, input   
    [WIDTH-1:0] src_a, input src_b, input imm_val,output  [WIDTH-1:0] alu_result, output debug_bus, 
    output  zero_flag, output carry_flag, output ovf_flag, output neg_flag, output cmp_result, 
    output  [7:0] status_code, output  valid_out, output busy
    );
 
    // Internal signals
    logic [WIDTH:0] add_sub_tmp;
 
    always_comb begin
        alu_result   = '0;
        debug_bus    = '0;
        zero_flag    = 1'b0;
        carry_flag   = 1'b0;
        ovf_flag     = 1'b0;
        neg_flag     = 1'b0;
        cmp_result   = 1'b0;
        status_code  = 8'h00;
        valid_out    = alu_en;
        busy         = 1'b0;
 
        case (alu_op)
            4'b0000: begin // ADD
                add_sub_tmp = src_a + src_b + carry_in;
                alu_result  = add_sub_tmp[WIDTH-1:0];
                carry_flag  = add_sub_tmp[WIDTH];
                ovf_flag    = (src_a[WIDTH-1] == src_b[WIDTH-1]) &&
                              (alu_result[WIDTH-1] != src_a[WIDTH-1]);
            end
            4'b0001: begin // SUB
                add_sub_tmp = src_a - src_b - carry_in;
                alu_result  = add_sub_tmp[WIDTH-1:0];
                carry_flag  = add_sub_tmp[WIDTH];
                ovf_flag    = (src_a[WIDTH-1] != src_b[WIDTH-1]) &&
                              (alu_result[WIDTH-1] != src_a[WIDTH-1]);
            end
            4'b0010: alu_result = src_a & src_b; // AND
            4'b0011: alu_result = src_a | src_b; // OR
            4'b0100: alu_result = src_a ^ src_b; // XOR
            4'b0101: alu_result = src_a << shamt; // SLL
            4'b0110: alu_result = src_a >> shamt; // SRL
            4'b0111: alu_result = $signed(src_a) >>> shamt; // SRA
            4'b1000: alu_result = src_a + imm_val; // ADDI
            4'b1001: begin // Compare
                if (cmp_signed)
                    cmp_result = ($signed(src_a) < $signed(src_b));
                else
                    cmp_result = (src_a < src_b);
                alu_result = {31'b0, cmp_result};
            end
            default: alu_result = '0;
        endcase
 
        zero_flag = (alu_result == '0);
        neg_flag  = alu_result[WIDTH-1];
        debug_bus = alu_result;
        status_code = {4'h0, alu_op};
    end
 
endmodule