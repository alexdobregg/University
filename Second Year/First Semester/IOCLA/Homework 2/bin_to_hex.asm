%include "io.mac"

section .data
    cat dd 0
    re dd 0
    retcat dd 0
    num4biti dd 0
    hexstring dd 0

section .text
    global bin_to_hex
    extern printf

bin_to_hex:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; hexa_value
    mov     esi, [ebp + 12]     ; bin_sequence
    mov     ecx, [ebp + 16]     ; length
    ;; DO NOT MODIFY

    ;; TODO: Implement bin to hex
    
    ;; Am grupat caracterele cate 4 si am obtinut numarul lor, cat si cate mai raman.
    mov al, cl
    mov bl, 0x4
    div bl
    xor ebx, ebx
    mov bl, al
    mov dword [cat], ebx
    mov dword [retcat], ebx
    xor ebx, ebx
    mov bl, ah
    mov dword [re], ebx
    mov eax, ecx

loop1:
    xor ebx, ebx
    xor ecx, ecx
    xor edi, edi
    mov dword [num4biti], 0
loop2:
    mov edi, 1 ; in loop2 facem suma din grupare daca caracterul este 1
    mov bl, [esi + eax - 1]
    cmp bl, '0'
    je et
    shl edi, cl
    add dword [num4biti], edi
et:
    inc cl
    dec eax
    cmp cl, 4
    jl loop2

    mov ebx, dword [num4biti]
    mov edi, dword [cat]
    cmp dword [re], 0 ; Daca restul e 0 nu mai trebuie sa tinem prima pozitie libera
    jne cmpet
    dec edi

cmpet:
    cmp ebx, 0
    je caz0
    cmp ebx, 1
    je caz1
    cmp ebx, 2
    je caz2
    cmp ebx, 3
    je caz3
    cmp ebx, 4
    je caz4
    cmp ebx, 5
    je caz5
    cmp ebx, 6
    je caz6
    cmp ebx, 7
    je caz7
    cmp ebx, 8
    je caz8
    cmp ebx, 9
    je caz9
    cmp ebx, 10
    je cazA
    cmp ebx, 11
    je cazB
    cmp ebx, 12
    je cazD
    cmp ebx, 13
    je cazD
    cmp ebx, 14
    je cazE
    cmp ebx, 15
    je cazF
caz0:
    mov cl, '0'
    jmp etfinloop
caz1:
    mov cl, '1'
    jmp etfinloop
caz2:
    mov cl, '2'
    jmp etfinloop
caz3:
    mov cl, '3'
    jmp etfinloop
caz4:
    mov cl, '4'
    jmp etfinloop
caz5:
    mov cl, '5'
    jmp etfinloop
caz6:
    mov cl, '6'
    jmp etfinloop
caz7:
    mov cl, '7'
    jmp etfinloop
caz8:
    mov cl, '8'
    jmp etfinloop
caz9:
    mov cl, '9'
    jmp etfinloop
cazA:
    mov cl, 'A'
    jmp etfinloop
cazB:
    mov cl, 'B'
    jmp etfinloop
cazC:
    mov cl, 'C'
    jmp etfinloop
cazD:
    mov cl, 'D'
    jmp etfinloop
cazE:
    mov cl, 'E'
    jmp etfinloop
cazF:
    mov cl, 'F'

etfinloop:
    mov [edx + edi], cl ; mutam litera in edx si continuam pana cand terminam gruparile
    dec dword [cat]
    cmp dword [cat], 0
    jg loop1

    cmp dword [re], 0
    je newline
    inc dword [retcat]
    xor ebx, ebx
    xor ecx, ecx
    xor edi, edi
    mov dword [num4biti], 0
    ;; Loop in cazul in care ne mai raman caractere negrupate.
loop3:
    mov edi, 1
    mov bl, [esi + eax - 1]
    cmp bl, '0'
    je loop3et
    shl edi, cl
    add dword [num4biti], edi
loop3et:
    dec eax
    inc cl
    cmp ecx, dword [re]
    jl loop3

    mov ebx, dword [num4biti]
    xor ecx, ecx

    cmp ebx, 0
    je caz02
    cmp ebx, 1
    je caz12
    cmp ebx, 2
    je caz22
    cmp ebx, 3
    je caz32
    cmp ebx, 4
    je caz42
    cmp ebx, 5
    je caz52
    cmp ebx, 6
    je caz62
    cmp ebx, 7
    je caz72
    cmp ebx, 8
    je caz82
    cmp ebx, 9
    je caz92
    cmp ebx, 10
    je cazA2
    cmp ebx, 11
    je cazB2
    cmp ebx, 12
    je cazD2
    cmp ebx, 13
    je cazD2
    cmp ebx, 14
    je cazE2
    cmp ebx, 15
    je cazF2
caz02:
    mov cl, '0'
    jmp etfinal
caz12:
    mov cl, '1'
    jmp etfinal
caz22:
    mov cl, '2'
    jmp etfinal
caz32:
    mov cl, '3'
    jmp etfinal
caz42:
    mov cl, '4'
    jmp etfinal
caz52:
    mov cl, '5'
    jmp etfinal
caz62:
    mov cl, '6'
    jmp etfinal
caz72:
    mov cl, '7'
    jmp etfinal
caz82:
    mov cl, '8'
    jmp etfinal
caz92:
    mov cl, '9'
    jmp etfinal
cazA2:
    mov cl, 'A'
    jmp etfinal
cazB2:
    mov cl, 'B'
    jmp etfinal
cazC2:
    mov cl, 'C'
    jmp etfinal
cazD2:
    mov cl, 'D'
    jmp etfinal
cazE2:
    mov cl, 'E'
    jmp etfinal
cazF2:
    mov cl, 'F'
etfinal:
    mov [edx + 0], cl
newline:
    mov eax, dword [retcat]
    mov cl, 10 ; punem \n la final
    mov [edx + eax], cl
    
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY