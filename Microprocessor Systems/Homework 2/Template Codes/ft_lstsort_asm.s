; Function: ft_lstsort_asm
; Parameters:
;   R0 - Pointer to the list (address of t_list *)
;   R1 - Pointer to comparison function (address of int (*f_comp)(int, int))
        AREA    Sorting_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  ft_lstsort_asm
ft_lstsort_asm FUNCTION
        PUSH    {R0-R1, LR}                ; Save R0 (list), R1 (comparison function), and LR (return address)
        MOVS     R3, #1                     ; R3 = flag to track if swaps occurred (set to 1 initially)
		
sort_loop
        LDR     R2, [SP]                     ; R2 = head pointer of the list
        CMP     R3, #0                     ; Check if any swaps occurred in the previous pass
        BEQ     done                       ; If no swaps occurred, sorting is complete
        MOVS     R3, #0                     ; Reset swap flag for this pass
        LDR     R4, [R2]                     ; Start at the head of the list

traverse
        LDR     R5, [R4, #4]               ; R5 = next node pointer (current->next)
        CMP     R5, #0                     ; Check if we reached the end of the list
        BEQ     check_swaps                ; If end, check if swaps occurred

        ; Compare current node's data with next node's data using ft_cmp
        LDR     R6, [R4]               ; R6 = current node's data (current->data)
        LDR     R7, [R5]               ; R7 = next node's data (next->data)
		LDR     R2, [SP, #4]                ; Preserve the function pointer in R2
        MOVS     R0, R6                     ; Load R6 (current data) into R0 for comparison
        MOVS     R1, R7                     ; Load R7 (next data) into R1 for comparison
		PUSH	{R3}
        BLX     R2                        ; Call ft_cmp(a, b) via preserved pointer
		POP 	{R3}
        CMP     R0, #1                     ; Check if R0 (result of ft_cmp) is true (a < b)
        BEQ     no_swap                    ; If true, no swap is needed

        ; Swap node data
        STR     R7, [R4]               ; Set current node's data to R7 (next node's data)
        STR     R6, [R5]               ; Set next node's data to R6 (current node's data)
        MOVS     R3, #1                     ; Set swap flag to indicate a swap occurred

no_swap
        MOVS     R4, R5                    ; Move to the next node
        B       traverse                   ; Continue traversal

check_swaps
        CMP     R3, #0                     ; Check if any swaps occurred in this pass
        BNE     sort_loop                  ; If swaps occurred, repeat the sorting pass

done
        POP     {R0-R1, PC}                ; Restore registers and return
        ENDFUNC

