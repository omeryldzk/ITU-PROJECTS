///////////////////////////////////////////////////////////
//                        Part 2                         //
///////////////////////////////////////////////////////////

// Part 2a
`timescale 1ns / 1ps
module InstructionRegister(Clock,I,Write,LH,IROut);
    input wire [7:0] I;
    input wire Write;
    input wire LH;
    input wire Clock;
    output reg [15:0] IROut;
    always @(posedge Clock)
        begin
             if(Write) 
                if(LH)
                begin 
                    IROut[15:8] <= I;
                end 
                else 
                begin 
                    IROut[7:0] <= I; 
                end
             else 
             begin 
                  IROut <= IROut;
             end
        end
endmodule