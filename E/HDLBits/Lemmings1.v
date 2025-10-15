module top_module(
    input clk,
    input areset,    // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    output walk_left,
    output walk_right); //  

    // parameter LEFT=0, RIGHT=1, ...
    reg state, next_state;

    always @(*) begin
        // State transition logic
        case(state)
            1'b0: if (bump_left)  next_state = 1'b1;
                  else            next_state = 1'b0;
            1'b1: if (bump_right) next_state = 1'b0;
                  else            next_state = 1'b1;
        endcase
    end

    always @(posedge clk, posedge areset) begin
        // State flip-flops with asynchronous reset
        if (areset) state <= 1'b0;
        else        state <= next_state;
    end

    // Output logic
    assign walk_left = (state == 1'b0);
    assign walk_right = (state == 1'b1);

endmodule

