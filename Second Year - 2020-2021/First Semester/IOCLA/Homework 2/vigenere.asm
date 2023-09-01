%include "io.mac"

section .data
    ptextlen dd 0
    keylen dd 0
    cat dd 0
    re dd 0
    plaintxt dd 0
section .text
    global vigenere
    extern printf

vigenere:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     edx, [ebp + 8]      ; ciphertext
    mov     esi, [ebp + 12]     ; plaintext
    mov     ecx, [ebp + 16]     ; plaintext_len
    mov     edi, [ebp + 20]     ; key
    mov     ebx, [ebp + 24]     ; key_len
    ;; DO NOT MODIFY

    ;; TODO: Implement the Vigenere cipher
    
    mov dword [keylen], ebx
    mov dword [ptextlen], ecx
    mov dword [plaintxt], esi
    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx

loop1:
    mov cl, [esi + eax] ; Verificam daca un caracter este litera sau nu.
    cmp cl, 0x41 ; comparam cu 'A'
    jl notchar
    cmp cl, 0x5A ; comparam cu 'Z'
    jle char
    cmp cl, 0x61 ; comparam cu 'a'
    jl notchar
    cmp cl, 0x7A ; comparam cu 'z'
    jg notchar
char:
    xor ecx, ecx
    mov cl, [edi + ebx] ; daca este litera, putem extinde cheia cu o litera din key
    inc ebx
    jmp cstrcheie
notchar:
    mov cl, 0x20 ; daca nu este litera, in cheia extinsa punem spatiu
cstrcheie:
    mov [edx + eax], cl
    inc eax
    cmp ebx, dword [keylen]
    jne cploop
    xor ebx, ebx
cploop:
    cmp eax, dword [ptextlen]
    jl loop1 ; la finalul loopului am obtinut cheia extinsa in edx.

    mov ecx, dword [ptextlen]

loopa:
    mov al, [esi + ecx - 1]
    ;; Vedem daca se afla intre 'A' si 'Z' sau intre 'a' si 'z'
    cmp al, 0x41 ; comparam cu 'A'
    jl et4
    cmp al, 0x5A ; comparam cu 'Z'
    jle found_char
    cmp al, 0x61 ; comparam cu 'a'
    jl et4
    cmp al, 0x7A ; comparam cu 'z'
    jg et4
found_char:
    mov edi, -1 ; Se incrementeaza o data la inceput si asa obtinem 0 pentru 'A'
    xor ebx, ebx
    xor eax, eax
    mov al, 0x40 ; Din acelasi motiv ca pentru edi, se obtine 'A' dupa o incrementare
    mov bl, [edx + ecx - 1]
loopc:
    inc al
    inc edi
    cmp al, bl
    jl loopc ; Obtinem valoarea cu care trebuie modificata litera
    xor eax, eax
    mov al, [esi + ecx - 1]
    xor ebx, ebx
    cmp ebx, edi
    je et4
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
et4:
    mov [edx + ecx - 1], al
    loop loopa

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY