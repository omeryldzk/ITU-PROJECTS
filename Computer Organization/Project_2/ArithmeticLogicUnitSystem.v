`timescale 1ns / 1ps
module MUX_2TO1(S, D0, D1, Y);
    input wire S;
    input wire [7:0] D0;
    input wire [7:0] D1;
    output reg [7:0] Y;

    always@(*) begin
        case(S)
            1'b0: Y <= D0;
            1'b1: Y <= D1;
            default: Y <= D0;
        endcase
    end
endmodule

module MUX_4TO1(S, D0, D1, D2, D3, Y);
    input wire [1:0] S;
    input wire [15:0] D0; 
    input wire [15:0] D1;
    input wire [7:0] D2;
    input wire [7:0] D3;
    output reg [15:0] Y;

    always@(*) begin
        case(S)
            2'b00: Y <= D0;
            2'b01: Y <= D1;
            2'b10:
                begin
                     Y[7:0] <= D2;   
                     Y[15:8] <= {8{1'b0}};   
                 end 
            2'b11: Y <= D3;    
            default: Y <= D0;
        endcase
    end
endmodule

module ArithmeticLogicUnitSystem (
    RF_OutASel, 
    RF_OutBSel, 
    RF_FunSel,
    RF_RegSel,
    RF_ScrSel,
    ALU_FunSel,
    ARF_OutCSel, 
    ARF_OutDSel, 
    ARF_FunSel,
    ARF_RegSel,
    IR_LH,
    IR_Write,
    Mem_WR,
    Mem_CS,
    MuxASel,
    MuxBSel,
    MuxCSel,
    ALU_WF,
    ALUOutFlag,
    Clock
);
     
    //RF
    input wire [2:0] RF_OutASel; 
    input wire [2:0] RF_OutBSel; 
    input wire [2:0] RF_FunSel; 
    input wire [3:0] RF_RegSel;
    input wire [3:0] RF_ScrSel;
    wire [15:0] OutA; // RF_OutA
    wire [15:0] OutB; // RF_OutB
    
    //ALU
    input wire [4:0] ALU_FunSel;
    input wire ALU_WF;
    wire [15:0] ALUOut;
    input wire [3:0] ALUOutFlag;

    //ARF
    input wire [1:0] ARF_OutCSel; 
    input wire [1:0] ARF_OutDSel; 
    input wire [2:0] ARF_FunSel;
    input wire [2:0] ARF_RegSel;
    wire [15:0] OutC;    // ARF_OutC
    wire [15:0] OutD;    // ARF_OutD
    
    //IR
    input wire IR_LH;
    input wire IR_Write;
    wire [15:0] IROut;

    //Memory
    input wire Mem_WR;
    input wire Mem_CS;
    wire [7:0] MemOut;
    wire [15:0] Address;

    // MUX A
    input wire [1:0] MuxASel;
    wire [15:0] MuxAOut;
    
    //MUX B
    input wire [1:0] MuxBSel;
    wire [15:0] MuxBOut;
    //MUX C
    input wire MuxCSel;
    wire [7:0] MuxCOut;
    
    input wire Clock;
    assign Address = OutD;
    
    Memory MEM(
        .Clock(Clock), 
        .address(Address), 
        .data(MuxCOut), 
        .wr(Mem_WR), 
        .cs(Mem_CS), 
        .o(MemOut)
    );

    ArithmeticLogicUnit ALU(
        .A(OutA), 
        .B(OutB), 
        .Clock(Clock),
        .FunSel(ALU_FunSel), 
        .ALUOut(ALUOut), 
        .FlagsOut(ALUOutFlag),
        .WF(ALU_WF)
    );
    
    MUX_4TO1 MUXA(
        .S(MuxASel), 
        .D0(ALUOut), 
        .D1(OutC), 
        .D2(MemOut), 
        .D3(IROut[7:0]), 
        .Y(MuxAOut)
    );

    MUX_4TO1 MUXB(
        .S(MuxBSel), 
        .D0(ALUOut), 
        .D1(OutC), 
        .D2(MemOut), 
        .D3(IROut[7:0]), 
        .Y(MuxBOut)
    );

    MUX_2TO1 MUXC(
        .S(MuxCSel), 
        .D0(ALUOut[7:0]), 
        .D1(ALUOut[15:8]), 
        .Y(MuxCOut)
    );

    InstructionRegister IR(
        .Clock(Clock),
        .I(MemOut),
        .Write(IR_Write),
        .LH(IR_LH),
        .IROut(IROut)
    );

    RegisterFile RF(
        .Clock(Clock),
        .I(MuxAOut), 
        .OutASel(RF_OutASel), 
        .OutBSel(RF_OutBSel), 
        .FunSel(RF_FunSel), 
        .RegSel(RF_RegSel), 
        .ScrSel(RF_ScrSel), 
        .OutA(OutA), 
        .OutB(OutB)
    );
    
    AddressRegisterFile ARF(
        .I(MuxBOut), 
        .OutCSel(ARF_OutCSel), 
        .OutDSel(ARF_OutDSel), 
        .FunSel(ARF_FunSel), 
        .RegSel(ARF_RegSel),
        .OutC(OutC), 
        .OutD(OutD), 
        .Clock(Clock)
    );
endmodule

