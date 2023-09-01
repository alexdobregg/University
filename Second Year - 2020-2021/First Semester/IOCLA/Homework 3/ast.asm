; Dobre Gigi-Alexandru 322CD

section .data
    delim db " ", 0
    inmul dd 10
    op dd 0
    myVect dd 0

section .bss
    root resd 1

section .text
extern malloc
extern strtok
extern strdup
extern check_atoi
extern print_tree_inorder
extern print_tree_preorder
extern evaluate_tree

global create_tree
global iocla_atoi
global create_myNode
global whatOP

; Functie care determina daca este operand sau operator.
; START whatOP

whatOP:
    push ebp
    mov ebp, esp
    push ebx

    ; Eliberam fiecare registru folosit.
    xor eax, eax
    xor ecx, ecx
    xor ebx, ebx

    mov eax, [ebp + 8] ; scoatem in eax stringul.

loopOP:
    mov bl, byte [eax + ecx]
    cmp bl, 0
    je compar
    inc ecx
    jmp loopOP

compar:
    cmp ecx, 1 ; este verificat daca stringul contine mai mult de un caracter.
    jg operand ; caz in care se stie ca stringul va fi un numar.

    mov bl, byte [eax] ; daca e doar de o litera, tot trebuie verificat.
    cmp bl, 0x2D ; este verificat daca e -.
    je operatie
    cmp bl, 0x2B ; este verificat daca e +.
    je operatie
    cmp bl, 0x2A ; este verificat daca e *.
    je operatie
    cmp bl, 0x2F ; este verificat daca e /.
    je operatie

operand:
    mov eax, 1 ; pentru numere mari si numere intre 0 si 9.
    jmp outOP

operatie:
    mov eax, 0

outOP:
    pop ebx
    leave
    ret

; END whatOP

; Functie pentru a crea nodurile arborelui.
; START create_myNode

create_myNode:
    push ebp
    mov ebp, esp
    push esi
    push edi

    ; Eliberam fiecare registru folosit.
    xor esi, esi
    xor edi, edi
    xor eax, eax

    push dword [ebp + 8] ; este alocata memorie pentru data, folosind strdup.
    call strdup
    add esp, 4
    mov esi, eax

    mov edi, 12
    call malloc ; sunt alocati 12 bytes, 4 pentru data, 4 pentru nodL si 4 pentru nodR.
    mov dword [eax], esi ; este pus continutul
    mov dword [eax + 4], 0 ; initial este setat la 0 
    mov dword [eax + 8], 0 ; initial este setat la 0

    pop edi
    pop esi
    leave
    ret

; END create_myNode

; Functie pentru convertirea numerelor din sir de caractere in int.
; START iocla_atoi

iocla_atoi:
    push ebp
    mov ebp, esp
    push ebx
    push esi

    ; Eliberam fiecare registru folosit.
    xor esi, esi
    xor ecx, ecx
    xor eax, eax
    xor edx, edx
    xor ebx, ebx

    mov esi, dword [ebp + 8] ; in esi se va afla numarul ca string

loopplus:
    mov bl, [esi + ecx]
    cmp bl, 0
    je out
    cmp bl, '-' ; este mai mult pentru primul caracter, in cazul in care e negativ numarul
    je loopminus
    sub bl, 0x30 ; il transformam in numar din caracter
    mul dword [inmul] ; se inmulteste cu 10 numarul si se aduna ce am obtinut dupa transformare.
    add eax, ebx
    inc ecx
    jmp loopplus

loopminus:
    inc ecx
    mov bl, [esi + ecx]
    cmp bl, 0
    je out
    sub bl, 0x30
    mul dword [inmul] ; se inmulteste cu 10 numarul si se scade ce am obtinut dupa transformare.
    sub eax, ebx
    jmp loopminus

out:
    pop esi
    pop ebx
    leave
    ret

; END iocla_atoi


; Functia create_tree care construieste arborele.
; START create_tree

create_tree:
    push ebp
    mov ebp, esp
    xor eax, eax
    push ebx
    push esi
    push edi
    ; Eliberam fiecare registru folosit.
    xor esi, esi
    xor edi, edi
    xor ecx, ecx
    
    mov ebx, dword [ebp + 8] ; in ebx este retinut sirul prima data.
    
    ; Un loop pentru a calcula numarul de elemente.
loopEl:
    mov al, byte [ebx + ecx]
    cmp al, 0
    je algoritm
    cmp al, ' ' ; este calculat in functie de spatiu.
    je nrEl
    jmp outEl
nrEl:
    inc esi
outEl:
    inc ecx
    jmp loopEl

algoritm:
    imul esi, 4 ; alocam cate 4 bytes pentru fiecare element.
    mov edi, esi
    call malloc ; alocam memoria pentru vectorul in care tinem minte elementele.
    mov [myVect], eax
    xor esi, esi

    push delim
    push ebx
    call strtok
    add esp, 8
    mov [myVect + 4 * esi], eax ; primul element din sir.
    
    inc esi

loop:
    push delim
    push 0x00
    call strtok
    add esp, 8
    cmp eax, 0
    je exittok ; iesim din loop daca ajunge la finalul sirului.
    mov [myVect + 4 * esi], eax ; stocam fiecare element in vector.
    inc esi
    jmp loop

exittok:
    dec esi ; ajunge sa fie la pozitia corespunzatoare finalului sirului.
    
    ; Pentru a parcurege sirul si a crea arborele am parcurs invers vectorul.
    ; de elemente.
loopfinal:
    mov eax, [myVect + 4 * esi]
    push eax
    call whatOP ; verificam daca e operand sau operator.
    add esp, 4
    mov dword [op], eax
    cmp dword [op], 1
    je operandNod
    jne operatieNod

operandNod:
    mov eax, [myVect + 4 * esi] ; daca e operand, este creat nodul
    push eax
    call create_myNode
    add esp, 4
    push eax ; este mutat pe stiva.
    jmp final    
    
operatieNod:
    mov eax, [myVect + 4 * esi] ; daca e operator, este creat nodul.
    push eax
    call create_myNode
    add esp, 4
    pop ebx
    mov [eax + 4], ebx ; mutam la adresa copilului din stanga, nodul din varful stivei.
    pop ebx
    mov [eax + 8], ebx ; mutam la adresa copilului din dreapta, al doilea nod din varful stivei.
    push eax ; este mutat pe stiva
    
final:
    dec esi
    cmp esi, 0
    jge loopfinal

    pop eax ; ultimul element ramas pe stiva, important pentru algoritm este radacina.

    pop edi
    pop esi
    pop ebx
    leave
    ret

; END create_tree