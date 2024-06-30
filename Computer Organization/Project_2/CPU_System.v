`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 14.05.2024 23:35:02
// Design Name: 
// Module Name: CPU_System
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


module counter(clk,reset,count,O);
    input wire clk,reset,count;
    output reg [3:0] O = 4'b0;

    always@(posedge clk)    //no need to reset if it overflows since the upper bits will be ignored 1111 -> 10000 which 0000 in our case
        begin
            if(reset)
                O <= 0;
            else if(count)
                O <= O + 1;
            else
                O <= O;
    end
endmodule

module decoder_3to8(enable,I,O);
    input wire enable;
    input wire [3:0] I;
    output reg [7:0] O;
    
    always @(*)
    begin
        if(enable) begin
        O[I] = 1;
        end
        else
        begin
            O = O;
        end
    end
endmodule
module decoder_6to64(
    input wire enable,
    input wire [5:0] I,
    output reg [63:0] O
);

always @(*)
begin
    if(enable) begin
       O[I] = 1;
    end
end

endmodule

module fetch_L(T0,ARF_FunSel,ARF_RegSel,OutDSel,OutCSel,cs,wr,Write,L_H);
    input wire T0;
    output reg [2:0] ARF_FunSel;
    output reg [2:0] ARF_RegSel;
    output reg [1:0] OutDSel;
    output reg [1:0] OutCSel;
    output reg cs; //chip select, is active low for memory
    output reg wr; //read is 0 write is 1
    output reg Write;
    output reg L_H;
    
    
    always @(T0)        //might need to change it to always * later, but for now it makes sense
    begin
        if(T0)
        begin
            OutDSel <= 2'b00;
            //idk what to do with OutASel as I dont need it here
            OutCSel <= 2'bZZ;
            cs <= 1'b0;
            wr <= 1'b0;
            Write <= 1'b1;
            L_H <= 1'b0;
            ARF_RegSel <= 3'b011;
            ARF_FunSel <= 3'b001;
        end
        else
        begin
            OutDSel <= 2'bZZ;
            //idk what to do with OutASel as I dont need it here
            OutCSel <= 2'bZZ;
            cs <= 1'bZ;
            wr <= 1'bZ;
            Write <= 1'bZ;
            L_H <= 1'bZ;
            ARF_RegSel <= 4'bZZZZ;
            ARF_FunSel <= 2'bZZ;
        end
        end
endmodule

module fetch_H(T1,ARF_FunSel,ARF_RegSel,OutDSel,OutCSel,cs,wr,Write,L_H);
    input wire T1;
    output reg [2:0] ARF_FunSel;
    output reg [2:0] ARF_RegSel;
    output reg [1:0] OutDSel;
    output reg [1:0] OutCSel;
    output reg cs; //chip select, is active low for memory
    output reg wr; //read is 0 write is 1
    output reg Write;
    output reg L_H;
    
    
    always @(T1)        //might need to change it to always * later, but for now it makes sense
    begin
        if(T1)
        begin
            OutDSel <= 2'b00;
            //idk what to do with OutASel as I dont need it here
            OutCSel <= 2'bZZ;
            cs <= 1'b0;
            wr <= 1'b0;
            Write <= 1'b1;
            L_H <= 1'b0;
            ARF_RegSel <= 3'b011;
            ARF_FunSel <= 3'b001;
        end
        else
        begin
            OutDSel <= 2'bZZ;
            //idk what to do with OutASel as I dont need it here
            OutCSel <= 2'bZZ;
            cs <= 1'bZ;
            wr <= 1'bZ;
            Write <= 1'bZ;
            L_H <= 1'bZ;
            ARF_RegSel <= 4'bZZZZ;
            ARF_FunSel <= 2'bZZ;
        end
        end
endmodule



module instruction_decoder(T2,IR_H, D, SEL);         // A is for addressing if from D we get 9,10,12,13
    input wire [7:0] IR_H;
    input wire T2;
    output wire [63:0] D;
    output reg [1:0] SEL;
    decoder_6to64 operation_decode(T2,IR_H[7:2],D);         // here I would get the output for the operation and accordingly I would use it in the enable parts of the other functions
        always @(*)
        begin
            if(T2)
            begin
            SEL <= IR_H[1:0];
            end
        end
endmodule

module control_unit(clk, IROut,ARF_FunSel,ARF_RegSel,ARF_OutCSel, ARF_OutDSel,WF,
    ALU_FlagReg, ALU_FunSel, RF_OutASel, RF_OutBSel, RF_FunSel, RF_RegSel, RF_ScrSel,
    MuxASel, MuxBSel, MuxCSel, Mem_CS, Mem_WR, IR_LH, Write, RESET, T);    //add the outputs you need for enable, I added the ones I need, FUNSEL, REGSEL, ...........
    input wire clk; 
    input wire [15:0] IROut = _ALUSystem.IR.IROut;     
    input wire [3:0] ALU_FlagReg;
            //this value is going to be changed from your final execution modules, also idk whether this should be input/output it depends on the way you guys code so change it if you see it suitable
    input wire RESET;
    output reg WF;
    output reg [2:0] ARF_FunSel;
    output reg [2:0] ARF_RegSel;
    output reg [1:0] ARF_OutCSel;
    output reg [1:0] ARF_OutDSel;
    output reg [4:0] ALU_FunSel;
    output reg [2:0] RF_OutASel;
    output reg [2:0] RF_OutBSel;
    output reg [2:0] RF_FunSel;
    output reg [3:0] RF_RegSel;
    output reg [3:0] RF_ScrSel;

    output reg [1:0] MuxASel;
    output reg [1:0] MuxBSel;
    output reg MuxCSel;
    output reg Mem_CS;
    output reg Mem_WR;
    output reg IR_LH;
    output reg Write;
    
    reg SC_Reset;
    input wire [7:0] T;
    wire [2:0] temp;    //temp wire to send data from counter to decoder
    //creating a counter that is then decoded to give me the times needed to make my instructions execute in orded
    counter SC(clk,SC_Reset,1,temp);       //gave 1 to count continuously
    decoder_3to8 SC_decoder(1,temp,T);    //gave 1 to be always active
    
    always @(posedge RESET)begin
               Mem_WR <= 1'b0;
               Mem_CS <= 1'b1;
               ARF_FunSel <= 3'b000;
               RF_FunSel <= 3'b000;
               RF_RegSel <= 4'b1111;
               ARF_RegSel <= 4'b1111;
               RF_ScrSel <= 4'b1111;
               SC_Reset <= 1;
               
               #6; // According to clock frequency
               RF_RegSel <= 4'b0000;
               ARF_RegSel <= 4'b0000;
               RF_ScrSel <= 4'b0000;
               SC_Reset <= 0;
               #4;
           end
           //Dont forget to reset PC at the start of the last module to start it from 0
           
           always @(T[0])
           begin
               if(T[0] && SC_Reset == 0)
               begin
                   ARF_OutDSel <= 2'b00;
                   ARF_OutCSel <= 2'bZZ;
                   Mem_CS <= 1'b0;
                   Mem_WR <= 1'b0;
                   Write <= 1'b1;
                   IR_LH <= 1'b0;
                   ARF_RegSel <= 3'b011;
                   ARF_FunSel <= 3'b001;
               end
               else
               begin
                  ARF_OutDSel <= 2'bZZ;
                  ARF_OutCSel <= 2'bZZ;
                  Mem_CS <= 1'bZ;
                  Mem_WR <= 1'bZ;
                  Write <= 1'bZ;
                  IR_LH <= 1'bZ;
                  ARF_RegSel <= 3'bZZZ;
                  ARF_FunSel <= 3'bZZZ;
               end
           end
           always @(T[1])
           begin
               if(T[1])
               begin
                  ARF_OutDSel <= 2'b00;
                  ARF_OutCSel <= 2'bZZ;
                  Mem_CS <= 1'b0;
                  Mem_WR <= 1'b0;
                  Write <= 1'b1;
                  IR_LH <= 1'b1;
                  ARF_RegSel <= 3'b011;
                  ARF_FunSel <= 3'b001;
               end
               else
               begin
                 ARF_OutDSel <= 2'bZZ;
                 //idk what to do with OutASel as I dont need it here
                 ARF_OutCSel <= 2'bZZ;
                 Mem_CS <= 1'bZ;
                 Mem_WR <= 1'bZ;
                 Write <= 1'bZ;
                 IR_LH <= 1'bZ;
                 ARF_RegSel <= 3'bZZZ;
                 ARF_FunSel <= 3'bZZZ;
               end
           end
    //decoding the instruction after it has been fully loaded to the instruction register
    wire [63:0] D;    //used to give out which instruction it is
    wire [1:0] SEL;        //used in the selection of the register, if you forgot how i explained it ask me
    instruction_decoder step3(T[2], IROut[15:8], D, SEL);

    
    wire S = IROut[9];
    wire [2:0] DSTREG = IROut[8:6];
    wire [2:0] SREG1 = IROut[5:3];
    wire [2:0] SREG2 = IROut[2:0];
    reg sreg1F  = 1'b0;
    reg sreg2F  = 1'b0;
    // Sources:
    // 11 : RF, RF
    // 10 : RF, ARF
    // 01 : ARF, RF
    // 00 : ARF, ARF
    reg [1:0] sources = 2'b11;

    always @(T) begin // I don't know if this works??
       
        // Flag check
        WF <= S;
        // S1 <- PC
        if((D[0] && T[3]) || (D[1] && T[3] && ALU_FlagReg) || (D[2] && T[3] && ~ALU_FlagReg))begin
                MuxASel <= 2'b01;
                ARF_OutCSel <= 2'b00;
                RF_ScrSel <= 4'b0111;
                RF_FunSel <= 3'b010;
        end
        // S2 <- IR(7-0)
        if((D[0] && T[4]) || (D[1] && T[4] && ALU_FlagReg) || (D[2] && T[4] && ~ALU_FlagReg))begin
                MuxASel <= 2'b11;
                RF_ScrSel <= 4'b1011;
                RF_FunSel <= 3'b010;
        end
        // PC <- S1 + S2
        if((D[0] && T[5]) || (D[1] && T[5] && ALU_FlagReg) || (D[2] && T[5] && ~ALU_FlagReg))begin
                MuxBSel <= 2'b00;
                ALU_FunSel <= 5'b00100;
                RF_OutASel <= 3'b100;
                RF_OutBSel <= 3'b101;
                ARF_RegSel <= 3'b011;
                ARF_FunSel <= 3'b010;
        end
        //MEM_CS
        if((D[3] || D[4]  || D[31] || D[18] || D[19] ) && T[3] || ((D[18] || D[19] || D[30] || D[31])&& T[4])  || (D[30] && T[5]))begin
                Mem_CS <= 0;
        end
        // Read
        if((D[3] || D[31] || D[18]) && T[3] || (D[18] || D[31] && T[4]))begin
                Mem_WR <= 0;
        end
        // Write
        if(( (D[4]  || D[19]) && T[3]) || (D[19] || D[30] && T[4]) || D[30] && T[5] )begin
                Mem_WR <= 1;
        end
        //RF_Regsel
        if((D[3] && T[3]) || (D[18] && T[3]) || (D[32] && T[3]) || ( D[17] && T[3]) || (D[20] && T[3])|| ( D[18] && T[4]) )begin
            case (SEL)
              2'b00: RF_RegSel <= 4'b0111; // R1
              2'b01: RF_RegSel <= 4'b1011; // R2
              2'b10: RF_RegSel <= 4'b1101; // R3
              2'b11: RF_RegSel <= 4'b1110; // R4
              default: RF_RegSel <= 4'b1111;
          endcase
        end
        // RF_FUNSEL LOW LOAD
        if((D[3] && T[3]) || (D[20] && T[3]) || (D[32] && T[3]) || (D[18] && T[3]))begin
            RF_FunSel <= 3'b101;
        end
        // RF_FUNSEL HIGH LOAD
        if((D[17] && T[3]) || (D[18] && T[4]) )begin
            RF_FunSel <= 3'b110;
        end
        //RF_OutASel
        if((D[4] && T[3]) || (D[19] && T[3]) || (D[19] && T[4])  || (D[30] && T[6] ) || (D[33] && T[6]))begin
            case (SEL)
              2'b00: RF_OutASel <= 3'b000; // R1
              2'b01: RF_OutASel <= 3'b001; // R2
              2'b10: RF_OutASel <= 3'b010; // R3
              2'b11: RF_OutASel <= 3'b011; // R4
              default: ARF_OutCSel <= 2'b11;
          endcase
        end
        
        // RX <- PC (S1)
        if(D[30] && T[3])begin
            RF_FunSel <= 3'b010;
            RF_ScrSel <= 4'b0111; // ARF => S1
        end
         // M[SP] <- RX 
         if(D[30] && T[4] || D[30] && T[5])begin
             RF_OutASel <= 3'b100; // S1
             ALU_FunSel <= 5'b10000;
         end
         
        //MUXBSEL
        
            //ALU_OUT
            if((D[30] && T[6]))begin
                MuxBSel <= 2'b00;
            end
            // Memory Output
            if((D[31] && T[4] || D[31] && T[3] ))begin
                MuxBSel <= 2'b10;
            end
            
        //MUXASEL
            //RX <- Memory
            if((D[3] && T[3]) ||  (D[18] && T[3]) || (D[18] && T[4]) )begin
                MuxASel <= 2'b10;
            end
            //ARF
            if(D[30] && T[3])begin
                MuxASel <= 2'b01;
            end
            //IR
            if(D[32] && T[3] || (D[17] && T[3]) || D[15] && T[3] || (D[20] && T[3]))begin
                MuxASel <= 2'b11;
            end
        //MUXCSEL
             // (7-0)
            if((D[4] && T[3]) || D[19] && T[3] || D[30] && T[4])begin
                MuxCSel <= 1'b0;
            end
            // (15-8)
            if( D[19] && T[4] || D[30] && T[5])begin
                MuxCSel <= 1'b1;
            end
        //ARF_Regsel //ARF_Funsel
        //Write Low
        if( (D[31] && T[3]) || (D[30] && T[6]))//PC
        begin
            ARF_RegSel <= 3'b011;
            ARF_FunSel <= 3'b101;        
        end
        //Write High
        if( (D[31] && T[4]))begin
            ARF_RegSel <= 3'b110;
            ARF_FunSel <= 3'b101;        
         end
        //ARF_Regsel //ARF_Funsel
        if((D[30] && T[6]))//PC
        begin
            ARF_RegSel <= 3'b011;
            ARF_FunSel <= 3'b010;
        end
        
         if( (D[18] && T[3]) || (D[19] && T[3]) )//AR <- AR + 1
           begin
               ARF_RegSel <= 3'b101;
               ARF_FunSel <= 3'b001;
           end
        if((D[4] && T[4]) || (D[3] && T[3]) || (D[3] && T[4])|| (D[4] && T[3]) || D[30] && T[4] || D[31] && T[3])//SP
        begin
            ARF_RegSel <= 3'b110;
        end
       
        if(D[3] && T[4] || (D[3] && T[3]) || D[30] && T[4] || D[31] && T[3])//SP + 1
        begin
            ARF_FunSel <= 3'b001;
        end
        if(D[4] && T[4] || (D[4] && T[3]))//SP - 1
        begin
            ARF_FunSel <= 3'b000;
        end
        
        //ARF_OutCSel
        if (D[30] && T[3])//PC
        begin
            ARF_OutCSel <= 3'b011;
        end
         //ARF_OutDSel       
        if((D[4] && T[3])|| (D[3] && T[3])|| (D[30] && T[4]) ||  (D[31] && T[3]) ||  (D[31] && T[4]))//SP
        begin
            ARF_OutDSel <= 2'b11;
        end
         if((D[18] && T[3]) || (D[18] && T[4]) || (D[19] && T[3]) || (D[19] && T[4]))//AR
        begin
           ARF_OutDSel <= 2'b10;
         end
        //STRIM S1 <- AR
         if (D[33] && T[3]) 
         begin
             ARF_OutCSel <= 2'b10;
             RF_FunSel <= 3'b010;
             RF_ScrSel <= 4'b0111; // ARF => S1
             MuxASel <= 2'b01;
         end
         // S2 <- IR
         if (D[33] && T[4]) 
         begin
              RF_FunSel <= 3'b010;
              RF_ScrSel <= 4'b1011; // IR => S2
              MuxASel <= 2'b11;
         end
         // AR <- S1 + S2
        if (D[33] && T[5]) 
         begin
             MuxBSel <= 2'b00;
             ALU_FunSel <= 5'b00100;
             RF_OutASel <= 3'b100;
             RF_OutBSel <= 3'b101;
             ARF_RegSel <= 3'b101;
             ARF_FunSel <= 3'b010;
         end
         if (D[33] && T[6]) 
          begin
           ARF_OutDSel <= 2'b10;
           Mem_CS <= 0;
           Mem_WR <= 0;
           RF_FunSel <= 3'b010;
           MuxCSel <= 1'b0;
           ALU_FunSel <= 5'b00000;

          end
        case(D)
            64'h04:ALU_FunSel <= 5'b00000;// M[SP] <- Rx
            64'h05:ALU_FunSel <= 5'b10000;// INC
            64'h06:ALU_FunSel <= 5'b10000;// DEC
            64'h07:ALU_FunSel <= 5'b11011;// LSL
            64'h08:ALU_FunSel <= 5'b11100;// LSR
            64'h09:ALU_FunSel <= 5'b11101;// ASR
            64'h0A:ALU_FunSel <= 5'b11110;// CSL
            64'h0B:ALU_FunSel <= 5'b11111;// CSR
            64'h0C:ALU_FunSel <= 5'b10111; //AND
            64'h0D:ALU_FunSel <= 5'b11000; //ORR
            64'h0E:ALU_FunSel <= 5'b10011; //NOT
            64'h0F:ALU_FunSel <= 5'b11001; //XOR
            64'h10:ALU_FunSel <= 5'b11010; //NAND
            64'h13:ALU_FunSel <= 5'b00000; //M[AR] <- Rx
            64'h15:ALU_FunSel <= 5'b10100; //ADD
            64'h16:ALU_FunSel <= 5'b10101; //ADC
            64'h17:ALU_FunSel <= 5'b10110; //SUB
            64'h18:ALU_FunSel <= 5'b10000; //MOVS
            64'h19:ALU_FunSel <= 5'b10100; //ADDS
            64'h1A:ALU_FunSel <= 5'b10110; //SUBS
            64'h1B:ALU_FunSel <= 5'b10111; //ANDS
            64'h1C:ALU_FunSel <= 5'b11000; //ORRS
            64'h1D:ALU_FunSel <= 5'b11001; //XORS
            64'h1E:ALU_FunSel <= 5'b10000; //BX
           
       endcase
        
       // SREG1 from RF
       if ((D[5] || D[6] || D[7] || D[9] || D[10] || D[11] || D[12] || D[13] || D[14] || D[15]
        || D[16] || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])
           && T[3] && SREG1[2] == 1) begin
           // ALU_A <- SREG1
           RF_OutASel <= {1'b0, SREG1[1:0]};
           sources <= {SREG1[2], sources[0]};  
       end
       
        // SREG2 from RF
       if ((D[12] || D[13] || D[14] || D[15] || D[16] || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])
          && T[3] && SREG2[2] == 0) begin
          // ALU_B <- SREG2
          RF_OutBSel <= {1'b0, SREG2[1:0]};
          sources <= {sources[1], SREG2[2]}; 
       end
       
        // SREG1 from ARF
       if ((D[5] || D[6] || D[7] || D[9] || D[10] || D[11] || D[12] || D[13] || D[14] || D[15]
        || D[16] || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])
           && T[3] && SREG1[2] == 1) begin
           // ALU_A <- SREG1
           case (SREG1[1:0])
               2'b00: ARF_OutCSel <= 2'b00; // ARF Output is SP
               2'b01: ARF_OutCSel <= 2'b01; // ARF Output is AR
               2'b10: ARF_OutCSel <= 2'b11; // ARF Output is PC
               2'b11: ARF_OutCSel <= 2'b10; // ARF Output is PC
               default: ARF_OutCSel <= 2'b11;
           endcase
           sources <= {SREG1[2], sources[0]};
           MuxASel <= 2'b11;
           RF_FunSel <= 3'b010;
           RF_ScrSel <= 4'b0111; // ARF => S1
           
       end
       // SREG2 from ARF if SREG1 NOT ARF
       if ((D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])
        && T[3] && SREG2[2] == 1 && ~(sources[1] == 0))begin
          // ALU_B <- SREG2
          case (SREG2[1:0])
              2'b00: ARF_OutCSel <= 2'b00; // ARF Output is SP
              2'b01: ARF_OutCSel <= 2'b01; // ARF Output is AR
              2'b10: ARF_OutCSel <= 2'b11; // ARF Output is PC
              2'b11: ARF_OutCSel <= 2'b10; // ARF Output is PC
              default: ARF_OutCSel <= 2'b11;
          endcase
          
          sources <= { sources[1] , SREG1[2]};
          MuxASel <= 2'b11;
          RF_FunSel <= 3'b010;
          RF_ScrSel <= 4'b1011; // ARF => S2
          
      end
        // SREG2 from ARF if SREG1 ARF
        if ((D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])
        && T[4] && SREG2[2] == 1 && (sources[1] == 0))begin
          // ALU_B <- SREG2
          case (SREG2[1:0])
              2'b00: ARF_OutCSel <= 2'b00; // ARF Output is SP
              2'b01: ARF_OutCSel <= 2'b01; // ARF Output is AR
              2'b10: ARF_OutCSel <= 2'b11; // ARF Output is PC
              2'b11: ARF_OutCSel <= 2'b10; // ARF Output is PC
              default: ARF_OutCSel <= 2'b11;
          endcase
          sources <= {SREG1[2], sources[0]};
          MuxASel <= 2'b11;
          RF_FunSel <= 3'b010;
          RF_ScrSel <= 4'b1011; // ARF => S2
      end
       if ((D[5] || D[6] || D[7] || D[9] || D[10] || D[11]) && T[4])begin
            // RF
          if(sources[1] == 1)begin
             //DESTREG RF
              if(DSTREG[2] == 1)begin     
               case (DSTREG[1:0])
                  2'b00: RF_RegSel <=   4'b0111; // RF Output is R1
                  2'b01: RF_RegSel <=   4'b1010; // RF Output is R2
                  2'b10: RF_RegSel <=   4'b1101; // RF Output is R3
                  2'b11: RF_RegSel <=   4'b1110; // RF Output is R3
                  default: RF_RegSel <= 4'b1111;
               endcase
               MuxASel <= 00;
               RF_FunSel <= 3'b010;
            end
            // DESTREG ARF
            if(DSTREG[2] == 0)begin
              case (DSTREG[1:0])
                    2'b00: ARF_RegSel <=   3'b011; // ARF Output is SP
                    2'b01: ARF_RegSel <=   3'b011; // ARF Output is AR
                    2'b10: ARF_RegSel <=   3'b110; // ARF Output is PC
                    2'b11: ARF_RegSel <=   3'b101; // ARF Output is PC
                    default: RF_RegSel <= 3'b111;
                endcase
                ARF_FunSel <= 3'b010;
                MuxBSel <= 00;
              end
          end
          //ARF
           if(sources[1] == 0)begin
               RF_OutASel <= 100;//S1
              //DESTREG RF
               if(DSTREG[2] == 1)begin     
                case (DSTREG[1:0])
                   2'b00: RF_RegSel <=   4'b0111; // RF Output is R1
                   2'b01: RF_RegSel <=   4'b1010; // RF Output is R2
                   2'b10: RF_RegSel <=   4'b1101; // RF Output is R3
                   2'b11: RF_RegSel <=   4'b1110; // RF Output is R3
                   default: RF_RegSel <= 4'b1111;
                endcase
                MuxASel <= 00;
                RF_FunSel <= 3'b010;
             end
             // DESTREG ARF
             if(DSTREG[2] == 0)begin
               case (DSTREG[1:0])
                     2'b00: ARF_RegSel <=   3'b011; // ARF Output is SP
                     2'b01: ARF_RegSel <=   3'b011; // ARF Output is AR
                     2'b10: ARF_RegSel <=   3'b110; // ARF Output is PC
                     2'b11: ARF_RegSel <=   3'b101; // ARF Output is PC
                     default: RF_RegSel <= 3'b111;
                 endcase
                 ARF_FunSel <= 3'b010;
                 MuxBSel <= 00;
               end
           end
       end
      
      // RF RF end Cycle RF ARF or ARF RF T4
      if(~(sources[0] == 0 && sources[1] == 0) && T[4] && (D[12] || D[13] || D[14] || D[15]|| D[16]
               || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29]))
          begin
        // ARF RF
           if((sources[1] == 0 && sources[0] == 1) && (D[12] || D[13] || D[14] || D[15]|| D[16]
            || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29]))
           begin
               RF_OutASel <= 3'b100; // S1
           end
         // RF ARF
           if((sources[1] == 1 && sources[0] == 0) && (D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] ||
            D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29]))
            begin
               RF_OutBSel <= 3'b101; // S2
           end
           if((sources[1] == 1 && sources[0] == 1) && (D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] ||
                            D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29]))
               begin
               if(DSTREG[2] == 1)begin     
                 case (DSTREG[1:0])
                    2'b00: RF_RegSel <=   4'b0111; // RF Output is R1
                    2'b01: RF_RegSel <=   4'b1010; // RF Output is R2
                    2'b10: RF_RegSel <=   4'b1101; // RF Output is R3
                    2'b11: RF_RegSel <=   4'b1110; // RF Output is R3
                    default: RF_RegSel <= 4'b1111;
                 endcase
                 MuxASel <= 00;
                 RF_FunSel <= 3'b010;
              end
              // DESTREG ARF
              if(DSTREG[2] == 0)begin
                case (DSTREG[1:0])
                      2'b00: ARF_RegSel <=   3'b011; // ARF Output is SP
                      2'b01: ARF_RegSel <=   3'b011; // ARF Output is AR
                      2'b10: ARF_RegSel <=   3'b110; // ARF Output is PC
                      2'b11: ARF_RegSel <=   3'b101; // ARF Output is PC
                      default: RF_RegSel <= 3'b111;
                  endcase
                  ARF_FunSel <= 3'b010;
                  MuxBSel <= 00;
                end
            end
        end   
        
        
      // ARF ARF
      if((sources[0] == 0 && sources[1] == 0) && T[5])begin
                RF_OutASel <= 3'b100; // S1
                RF_OutBSel <= 3'b101; // S2
      end
      if((sources[0] == 0 && sources[1] == 0) && T[6])begin
          // DESTREG RF
         if(DSTREG[2] == 1)begin
             case (DSTREG[1:0])
                2'b00: RF_RegSel <=   4'b0111; // RF Output is R1
                2'b01: RF_RegSel <=   4'b1010; // RF Output is R2
                2'b10: RF_RegSel <=   4'b1101; // RF Output is R3
                2'b11: RF_RegSel <=   4'b1110; // RF Output is R4
                default: RF_RegSel <= 4'b1111;
             endcase
             MuxASel <= 01;
             RF_FunSel <= 3'b010;
           end
        // DESTREG ARF
        if(DSTREG[2] == 0)begin
           case (DSTREG[1:0])
                 2'b00: ARF_RegSel <=   3'b011; // ARF Output is SP
                 2'b01: ARF_RegSel <=   3'b011; // ARF Output is AR
                 2'b10: ARF_RegSel <=   3'b110; // ARF Output is PC
                 2'b11: ARF_RegSel <=   3'b101; // ARF Output is PC
                 default: RF_RegSel <= 3'b111;
             endcase
             MuxBSel <= 01;
             ARF_FunSel <= 3'b010;
           end
         end
         
         //D0 T6
         //D1 T6
         //D2 T6
         //D3 D4 T5
         //D5 - D11 T5
         //D12 - D16 T6 or T5
         //D17 D20 T4
         //D18 D19 T5
         //D22 - D29 T7 or T6 or T5
         //D30 T7
         //D31 T6
         //D32 T4
         //D[33] T7
        // Clean up
        
        if ((T[4] && (D[17] || D[18] || D[19] || D[20] || D[32] ) ||
            (T[5] && (D[3] || D[4] || D[5] || D[6] || D[7] || D[8] || D[9] || D[10] || D[11])) ||
            (T[6] && D[0]) || D[1] || D[2] ||D[31]) ||
            ((sources[0] == 0 && sources[1] == 0) && T[7] && (D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] ||
                                        D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])) ||
            ((sources[1] == 1 && sources[0] == 1) && T[5] && (D[12] || D[13] || D[14] || D[15]|| D[16] || D[21] ||
                                        D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29])  )   ||
            ((sources[1] == 0 && sources[0] == 1) || (sources[1] == 0 && sources[0] == 1) && T[6]  && (D[12] || D[13] || D[14] || D[15]|| D[16]
                                                    || D[21] || D[22] || D[23] || D[24] || D[25] || D[26] || D[27] || D[28] || D[29]))             
                                        ||
              (T[7] && (D[30] || D[33]))          
            ) begin
            SC_Reset <= 1; // Reset the counter to zero
            RF_RegSel <= 4'bZZZZ;
            RF_ScrSel <= 4'bZZZZ;
            ARF_RegSel <= 4'bZZZZ;
            RF_FunSel <= 3'bZZZ;
            ARF_FunSel <= 3'bZZZ;
            Mem_CS <= 1;
            Mem_WR <= 0;
            MuxASel <= 2'bZZ;
            MuxBSel <= 2'bZZ;
            MuxCSel <= 1'bZ;
        end
         
      end
endmodule

module CPUSystem(input wire Clock, input wire Reset, input wire [7:0] T);

    wire [2:0] RF_OutASel;
    wire [2:0] RF_OutBSel;
    wire [2:0] RF_FunSel;
    wire [3:0] RF_RegSel;
    wire [3:0] RF_ScrSel;
    wire [4:0] ALU_FunSel;
    wire [1:0] ARF_OutCSel; 
    wire [1:0] ARF_OutDSel; 
    wire [2:0] ARF_FunSel; 
    wire [2:0] ARF_RegSel;
    wire IR_LH; 
    wire Write;
    wire [15:0] IROut;
    wire Mem_WR;
    wire Mem_CS;
    wire [1:0] MuxASel;
    wire [1:0] MuxBSel;
    wire MuxCSel;
    wire WF;
    wire [3:0] ALU_FlagReg;

    ArithmeticLogicUnitSystem _ALUSystem(
        .RF_OutASel(RF_OutASel), 
        .RF_OutBSel(RF_OutBSel), 
        .RF_FunSel(RF_FunSel),
        .RF_RegSel(RF_RegSel),
        .RF_ScrSel(RF_ScrSel),
        .ALU_FunSel(ALU_FunSel),
        .ARF_OutCSel(ARF_OutCSel), 
        .ARF_OutDSel(ARF_OutDSel), 
        .ARF_FunSel(ARF_FunSel),
        .ARF_RegSel(ARF_RegSel),
        .IR_LH(IR_LH),
        .IR_Write(Write),
        .Mem_WR(Mem_WR),
        .Mem_CS(Mem_CS),
        .MuxASel(MuxASel),
        .MuxBSel(MuxBSel),
        .MuxCSel(MuxCSel),
        .ALUOutFlag(ALU_FlagReg),
        .ALU_WF(WF),
        .Clock(Clock));
        
    control_unit CU(
        .clk(Clock),
        .IROut(IROut),
        .ARF_FunSel(ARF_FunSel), 
        .ARF_RegSel(ARF_RegSel),
        .ARF_OutCSel(ARF_OutCSel), 
        .ARF_OutDSel(ARF_OutDSel),
        .ALU_FlagReg(ALU_FlagReg), 
        .ALU_FunSel(ALU_FunSel), 
        .RF_OutASel(RF_OutASel), 
        .RF_OutBSel(RF_OutBSel), 
        .RF_FunSel(RF_FunSel), 
        .RF_RegSel(RF_RegSel), 
        .RF_ScrSel(RF_ScrSel),
        .MuxASel(MuxASel), 
        .MuxBSel(MuxBSel), 
        .MuxCSel(MuxCSel), 
        .Mem_CS(Mem_CS), 
        .Mem_WR(Mem_WR), 
        .IR_LH(IR_LH), 
        .Write(Write),
        .WF(WF),
        .RESET(Reset),
        .T(T));
endmodule