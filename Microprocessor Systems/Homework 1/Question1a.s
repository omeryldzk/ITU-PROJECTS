W_CAPACITY   EQU    0x32                    ; Knapsack capacity (50)
SIZE         EQU    0x3                    ; Number of items        AREA My_Code, CODE, READONLY     ; Code area
		AREA    copy_array, CODE, READONLY  ; Code section
        ENTRY
        THUMB
        ALIGN 

__main FUNCTION
        EXPORT __main                   ; Export main function

        MOVS    R2,#SIZE               	; Load SIZE into R0 (number of items)
        MOVS    R0,#0              		; temp return value
        MOVS    R1,#W_CAPACITY         	; Load W_Capacity into R1
        BL      knap                    ; Call knapSack function
		LDR     R2,=w_array            	; Base address of weight array
        LDR     R1,=p_array            	; Base address of profit array
stop    B       stop                    ; Infinite loop to end program
		ALIGN
		ENDFUNC
;========================
; knapSack function
;========================
knap    PUSH    {LR}                    ; Save the return address
        ; Base cases: if n == 0 or W == 0, return 0
        CMP     R2,#0                  	; Compare n with 0
        BEQ     base_case               ; If n == 0, go to base case
        CMP     R1,#0                  	; Compare W with 0
        BEQ     base_case               ; If W == 0, go to base case
	
        ; Check if weight[n-1] > W
        SUBS    R3,R2,#1              	; R3 = n - 1
        MOVS    R4,#4                  	; Each int is 4 bytes
        MULS    R3,R4,R3              	; R3 = (n - 1) * 4
        LDR     R5,=w_array            	; Base address of weight array
        LDR     R5,[R5, R3]            	; Load weight[n - 1] into R5
        CMP     R5,R1                  	; Compare weight[n - 1] with W
        BGT     exc            			; If weight[n - 1] > W, go to exclude item

		BL 		excT					; knapSack(W,n-1)
        ; Recursive case 1: Include current item
		SUBS    R3,R2,#1              	; R3 = n - 1
        MOVS    R4,#4                  	; Each int is 4 bytes
        MULS    R3,R4,R3              	; R3 = (n - 1) * 4
		LDR     R5,=w_array            	; Base address of weight array
        LDR     R5,[R5, R3]            	; Load weight[n - 1] into R5
        SUBS    R5,R1,R5              	; R5 = W - weight[n - 1]
        SUBS    R2,R2,#1        		; R2 = n - 1 
		MOVS	R1,R5					; R1 = W - weight[n - 1]
		PUSH    {R1,R2}                	; Save current n and W on stack (n-1) and (W - weight[n - 1])
        BL      knap                    ; Recursive call: knapSack(W - weight[n - 1], n - 1)
		POP     {R1,R2}                	; Restore n and W from the stack
        MOVS    R7, R0                  ; Store result in R7

        ; Add profit[n - 1] to the result of knapSack(W - weight[n - 1], n - 1)
		MOVS    R3,R2              		; R3 = n - 1
        MOVS    R4,#4                  	; Each int is 4 bytes
        MULS    R3,R4,R3              	; R3 = (n - 1) * 4
        LDR     R5,=p_array            	; Base address of profit array
        LDR     R5,[R5,R3]            	; Load profit[n - 1] into R5
        ADDS    R7,R7,R5              	; R7 = profit[n - 1] + knapSack(W - weight[n - 1], n - 1)
	
		; Get max of include and exclude results
        CMP     R7,R6                  	; Compare include result (R7) with exclude result (R6)
        BGT     end_case                ; If include result is greater, keep R7
        MOVS    R0,R6                  	; Otherwise, keep exclude result in R7
        POP     {PC}                    ; Return from function

        ; Recursive case 2: Exclude current item knapSack(W,n-1)
excT	PUSH    {LR}                    ; Save the return address
		PUSH    {R1,R2}                	; Save current n and W on stack
        SUBS    R2,R2,#1              	; R0 = n - 1
        BL      knap                    ; Recursive call: knapSack(W, n - 1)
        MOVS    R6,R0                  	; Store result in R6
        POP     {R1,R2}                	; Restore n and W from the stack
		POP     {PC}                    ; Return from function

        ; Recursive case 3: Exclude current item knapSack(W,n-1)
exc		SUBS    R2,R2,#1              	; R0 = n - 1
        BL      knap                    ; Recursive call: knapSack(W, n - 1)
        MOVS    R6,R0                  	; Store result in R6
		POP     {PC}                    ; Return from function

end_case
        MOVS    R0, R7                  ; Move max result into R0 (return value)
        POP     {PC}                    ; Return from function

; Base case: Return 0
base_case
        MOVS    R0,#0                  	; Base case result: 0
        POP     {PC}                    ; Return from function

;========================
; Data section
;========================
       
p_array DCD      60,100,120            ; Profit array
w_array DCD    	 10,20,30              ; Weight array
        END                             ; End of the program
