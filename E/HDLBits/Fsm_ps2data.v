module top_module(
    input clk,
    input [7:0] in,
    input reset,    // Synchronous reset
    output [23:0] out_bytes,
    output done); //

    // FSM from fsm_ps2

    // 00 IDLE->01 BYTE1->10 BYTE2->11 BYTE3
    reg [1:0] state, next;
    // State transition logic (combinational)
    always @(*) begin
        case (state)
            2'b00:
                if (in[3]) next = 2'b01;
            else next = 2'b00;
            2'b01:
                next = 2'b10;
            2'b10:
                next = 2'b11;
            2'b11:
                if (in[3]) next = 2'b01;
                else next = 2'b00;
        endcase
    end
    // State flip-flops (sequential)
    always @(posedge clk) begin
        if (reset) state <= 2'b00;
        else state <= next;
    end
    // Output logic
    assign done = state == 2'b11;

    // New: Datapath to store incoming bytes.
    always @(posedge clk) begin
        if (reset) out_bytes <= 24'd0;
        else if (next == 2'b01) out_bytes[23:16] <= in;
        else if (next == 2'b10) out_bytes[15:8] <= in;
        else if (next == 2'b11) out_bytes[7:0] <= in;
    end
endmodule

