		AREA    Timing_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  Systick_Start_asm
        EXPORT  Systick_Stop_asm
        EXPORT  SysTick_Handler            ; Uncomment as required
        EXTERN  ticks                      ; ticks counter in the main C program

SysTick_Handler FUNCTION
        PUSH    {R4, LR}                   ; Save registers 
        LDR     R4, =ticks                 ; Load address of ticks variable
        LDR     R1, [R4]                   ; Load current value
        ADDS    R1, R1, #1                 ; Increment ticks
        STR     R1, [R4]                   ; Store back
        POP     {R4, PC}                   ; Restore and return
        ENDFUNC

Systick_Start_asm FUNCTION
        PUSH    {R4-R7, LR}                ; Save registers 

        ; Configure SysTick registers
        LDR     R5, =0xE000E010           ; SYST_CSR
        LDR     R6, =0xE000E014           ; SYST_RVR
        LDR     R7, =0xE000E018           ; SYST_CVR

        ; Clear ticks
        LDR     R0, =ticks
        MOVS    R1, #0
        STR     R1, [R0]                  ; ticks = 0
        
        ; Load the correct reload value 
        MOVS     R4, #249                  ; 
        
        ; Configure timer
        STR     R4, [R6]                  ; Set reload value
        STR     R1, [R7]                  ; Clear current value
        MOVS    R0, #7                    ; Enable CLKSOURCE + TICKINT + ENABLE
        STR     R0, [R5]                  ; Start timer
        
        POP     {R4-R7, PC}               ; Restore and return
        ENDFUNC

Systick_Stop_asm FUNCTION
        PUSH    {R4, LR}                  ; Save registers 
        
        ; Disable SysTick
        LDR     R4, =0xE000E010          ; SYST_CSR
        MOVS    R1, #0                    ; Disable all
        STR     R1, [R4]                  ; Stop timer
        
        ; Get elapsed ticks
        LDR     R4, =ticks               ; Load ticks address
        LDR     R0, [R4]                 ; Return value in R0
        
        POP     {R4, PC}                 ; Restore and return
        ENDFUNC

        END