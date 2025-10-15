module top_module(
    input clk,
    input in,
    input reset,    // Synchronous reset
    output done
); 

    // 00 IDLE, 01 START, 10 FINISHED, 11 DONE
    reg [1:0] state, next;
    reg [2:0] cnt;
    reg [7:0] data;
    
    always @ (posedge clk) begin
        if (reset) cnt <= 3'd0;
        else if (state == 2'b01 && cnt == 3'd7) cnt <= 3'd7;
        else if (state == 2'b01) cnt <= cnt + 3'd1;
        else cnt <= 3'd0;
    end
    
    always @ (posedge clk) begin
        if (reset) data <= 8'd0;
        else if (state == 2'b01) data <= {data[6:0], in}; 
    end
    
    always @ (*) begin
        case(state)
            2'b00: 
            	if (!in) next = 2'b01;
            	else next = 2'b00;
            2'b01:
                if (cnt == 3'd7) next = 2'b10;
            	else next = 2'b01;
            2'b10:
                if (in & (cnt == 3'd7)) next = 2'b11;
            else if (in) next = 2'b00;
            	else next = 2'b10;
            2'b11:
                if (!in) next = 2'b01;
            	else next = 2'b00;
        endcase
    end
    
    always @ (posedge clk) begin
        if (reset) state <= 2'b00;
        else state <= next;
    end
    
    assign done = state == 2'b11;
endmodule

