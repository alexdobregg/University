%include "io.mac"

section .data
    hayst_len dd 0
    needl_len dd 0
    idx dd 0

section .text
    global my_strstr
    extern printf

my_strstr:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edi, [ebp + 8]      ; substr_index
    mov     esi, [ebp + 12]     ; haystack
    mov     ebx, [ebp + 16]     ; needle
    mov     ecx, [ebp + 20]     ; haystack_len
    mov     edx, [ebp + 24]     ; needle_len
    ;; DO NOT MODIFY

    ;; TO DO: Implement my_strstr

    mov dword [hayst_len], ecx
    mov ecx, 1
    mov dword [needl_len], edx
    mov edx, 1
    xor eax, eax

loop1:
    mov al, [esi + ecx - 1]
    cmp al, [ebx + edx - 1]
    jne et
    mov dword [idx], ecx ; salvam indexul aparitiei
    inc edx
    cmp edx, dword [needl_len] ; daca am ajuns la finalul subsirului am gasit indexul
    jg et3
    jmp et2
et:
    mov edx, 1 ; resetam indexul pentru subsir
    mov dword [idx], 0 ; resetam indexul aparitiei
et2:
    inc ecx
    cmp ecx, dword [hayst_len]
    jle loop1
et3:
    mov eax, dword [idx]
    cmp eax, dword [needl_len]
    jl et4 ; in cazul in care s-a terminat sirul dar nu a fost verificat to subsirul.
    cmp eax, 0 ; daca indexul e 0, vom returna lungimea sirului + 1
    jne et5
et4:
    mov eax, dword [hayst_len]
    add eax, 1
    mov [edi], eax
    jmp et6
et5:
    sub eax, dword [needl_len] ; daca nu este 0, vom scadea lungimea subsirului
    mov [edi], eax
et6:
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
