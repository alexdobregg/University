#lang racket
(require racket/match)

(provide (all-defined-out))

(define ITEMS 5)

; Dobre Gigi-Alexandru 322CDb

;; C1, C2, C3, C4 sunt case într-un supermarket.
;; C1 acceptă doar clienți care au cumparat maxim ITEMS produse (ITEMS este definit mai sus).
;; C2 - C4 nu au restricții.
;; Considerăm că procesarea fiecărui produs la casă durează un minut.
;; Casele pot suferi întarzieri (delay).
;; La un moment dat, la fiecare casă există 0 sau mai mulți clienți care stau la coadă.
;; Timpul total (tt) al unei case reprezintă timpul de procesare al celor aflați la coadă,
;; adică numărul de produse cumpărate de ei + întârzierile suferite de casa respectivă (dacă există).
;; Ex:
;; la C3 sunt Ana cu 3 produse și Geo cu 7 produse,
;; și C3 nu are întârzieri => tt pentru C3 este 10.


; Definim o structură care descrie o casă prin:
; - index (de la 1 la 4)
; - tt (timpul total descris mai sus)
; - queue (coada cu persoanele care așteaptă)
(define-struct counter (index tt queue) #:transparent)


; Implementați o functie care intoarce o structură counter goală.
; tt este 0 si coada este vidă.
; Obs: la definirea structurii counter se creează automat o funcție make-counter pentru a construi date de acest tip
(define (empty-counter index)
  (make-counter index 0 '()))


; Implementați o funcție care crește tt-ul unei case cu un număr dat de minute.
(define (tt+ C minutes)
  (struct-copy counter C [tt (+ minutes (match C [(counter _ tt _) tt]))]))


; Implementați o funcție care primește o listă de case și intoarce o pereche dintre:
; - indexul casei (din listă) care are cel mai mic tt
; - tt-ul acesteia
; Obs: când mai multe case au același tt, este preferată casa cu indexul cel mai mic
(define (min-tt counters)
  (find-min-tt (car counters) (cdr counters)))

(define (find-min-tt C counters)
  (cond
    ((null? counters) (cons (match C [(counter index _ _) index])
                            (match C [(counter _ tt _) tt])))
    ((<= (match C [(counter _ tt _) tt]) (match (car counters) [(counter _ tt _) tt]))
     (find-min-tt C (cdr counters)))
    (else (find-min-tt (car counters) (cdr counters)))))


; Implementați o funcție care adaugă o persoană la o casă.
; C = casa, name = numele persoanei, n-items = numărul de produse cumpărate
; Veți întoarce o nouă structură obținută prin așezarea perechii (name . n-items)
; la sfârșitul cozii de așteptare.
(define (add-to-counter C name n-items)
  (struct-copy counter C [tt (+ (match C [(counter _ tt _) tt]) n-items)]
               [queue (append (match C [(counter _ _ queue) queue])
                              (list (cons name n-items)))]))


; Implementați funcția care simulează fluxul clienților pe la case.
; requests = listă de cereri care pot fi de 2 tipuri:
; - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă
; - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute
; C1, C2, C3, C4 = structuri corespunzătoare celor 4 case
; Sistemul trebuie să proceseze aceste cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele cu tt minim (dintre casele la care au voie)
; - când o casă suferă o întârziere, tt-ul ei crește
(define (serve requests C1 C2 C3 C4)
  (if (null? requests)
      (list C1 C2 C3 C4)
      (match (car requests)
        [(list 'delay index minutes) (delay requests C1 C2 C3 C4 index minutes)]
        [(list name n-items)         (add-queue requests C1 C2 C3 C4 name n-items)])))


(define (delay requests C1 C2 C3 C4 index minutes)
  (cond
    ((= index (match C1 [(counter index _ _) index]))
     (serve (cdr requests) (tt+ C1 minutes) C2 C3 C4))
    ((= index (match C2 [(counter index _ _) index]))
     (serve (cdr requests) C1 (tt+ C2 minutes) C3 C4))
    ((= index (match C3 [(counter index _ _) index]))
     (serve (cdr requests) C1 C2 (tt+ C3 minutes) C4))
    ((= index (match C4 [(counter index _ _) index]))
     (serve (cdr requests) C1 C2 C3 (tt+ C4 minutes)))))

(define (add-queue requests C1 C2 C3 C4 name n-items)
  (if (<= n-items ITEMS)
      (cond
        ((= (car (min-tt (list C1 C2 C3 C4))) (match C1 [(counter index _ _) index]))
         (serve (cdr requests) (add-to-counter C1 name n-items) C2 C3 C4))
        ((= (car (min-tt (list C1 C2 C3 C4))) (match C2 [(counter index _ _) index]))
         (serve (cdr requests) C1 (add-to-counter C2 name n-items) C3 C4))
        ((= (car (min-tt (list C1 C2 C3 C4))) (match C3 [(counter index _ _) index]))
         (serve (cdr requests) C1 C2 (add-to-counter C3 name n-items) C4))
        ((= (car (min-tt (list C1 C2 C3 C4))) (match C4 [(counter index _ _) index]))
         (serve (cdr requests) C1 C2 C3 (add-to-counter C4 name n-items))))
      (cond
        ((= (car (min-tt (list C2 C3 C4))) (match C2 [(counter index _ _) index]))
         (serve (cdr requests) C1 (add-to-counter C2 name n-items) C3 C4))
        ((= (car (min-tt (list C2 C3 C4))) (match C3 [(counter index _ _) index]))
         (serve (cdr requests) C1 C2 (add-to-counter C3 name n-items) C4))
        ((= (car (min-tt (list C2 C3 C4))) (match C4 [(counter index _ _) index]))
         (serve (cdr requests) C1 C2 C3 (add-to-counter C4 name n-items))))))

