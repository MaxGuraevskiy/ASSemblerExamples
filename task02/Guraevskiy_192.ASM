format PE console

entry main

include 'win32a.inc'

section '.data' data readable writable
        SizeArray                db 'size of vector? ', 0
        SizeArrayIsNotCorrect    db 'Incorrect size of vector = %d', 10, 0
        ElementOfArray           db '[%d]? ', 0
        ScanInt                  db '%d', 0
        Summ                     db 'Summa = %d', 10, 0
        ElementOfArrayOut        db '[%d] = %d', 10, 0
        strEmptyLine             db '', 10, 0
        StringFirstArray         db 'That`s ur first array (Array A)', 10, 0
        StringSecondArray        db 'That`s ur second array (Array B)', 10, 0

        vec_size     dd 0
        i            dd ?
        tmp          dd ?
        tmpStack     dd ?
        vecIn        rd 100
        vecOut       rd 100
        NULL = 0


section '.code' code executable readable
main:
;input
        call Input
;output1
        push strEmptyLine
        call [printf]

        push StringFirstArray
        call [printf]

        mov ebx, vecIn
        call VectorOutput
;operations
        call VectorChanger
;output2
        push strEmptyLine
        call [printf]

        push StringSecondArray
        call [printf]

        mov ebx, vecIn
        call VectorOutput
;end
        call finish
;-----------------------------------------------------------------------
Input:
        push SizeArray
        call [printf]
        add esp, 4

        push vec_size
        push ScanInt
        call [scanf]
        add esp, 8

        mov eax, [vec_size]
        cmp eax, 0
        jg  getVector

        push vec_size
        push SizeArrayIsNotCorrect
        call [printf]
        call finish
getVector:
        xor ecx, ecx
        mov ebx, vecIn
getVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        jge endInputVector       ; to end of loop

        ; input element
        mov [i], ecx
        push ecx
        push ElementOfArray
        call [printf]
        add esp, 8

        push ebx
        push ScanInt
        call [scanf]
        add esp, 8

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp getVecLoop
endInputVector:
        ret
;-----------------------------------------------------------------------

;-----------------------------------------------------------------------
VectorChanger:
        xor ecx, ecx
        mov eax, vecIn
        mov ebx, vecOut
changeLoop:
        cmp ecx, [vec_size]
        je endChange

        cmp [eax], dword 5
        jle notMore5
;if a[i]>5
       ; push srtMore5
       ; call [printf]

        mov ebx, eax
        add [ebx],dword 5
        jmp littleEnd
notMore5:
        cmp [eax], dword -5
        jge between
;if a[i]<-5
        mov ebx, eax
        sub [ebx], dword 5
        jmp littleEnd
between:
        mov [eax], dword 0
        mov ebx, eax
        jmp littleEnd

littleEnd:
        add eax, 4
        add ebx, 4
        inc ecx
        jmp changeLoop


endChange:
        ret

;-----------------------------------------------------------------------

;-----------------------------------------------------------------------
VectorOutput:
        mov [tmpStack], esp
        xor ecx, ecx            ; ecx = 0
putVecLoop:
        mov [tmp], ebx
        cmp ecx, [vec_size]
        je endOutputVector      ; to end of loop
        mov [i], ecx

        ; output element
        push dword [ebx]
        push ecx
        push ElementOfArrayOut
        call [printf]

        mov ecx, [i]
        inc ecx
        mov ebx, [tmp]
        add ebx, 4
        jmp putVecLoop
endOutputVector:
        mov esp, [tmpStack]
        ret
;-----------------------------------------------------------------------

finish:
        call [getch]
        push NULL
        call [ExitProcess]


;-----------------------------------------------------------------------
section '.idata' import data readable
        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               getch, '_getch',\
               printf, 'printf',\
               scanf, 'scanf'
