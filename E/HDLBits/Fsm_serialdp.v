module top_module(
    input clk,
    input in,
    input reset,    // Synchronous reset
    output [7:0] out_byte,
    output done
); //

    // 00 IDLE, 01 START, 10 DONE, 11 WAIT
    // 100 PARITY_FAIL
    reg [2:0] state, next;
    reg [3:0] cnt;
    reg [7:0] data;
    reg [7:0] data_reg;
    reg parity_reset;
    wire odd;
    
    always @ (posedge clk) begin
        if (reset) cnt <= 4'd0;
        else if (state == 3'b001 && cnt == 4'd8) cnt <= 4'd8;
        else if (state == 3'b001) cnt <= cnt + 4'd1;
        else cnt <= 4'd0;
    end
    
    always @ (posedge clk) begin
        if (reset) data <= 8'd0;
        else data <= {in, data[7:1]}; 
    end
    
    always @ (posedge clk) begin
        if (reset) data_reg <= 8'd0;
        else if (next == 3'b100) data_reg <= data; 
    end
    
    always @ (posedge clk) begin
        if (reset) parity_reset <= 1'b1;
        else if (next == 3'b001) parity_reset <= 1'b0;
        else parity_reset <= 1'b1;
    end
    
    always @ (*) begin
        case(state)
            3'b000: 
                if (!in) next = 3'b001;
            	else next = 3'b000;
            3'b001:
                if ((in ^ odd) & (cnt == 4'd8)) next = 3'b100;
            	else if (cnt == 4'd8) next = 3'b011;
            	else next = 3'b001;
            3'b010:
                if (!in) next = 3'b001;
            	else next = 3'b000;
            3'b011:
                if (in) next = 3'b000;
            	else next = 3'b011;
            3'b100:
                if (in) next = 3'b010;
            	else next = 3'b011;
            default:
                next = 3'b000;
        endcase
    end
    
    always @ (posedge clk) begin
        if (reset) state <= 3'b000;
        else state <= next;
    end
    
    assign done = state == 3'b010;
    assign out_byte = data_reg;

    parity u_parity(.clk(clk), .in(in), .reset(parity_reset), .odd(odd));
endmodule

