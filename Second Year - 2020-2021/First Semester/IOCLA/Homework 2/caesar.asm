%include "io.mac"

section .text
    global caesar
    extern printf

caesar:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     edi, [ebp + 16]     ; key
    mov     ecx, [ebp + 20]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement the caesar cipher

loopa:
    mov al, [esi + ecx - 1]
    ;; Vedem daca se afla intre 'A' si 'Z' sau intre 'a' si 'z'
    cmp al, 0x41  ; comparam cu 'A'
    jl et
    cmp al, 0x5A  ; comparam cu 'Z'
    jle found_char
    cmp al, 0x61  ; comparam cu 'a'
    jl et
    cmp al, 0x7A  ; comparam cu 'z'
    jg et
found_char:
    mov ebx, 0
    cmp ebx, edi ; comparam cu cat trebuie modificat caracterul
    je et
loopb:
    cmp al, 0x5A ; daca ajunge la 'Z' se transforma in 'A'
    je change
    cmp al, 0x7A ; daca ajunge la 'z' se transforma in 'a'
    je change2
    inc al ; daca nu e in niciunul din cazuri, se incrementeaza la urm valoare
    jmp lo
change:
    mov al, 0x41 ; mutam 'A'
    jmp lo
change2:
    mov al, 0x61 ; mutam 'a'
lo:
    inc ebx
    cmp ebx, edi
    jl loopb
et:
    mov [edx + ecx - 1], al
    loop loopa


    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY