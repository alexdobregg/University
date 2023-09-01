%include "includes/io.inc"

extern getAST
extern freeAST

section .data
 array: times 600 dd 0
 var: dd 0
 
section .bss
    ; La aceasta adresa, scheletul stocheaza radacina arborelui
    root: resd 1
    

section .text

;pregatesc registrii pentru operatiile din atoi
prev_atoi:
    push ebp
    mov ebp, esp
    xor eax,eax
    xor ebx,ebx
    xor edx, edx
    mov ecx, [ebp + 8]
 
;functie de convertire din string in intreg 
;se ia byte cu byte si se formeaza numarul
atoi:
    mov dl, byte [ecx]
    cmp dl,0
    je exit
    cmp dl, '-' ;daca e numar negativ
    je atoi_minus
    sub dl,0x30 ;se scade valoarea lui 0
    imul eax,10 ;pentru a forma numarul
    add eax,edx
    inc ecx
    jmp atoi
 
;pentru numerele negative
atoi_minus:
    inc ecx
    mov dl, byte [ecx]
    cmp dl,0
    je exit
    sub dl,0x30
    imul eax,10
    sub eax,edx
    jmp atoi_minus

;pentru a reface stiva
exit:
    mov dword [ebp + 8], eax
    leave
    ret

;functia recursiva de parcurgere 
;a arborelui
crossing_tree:
    push ebp
    mov ebp, esp 
    ;pentru a accesa rootul
    mov ecx, dword [ebp + 8]
    
  
    mov esi, [ebp + 8]
    mov edi, [esi + 4]
    cmp edi, 0
    jz leaf
   
    
    mov eax, [ebp + 8] ;pentru partea stanga a arborekui
    mov edx, [eax + 4]
    mov ebx, [edx]
   
    
    ;merge recursiv pentru stanga
    push edx 
    call crossing_tree
    pop edx
    mov ecx, edx
    mov ecx, [ecx]
    mov edi, [edx]
    
    mov eax, [ebp+8]
    mov edx, [eax+8] ; pentru partea dreapta a arborelui
    mov ebx, [edx] 
     
    ;merge recursiv pentru dreapta
    push edx
    call crossing_tree
    pop edx
    mov ecx, edx
    mov ecx, [ecx]
    
    ;identifica operatia ce trebuie facuta
    mov ecx, dword [ebp + 8]
    mov eax, [ecx]
    mov ebx, 0
    mov bl, byte[eax]
    cmp bl, '-'
    je substraction
    cmp bl, '+'
    je addition
    cmp bl, '*'
    je multiplication
    cmp bl, '/'
    je division
   
;pentru a reface stiva
jump_label:
    leave
    ret 
 
;in cazul frunzelor,
;le adaug intr-un vector
;(var fiind indexul)
;si fac operatia cu cate 2 frunze
;recursiv     
leaf:
    push dword [esi]
    call prev_atoi ;convertirea stringului
    pop ebx
    add dword[var], 4
    mov edi,dword[var]
    mov[array+edi], ebx ;adaug in vector valoarea din frunza
    jmp jump_label

;scadere          
substraction:
    mov edi, [var]
    mov esi, dword [array + edi - 4]
    sub esi, dword [array + edi]
    mov dword [array + edi - 4], esi
    sub dword [var], 4
    jmp jump_label

;adunare
addition:
    mov edi, [var]
    mov esi, dword [array + edi - 4]
    add esi, dword [array + edi]
    mov dword [array + edi - 4], esi
    sub dword [var], 4
    jmp jump_label
    
;impartire    
division:
    xor edx, edx
    mov edi, [var]
    mov esi, dword [array + edi]
    mov eax, dword [array + edi - 4]
    cdq ;pentru valorile negative
    idiv esi
    mov dword [array + edi - 4], eax
    sub dword [var], 4
    jmp jump_label

;inmultire      
multiplication:
    xor edx, edx
    mov edi, [var]
    mov esi, dword [array + edi]
    mov eax, dword [array + edi - 4]
    imul eax, esi
    mov dword [array + edi - 4], eax
    sub dword [var], 4
    jmp jump_label
   

global main
main:
    mov ebp, esp; for correct debugging
    ; NU MODIFICATI
    push ebp
    mov ebp, esp
    
    ; Se citeste arborele si se scrie la adresa indicata mai sus
    call getAST
    mov [root], eax
    push dword [root]
    call crossing_tree
    add esp, 4
    mov edi, [var]
    mov eax, [array + edi]
    
    ;rezultatul final, ultima valoare ramasa in vector
    PRINT_DEC 4, eax
    NEWLINE
   
    
    ; Implementati rezolvarea aici:
    ; NU MODIFICATI
    ; Se elibereaza memoria alocata pentru arbore
    push dword [root]
    call freeAST
    
    xor eax, eax
    leave
    ret