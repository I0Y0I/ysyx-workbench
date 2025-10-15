module top_module(
    input clk,
    input in,
    input reset,    // Synchronous reset
    output [7:0] out_byte,
    output done
); 

    // 00 IDLE, 01 START, 10 DONE, 11 WAIT
    reg [1:0] state, next;
    reg [3:0] cnt;
    reg [7:0] data;
    reg [7:0] data_reg;
    
    always @ (posedge clk) begin
        if (reset) cnt <= 4'd0;
        else if (state == 2'b01 && cnt == 4'd8) cnt <= 4'd8;
        else if (state == 2'b01) cnt <= cnt + 4'd1;
        else cnt <= 4'd0;
    end
    
    always @ (posedge clk) begin
        if (reset) data <= 8'd0;
        else data <= {in, data[7:1]}; 
    end
    
    always @ (posedge clk) begin
        if (reset) data_reg <= 8'd0;
        else if (next == 2'b10) data_reg <= data; 
    end
    
    always @ (*) begin
        case(state)
            2'b00: 
            	if (!in) next = 2'b01;
            	else next = 2'b00;
            2'b01:
                if (in & (cnt == 4'd8)) next = 2'b10;
            	else if (cnt == 4'd8) next = 2'b11;
            	else next = 2'b01;
            2'b10:
                if (!in) next = 2'b01;
            	else next = 2'b00;
            2'b11:
                if (in) next = 2'b00;
            	else next = 2'b11;
        endcase
    end
    
    always @ (posedge clk) begin
        if (reset) state <= 2'b00;
        else state <= next;
    end
    
    assign done = state == 2'b10;
    assign out_byte = data_reg;
endmodule

