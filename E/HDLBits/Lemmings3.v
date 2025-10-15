module top_module(
    input clk,
    input areset,    // Freshly brainwashed Lemmings walk left.
    input bump_left,
    input bump_right,
    input ground,
    input dig,
    output walk_left,
    output walk_right,
    output aaah,
    output digging ); 
    
    // 000 left, 001 right, 010 fall when left, 011 fall when right, 100 dig left, 101 dig right
    reg [2:0] state, next;

    always @(*) begin
        case (state)
            3'b000: 
                if (!ground) next = 3'b010;
            else if (dig) next = 3'b100;
            else if (bump_left) next = 3'b001;
            else next = 3'b000;
            3'b001: 
                if (!ground) next = 3'b011;
            else if (dig) next = 3'b101;
            else if (bump_right) next = 3'b000;
            else next = 3'b001;
            3'b010: 
                if (ground) next = 3'b000;
            else next = 3'b010;
            3'b011:
                if (ground) next = 3'b001;
            else next = 2'b011;
            3'b100:
                if (!ground) next = 3'b010;
            else next = 3'b100;
            3'b101:
                if (!ground) next = 3'b011;
            else next = 3'b101;
        endcase
    end
    
    always @(posedge clk, posedge areset) begin
        if (areset) state <= 3'b000;
        else state <= next;
    end
    
    assign walk_left = state === 3'b000;
    assign walk_right = state == 3'b001;
    assign aaah = (state == 3'b010) || (state == 3'b011);
    assign digging = (state == 3'b100) || (state == 3'b101);
endmodule

