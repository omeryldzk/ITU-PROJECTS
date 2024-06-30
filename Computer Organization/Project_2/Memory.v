`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/04/2024 12:27:27 PM
// Design Name: 
// Module Name: Memory
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Memory(
    input wire [15:0] address,
    input wire [7:0] data,
    input wire wr, // Read = 0, Write = 1
    input wire cs, // Chip is enable when cs = 0
    input wire Clock,
    output reg [7:0] o // Output
);
    //Declaration of the RAM area
    reg [7:0] RAM_DATA[0:255];
    //Read RAM data from the file
    initial $readmemh("RAM.mem", RAM_DATA);
    // Read the selected data from RAM
    always @(*) begin
        o = ~wr && ~cs ? RAM_DATA[address] : 8'hZ;
    end

    //Write the data to ram
    always @(posedge Clock) begin
        if (wr && ~cs) begin
            RAM_DATA[address] <= data;
        end
    end
endmodule