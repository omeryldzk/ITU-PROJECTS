`timescale 1ns / 1ps
///////////////////////////////////////////////////////////
//Ömer Y?ld?z
//Utku Kadir Somer
///////////////////////////////////////////////////////////
//                        Part 1                         //
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
module Register (Clock, I, FunSel, E, Q);

    input wire [15:0] I;
    input wire [2:0] FunSel;
    input wire E;
    input wire Clock;
    output reg [15:0] Q;
    
    always @(posedge Clock) begin
        if (E) begin
            case (FunSel)
                3'b000 : Q <= Q - {{15{1'b0}},1'b1}; // Decrement
                3'b001 : Q <= Q + {{15{1'b0}},1'b1}; // Increment
                3'b010 : Q <= I[15:0]; // Load
                3'b011 : Q <= {15{1'b0}}; // Clear
                3'b100 : begin
                         Q[15:8] <= {8{1'b0}}; // Write Low
                         Q[7:0] <= I[7:0];
                end
                3'b101 : Q[7:0] <= I[7:0]; // Only Write Low
                3'b110 : Q[15:8] <= I[7:0];  // Only Write High
                3'b111 : begin
                         Q[15:8] <= {{7{1'b0}},I[7]};
                         Q[7:0] <= I[7:0];
                end
                default: Q <= Q; // Retain value
            endcase
        end else begin
            Q <= Q; // If enable is not set, retain the value
        end
    end
endmodule
