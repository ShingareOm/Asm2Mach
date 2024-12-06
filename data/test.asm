; This is a simple program with data directives and instructions

START:
    MOV AX, 5              ; Move value 5 into AX register
    ADD AX, 10             ; Add 10 to AX
    MOV BX, 20             ; Move value 20 into BX register
    JMP END                ; Jump to label END

DATA_SEG:
    DB 0x01, 0x02, 0x03    ; Define byte values
    DW 0x1234, 0x5678      ; Define word values
    RESB 5                 ; Reserve 5 bytes
    RESW 2                 ; Reserve 2 words

END:
    HLT                    ; Halt the program
