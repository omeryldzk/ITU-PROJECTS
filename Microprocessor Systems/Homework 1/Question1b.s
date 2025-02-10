W_CAPACITY   EQU    0xC8                    ; Knapsack capacity (50x4)
W_NUMBER   	 EQU    0x32                  ; Knapsack capacity 50
SIZE         EQU    0x0C                    ; Number of items 3x4

		AREA    My_Array, DATA, READWRITE     ; Data section
        ALIGN   
DP_array  SPACE   W_CAPACITY               ; DP array (W_CAPACITY)
DP_end
        AREA    copy_array, CODE, READONLY  ; Code section
        ENTRY
        THUMB
        ALIGN 

__main FUNCTION
        EXPORT __main

        MOVS    R0,#SIZE               ; Load SIZE into R0 (number of items)
        LDR     R1,=DP_array           ; Load address of DP_array into R1
        BL      knap                	; Call knapSack function
		LDR     R2,=w_array            	; Base address of weight array
        LDR     R1,=p_array            	; Base address of profit array
        LDR     R3,=DP_array           	; Base address of DP_array
stop    B       stop                    ; Infinite loop to stop program
        ALIGN
        ENDFUNC

;========================
; knapSack function
;========================

knap 	MOVS    R2,#0                  	; i = 0 (outer loop index)
out     CMP     R2,R0                  	; Compare i with SIZE 
        BGT     return         			; If i > SIZE, exit loop
        MOVS    R3,#W_NUMBER         	; Set initial capacity for inner loop
in      CMP     R3,#0                  	; Check if capacity is >= 0
        BEQ     next             		; If capacity < 0, move to next item

        ; Load weight and profit for current item

        LDR     R4,=w_array            	; Base address of weight array
        LDR     R5,[R4,R2]    			; Load weight[i] into R5
        CMP     R5,R3                  	; If weight[i] > capacity, skip
        BGT     dec     				; Skip this item if it's too heavy

        ; Compute profit including current item
		
        SUBS    R6,R3,R5               	; R6 = capacity - weight[i]
        LDR     R7,=p_array            	; Base address of profit array
		MOVS    R5,#4
		MULS	R6,R5,R6				;capacity - weight[i] X 4
        LDR     R4,[R7,R2]    			; Load profit[i] into R4
        LDR     R7,[R1,R6]    			; Load DP[capacity - weight[i]] into R7
        ADDS    R7,R7,R4              	; R7 = DP[capacity - weight[i]] + profit[i]

		; Calculate max(DP[capacity], R7)
		
		MOVS	R6,R3
		MULS    R6,R5,R6
        LDR     R4,[R1,R6]    		; Load DP[capacity] into R4
        CMP     R7,R4               ; Compare the new profit with current DP[capacity]
        BGT     stor        		; If R7 > DP[capacity], update it
        B       dec      			; Otherwise, continue with the loop
stor    STR     R7,[R1,R6]    		; Store new profit in DP[capacity]
dec     SUBS    R3,R3,#1            ; Decrement capacity
        B       in              	; Repeat the inner loop
next    ADDS    R2,R2,#4            ; Increment i to process the next item
        B       out              	; Repeat the outer loop

return  MOVS 	R3,#W_NUMBER
		MOVS 	R2,#4
		MULS    R3,R2,R3
		LDR    	R0,[R1,R3]			 ;Load max profit from DP[W_CAPACITY] into R0
        BX      LR                   ; Return from knapSack function

;========================
; Data section
;========================
p_array  DCD     60, 100, 120          ; Profit array
p_end
w_array  DCD     10, 20, 30           
w_end	; Weight array



	END