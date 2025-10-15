module top_module(
    input clk,
    input areset,    // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    input ground,
    output walk_left,
    output walk_right,
    output aaah );
    
    // 00 left, 01 right, 10 fall when left, 11 fall when right
    reg [1:0] state, next;

    always @(*) begin
        case (state)
            2'b00: 
            if (!ground) next = 2'b10;
            else if (bump_left) next = 2'b01;
            else next = 2'b00;
            2'b01: 
            if (!ground) next = 2'b11;
            else if (bump_right) next = 2'b00;
            else next = 2'b01;
            2'b10: 
            if (ground) next = 2'b00;
            else next = 2'b10;
            2'b11:
            if (ground) next = 2'b01;
            else next = 2'b11;
        endcase
    end
    
    always @(posedge clk, posedge areset) begin
        if (areset) state <= 2'b00;
        else state <= next;
    end
    
    assign walk_left = state == 2'b00;
    assign walk_right = state == 2'b01;
    assign aaah = (state == 2'b10) || (state == 2'b11) ;
endmodule

