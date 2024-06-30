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
    reg tmp = 0;
    reg Z = 0;
    reg C = 0;
    reg N = 0;
    reg O = 0; 
    // Zero|Carry|Negative|Overflow
    
    always @(*) begin
        case (FunSel)
        5'b00000: // OutALU = A
            begin
                ALUOut = A;
            end

        5'b00001: // OutALU = B
            begin
                ALUOut = B;
            end

        //-----------------------//

        5'b00010: // OutALU = NOT A
            begin
                ALUOut = ~A;
               
            end

        5'b00011: // OutALU = NOT B
            begin
                ALUOut = ~B;

            end

        //-----------------------//

        5'b00100: // OutALU = A + B 
            begin
                ALUOut = A + B ;
            end

        5'b00101: // OutALU = A + B + Carry
            begin
                
                {tmp, ALUOut} = {1'b0, A} + {1'b0, B} + {8'd0, tmp};
                
            end

        5'b00110: // OutALU = A - B
            begin
                {tmp, ALUOut} = {1'b0, A} + {1'b0, (~B + 8'd1)};
            end
        5'b00111: // OutALU = A AND B
            begin
                ALUOut = A & B;
            end

        5'b01000: // OutALU = A OR B
            begin
                ALUOut = A | B;
                
            end
        5'b01001: // OutALU = A XOR B
            begin
                ALUOut = A ^ B;

            end

        5'b01010: // OutALU = A NAND B
            begin
                ALUOut = ~(A & B);

               
            end
        //-----------------------//

        5'b01011: // OutALU = LSL A
            begin
                
                ALUOut = A;
                ALUOut = ALUOut << 1;
            end
        5'b01100: // OutALU = LSR A
            begin
                
                ALUOut = A;
                ALUOut = ALUOut >> 1;
            end

        5'b01101: // OutALU = ASR A
            begin
                ALUOut = A;
                ALUOut = ALUOut >> 1;
                ALUOut[7] = ALUOut[6];
                
                
            end
         5'b01110: // OutALU = CSL A
            begin
                ALUOut = {tmp, A[6:0]};
                tmp = A[7];

            end
        5'b01111: // OutALU = CSR A
            begin
                ALUOut = {tmp, A[7:1]};
                tmp = A[0];

            end


        5'b10000: // OutALU = A
            begin
                ALUOut = A;

            end
        5'b10001: // OutALU = B
            begin
                ALUOut = B;

                
            end
        //-----------------------//

        5'b10010: // OutALU = NOT A
            begin
                ALUOut = ~A;

                
            end
        5'b10011: // OutALU = NOT B
            begin
                ALUOut = ~B;

            end

        //-----------------------//

        5'b10100: // OutALU = A + B 
            begin
                
                {tmp, ALUOut} = {1'b0, A} + {1'b0, B};
                
                
            end

        5'b10101: // OutALU = A + B + Carry
            begin
                
                {tmp, ALUOut} = {1'b0, A} + {1'b0, B} + {16'd0, tmp};

            end

        5'b10110: // OutALU = A - B
            begin
                O = 0;
                {tmp, ALUOut} = {1'b0, A} + {1'b0, (~B + 16'd1)};
               
            end
        5'b10111: // OutALU = A AND B
            begin
                ALUOut = A & B;

                
            end

        5'b11000: // OutALU = A OR B
            begin
                ALUOut = A | B;

                
            end

        5'b11001: // OutALU = A XOR B
            begin
                ALUOut = A ^ B;

               
            end

        5'b11010: // OutALU = A NAND B
            begin
                ALUOut = ~(A & B);

               
            end
        //-----------------------//

        5'b11011: // OutALU = LSL A
            begin
                tmp = A[15];
                ALUOut = A;
                ALUOut = ALUOut << 1;

                
            end

        5'b11100: // OutALU = LSR A
            begin
                tmp = A[0];
                ALUOut = A;
                ALUOut = ALUOut >> 1;

            end


        5'b11101: // OutALU = ASR A
            begin
                tmp = A[0];
                ALUOut = A;
                ALUOut = ALUOut >> 1;
                ALUOut[15] = ALUOut[14];
                
                
            end
         5'b11110: // OutALU = CSL A
            begin
                ALUOut = {C, A[14:0]};
                tmp = A[15];

               
            end
        5'b11111: // OutALU = CSR A
            begin
                ALUOut = {C, A[15:1]};
                tmp = A[0];

                
            end
            

        default: // Return A
            begin
                ALUOut = A;

                
            end
        endcase
        if (FunSel[4] == 1'b0) begin // Input 8 Bit
                        ALUOut[15:8] <= {8{1'b0}}; 
        end
        
    end
    always @(posedge Clock) begin
        case (FunSel)
        5'b00000: // OutALU = A
            begin
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
                    C = tmp;
                end
            end

        5'b00101: // OutALU = A + B + Carry
            begin
                O = 0;
               

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
                C = tmp;
            end

        5'b00110: // OutALU = A - B
            begin
                O = 0;
                
                
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
                C = tmp;
            end
        5'b00111: // OutALU = A AND B
            begin
                

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
                C = tmp;
             
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
                C = tmp;                
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
                C = tmp;
                
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
                
                C = tmp;

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
                
                C = tmp;

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
                C = tmp;
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
                C = tmp;
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
                C = tmp;
                
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
                C = tmp;
                
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
               C = tmp;
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
                
                C = tmp;
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
               C = tmp;

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
                C = tmp;
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
        FlagsOut <= {Z,C,N,O};
    end 
        
endmodule
