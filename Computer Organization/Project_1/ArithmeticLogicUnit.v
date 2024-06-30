`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/04/2024 12:19:55 PM
// Design Name: 
// Module Name: ArithmeticLogicUnit
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

///////////////////////////////////////////////////////////
//                        Part 3                         //
///////////////////////////////////////////////////////////

module ArithmeticLogicUnit(A, B, FunSel, ALUOut, FlagsOut,Clock,WF);
    input wire [15:0] A;
    input wire [15:0] B;
    input wire [4:0] FunSel;
    output reg [15:0] ALUOut; 
    input wire Clock;
    input wire WF;
    output reg [3:0] FlagsOut; // Flags: Zero, Carry, Negative, Overflow
    reg Z = 0;
    reg C = 0;
    reg N = 0;
    reg O = 0; 
    // Zero|Carry|Negative|Overflow
    
    always @(posedge Clock) begin
        case (FunSel)
        5'b00000: // OutALU = A
            begin
                ALUOut = A;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b00001: // OutALU = B
            begin
                ALUOut = B;
                if ( WF == 1) begin
                // FLAG UPDATE
                    if (ALUOut[7] == 1 && WF == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        //-----------------------//

        5'b00010: // OutALU = NOT A
            begin
                ALUOut = ~A;
                
                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1 ) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b00011: // OutALU = NOT B
            begin
                ALUOut = ~B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        //-----------------------//

        5'b00100: // OutALU = A + B 
            begin
                O = 0;
                ALUOut = A + B ;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((A[7] == B[7]) && (B[7] != ALUOut[7])) begin
                        O = 1;
                    end
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b00101: // OutALU = A + B + Carry
            begin
                O = 0;
                {C, ALUOut} = {1'b0, A} + {1'b0, B} + {8'd0, C};

                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((A[7] == B[7]) && (B[7] != ALUOut[7])) begin
                        O = 1;
                    end
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b00110: // OutALU = A - B
            begin
                O = 0;
                {C, ALUOut} = {1'b0, A} + {1'b0, (~B + 8'd1)};
                
                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((B[7] == ALUOut[7]) && (B[7] != A[7])) begin
                        O = 1;
                    end
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b00111: // OutALU = A AND B
            begin
                ALUOut = A & B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b01000: // OutALU = A OR B
            begin
                ALUOut = A | B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b01001: // OutALU = A XOR B
            begin
                ALUOut = A ^ B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b01010: // OutALU = A NAND B
            begin
                ALUOut = ~(A & B);

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        //-----------------------//

        5'b01011: // OutALU = LSL A
            begin
                C = A[7];
                ALUOut = A;
                ALUOut = ALUOut << 1;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b01100: // OutALU = LSR A
            begin
                C = A[0];
                ALUOut = A;
                ALUOut = ALUOut >> 1;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end        
                end
            end

        5'b01101: // OutALU = ASR A
            begin
                ALUOut = A;
                ALUOut = ALUOut >> 1;
                ALUOut[7] = ALUOut[6];
                
                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
         5'b01110: // OutALU = CSL A
            begin
                ALUOut = {C, A[6:0]};
                C = A[7];

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b01111: // OutALU = CSR A
            begin
                ALUOut = {C, A[7:1]};
                C = A[0];

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[7] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end


        5'b10000: // OutALU = A
            begin
                ALUOut = A;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b10001: // OutALU = B
            begin
                ALUOut = B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        //-----------------------//

        5'b10010: // OutALU = NOT A
            begin
                ALUOut = ~A;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b10011: // OutALU = NOT B
            begin
                ALUOut = ~B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        //-----------------------//

        5'b10100: // OutALU = A + B 
            begin
                O = 0;
                ALUOut = A + B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((A[15] == B[15]) && (B[15] != ALUOut[15])) begin
                        O = 1;
                    end
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b10101: // OutALU = A + B + Carry
            begin
                O = 0;
                {C, ALUOut} = {1'b0, A} + {1'b0, B} + {16'd0, C};

                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((A[15] == B[15]) && (B[15] != ALUOut[15])) begin
                        O = 1;
                    end
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b10110: // OutALU = A - B
            begin
                O = 0;
                {C, ALUOut} = {1'b0, A} + {1'b0, (~B + 16'd1)};
                
                // FLAG UPDATE
                if ( WF == 1) begin
                    if ((B[15] == ALUOut[15]) && (B[15] != A[15])) begin
                        O = 1;
                    end
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b10111: // OutALU = A AND B
            begin
                ALUOut = A & B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b11000: // OutALU = A OR B
            begin
                ALUOut = A | B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b11001: // OutALU = A XOR B
            begin
                ALUOut = A ^ B;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b11010: // OutALU = A NAND B
            begin
                ALUOut = ~(A & B);

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        //-----------------------//

        5'b11011: // OutALU = LSL A
            begin
                C = A[15];
                ALUOut = A;
                ALUOut = ALUOut << 1;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end

        5'b11100: // OutALU = LSR A
            begin
                C = A[0];
                ALUOut = A;
                ALUOut = ALUOut >> 1;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end        
                end
            end


        5'b11101: // OutALU = ASR A
            begin
                ALUOut = A;
                ALUOut = ALUOut >> 1;
                ALUOut[15] = ALUOut[14];
                
                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
         5'b11110: // OutALU = CSL A
            begin
                ALUOut = {C, A[14:0]};
                C = A[15];

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        5'b11111: // OutALU = CSR A
            begin
                ALUOut = {C, A[15:1]};
                C = A[0];

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
            

        default: // Return A
            begin
                ALUOut = A;

                // FLAG UPDATE
                if ( WF == 1) begin
                    if (ALUOut[15] == 1) begin // Negative
                        N = 1;
                    end else begin
                        N = 0;
                    end
                end
            end
        endcase
        if ( WF == 1) begin
            if (ALUOut == 16'd0) begin // Zero
                Z = 1;
            end else begin
                Z = 0;
            end
        end
        if (FunSel[4] == 1'b0) begin // Input 8 Bit
                        ALUOut[15:8] <= {8{1'b0}}; 
        end
        FlagsOut <= {Z,C,N,O};
    end
        
endmodule
