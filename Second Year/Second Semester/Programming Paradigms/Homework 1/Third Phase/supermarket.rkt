#lang racket
(require racket/match)
(require "queue.rkt")

(provide (all-defined-out))

(define ITEMS 5)

;; ATENȚIE: Pentru această etapă a temei este necesar să implementați
;;          întâi TDA-ul queue în fișierul queue.rkt.
;; Reveniți la sarcinile din acest fișier după ce ați implementat tipul 
;; queue și ați verificat implementarea folosind checker-ul.


; Structura counter nu se modifică.
; Ceea ce se modifică este implementarea câmpului queue:
; - în loc de listă, acesta va fi o coadă (o structură de tip queue)
; - acest lucru nu este vizibil în definiția structurii counter,
;   ci în implementarea operațiilor acestui tip de date (tipul counter)
(define-struct counter (index tt et queue) #:transparent)


; TODO
; Actualizați funcțiile de mai jos astfel încât ele să folosească
; o structură de tip queue pentru reprezentarea cozii de persoane.
; Elementele cozii continuă să fie perechi (nume . nr_produse).
; Este esențial să respectați "bariera de abstractizare", adică să
; operați cu coada doar folosind interfața acestui TDA:
; - empty-queue
; - queue-empty?
; - enqueue
; - dequeue
; - top
; Obs: Doar câteva funcții vor necesita actualizări.
(define (empty-counter index)           ; testată de checker
  (make-counter index 0 0 empty-queue))

(define (update f counters index)
  (if (exist? counters index)
      (if (equal? index (counter-index (car counters)))
          (cons (f (car counters)) (cdr counters))
          (cons (car counters) (update f (cdr counters) index)))
      counters))

; Functia exist? verifica daca o casa cu un anume index face parte dintr-o lista de case.
(define (exist? counters index)
  (if (null? counters)
      #f
      (if (equal? index (counter-index (car counters)))
          #t
          (exist? (cdr counters) index))))

(define (tt+ minutes)
  (λ (C)
    (struct-copy counter C [tt (+ minutes (counter-tt C))])))

(define (et+ minutes)
  (λ (C)
    (struct-copy counter C [et (+ minutes (counter-et C))])))

(define (add-to-counter name items)     ; testată de checker
  (λ (C)                               ; nu modificați felul în care funcția își primește argumentele
    (if (and (null? (queue-left (counter-queue C))) (null? (queue-right (counter-queue C))))
        (struct-copy counter ((et+ items) ((tt+ items) C)) [queue (enqueue (cons name items) (counter-queue C))])
        (struct-copy counter ((tt+ items) C) [queue (enqueue (cons name items) (counter-queue C))]))))

(define (min-ct proced-type)
  (λ (counters)
    (find-min-ct (car counters) (cdr counters) proced-type)))

(define (find-min-ct C counters proced-type)
  (cond
    ((null? counters) (cons (counter-index C) (proced-type C)))
    ((<= (proced-type C) (proced-type (car counters))) (find-min-ct C (cdr counters) proced-type))
    (else (find-min-ct (car counters) (cdr counters) proced-type))))


(define min-tt (min-ct counter-tt)) ; folosind funcția de mai sus
(define min-et (min-ct counter-et)) ; folosind funcția de mai sus

(define remove-first-from-counter   ; testată de checker
  (λ (C)
    (define queueAR (dequeue (counter-queue C)))
    (cond
      ((queue-empty? queueAR) (struct-copy counter C [et 0] [tt 0] [queue queueAR]))
      ((null? (queue-left queueAR)) (struct-copy counter C [tt (nrIt-list (queue-right queueAR) 0)] [et (cdr (top queueAR))] [queue queueAR]))
      (else (struct-copy counter C [tt (nrIt-list (queue-left queueAR) 0)] [et (cdr (top queueAR))] [queue queueAR])))))

(define (nrIt-list list acc)
  (if (null? list)
      acc
      (nrIt-list (cdr list) (+ acc (cdr (car list))))))


; TODO
; Implementați o funcție care calculează starea unei case după un număr dat de minute.
; Funcția presupune, fără să verifice, că în acest timp nu a ieșit nimeni din coadă, 
; deci va avea efect doar asupra câmpurilor tt și et.
; (cu alte cuvinte, este responsabilitatea utilizatorului să nu apeleze această funcție
; cu minutes > timpul până la ieșirea primului client din coadă)
; Atenție: casele fără clienți nu trebuie să ajungă la timpi negativi!
(define (pass-time-through-counter minutes)
  (λ (C)
    (cond
      ((and (< minutes (counter-et C)) (< minutes (counter-tt C))) ((et+ (- 0 minutes)) ((tt+ (- 0 minutes)) C)))
      ((and (>= minutes (counter-et C)) (< minutes (counter-tt C))) ((et+ (- 0 (counter-et C))) ((tt+ (- 0 minutes)) C)))
      ((and (< minutes (counter-et C)) (>= minutes (counter-tt C))) ((et+ (- 0 minutes)) ((tt+ (- 0 (counter-tt C))) C)))
      ((and (>= minutes (counter-et C)) (>= minutes (counter-tt C))) ((et+ (- 0 (counter-et C))) ((tt+ (- 0 (counter-tt C))) C))))))
  

; TODO
; Implementați funcția care simulează fluxul clienților pe la case.
; ATENȚIE: Față de etapa 2, apar modificări în:
; - formatul listei de cereri (parametrul requests)
; - formatul rezultatului funcției (explicat mai jos)
; requests conține 4 tipuri de cereri (3 moștenite din etapa 2 plus una nouă):
;   - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă            (ca înainte)
;   - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute       (ca înainte)
;   - (ensure <average>) - cât timp tt-ul mediu al tuturor caselor este mai mare decât
;                          <average>, se adaugă case fără restricții (case slow)         (ca înainte)
;   - <x> - trec <x> minute de la ultima cerere, iar starea caselor se actualizează
;           corespunzător (cu efect asupra câmpurilor tt, et, queue)                     (   NOU!   )
; Obs: Au dispărut cererile de tip remove-first, pentru a lăsa loc unui mecanism mai 
; sofisticat de a scoate clienții din coadă (pe măsură ce trece timpul).
; Sistemul trebuie să proceseze cele 4 tipuri de cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele cu tt minim (dintre casele la care au voie)  (ca înainte)
; - când o casă suferă o întârziere, tt-ul și et-ul ei cresc (chiar dacă nu are clienți) (ca înainte)
; - tt-ul mediu (ttmed) se calculează pentru toate casele (și cele fast, și cele slow), 
;   iar la nevoie veți adăuga case slow una câte una, până când ttmed <= <average>       (ca înainte)
; - când timpul prin sistem avansează cu <x> minute, tt-ul, et-ul și queue-ul tuturor 
;   caselor se actualizează pentru a reflecta trecerea timpului; dacă unul sau mai mulți 
;   clienți termină de stat la coadă, ieșirile lor sunt contorizate în ordine cronologică.
; Rezultatul funcției serve va fi o pereche cu punct între:
; - lista sortată cronologic a clienților care au părăsit supermarketul
;   - fiecare element din listă va avea forma (index_casă . nume)
;   - dacă mai mulți clienți ies simultan, sortați-i crescător după indexul casei
; - lista caselor în starea finală (ca rezultatul din etapele 1 și 2)
; Obs: Pentru a contoriza ieșirile din cozi, puteți să lucrați într-o funcție ajutătoare
; (cu un parametru în plus față de funcția serve), pe care serve doar o apelează.
(define (serve requests fast-counters slow-counters)
  (serve-helper requests fast-counters slow-counters '()))

(define (serve-helper requests fast-counters slow-counters acc)
  (if (null? requests)
      (cons acc (append fast-counters slow-counters))
      (match (car requests)
        [(list 'delay index minutes) (delay requests fast-counters slow-counters index minutes acc)]
        [(list 'ensure average) (ensure-average requests fast-counters slow-counters average acc)]
        [(list name n-items) (add-to-queue requests fast-counters slow-counters name n-items acc)]
        [minutes (pass-time requests fast-counters slow-counters minutes acc)])))

; O structura care tine minte index-ul casei, tt-ul si et-ul de la momentul iesirii unei persoane.
(define-struct first-out (index tt et name) #:transparent)
; O structura in care se va salva persoana iesita si cum va fi modificat counterul dupa iesirea ei.
(define-struct after-out (first-out counter) #:transparent)

; Functia empty-first-out ne creeaza o noua structura goala, unde name este setat la invalid.
(define empty-first-out
  (make-first-out 0 0 0 'invalid))
  
; Functia pass-time rezolva requestul de tip <x> (minutes).
(define (pass-time requests fast-counters slow-counters mins acc)
  
  ; Functia pass-time1 ne returneaza o lista cu un element de tipul structurii after-out cu o persoana eliminata.
  (define pass-time1
    (λ (C)
      (list (make-after-out (make-first-out (counter-index C) (counter-tt C) (counter-et C) (car (top (counter-queue C)))) (remove-first-from-counter C)))))
  
  ; Functia pass-time2 ne returneaza o lista cu un element de tipul structurii after-out, fara nicio persoana eliminata si
  ; doar cu aux_min minute trecute pentru counter.
  (define (pass-time2 aux_min)
    (λ (C)
      (list (make-after-out empty-first-out ((pass-time-through-counter aux_min) C)))))
  
  ; Functia pass-time3 ne returneaza o lista cu un element de tipul structurii after-out, fara nicio persoana eliminata si
  ; cu nicio modificare asupra counterului, deoarece nu are persoane la coada.
  (define pass-time3
    (λ (C)
      (list (make-after-out empty-first-out C))))

  ; pass-time-helper va acumula in afterO 
  (define (pass-time-helper min_aux counters afterO)
    (cond
      ; Daca nu mai sunt counters, se returneaza o lista de elemente after-out.
      ((null? counters) afterO)
      ; Daca trec mai multe minute decat et-ul unui counter si counterul nu avea coada goala
      ; (caci poate avea delay dar sa nu aiba persoane la coada), atunci vom elimina prima persoana,
      ; si vom continua pe aceleasi case, doar ca prima persoana de la casa curenta este eliminat.
      ((and (<= (counter-et (car counters)) min_aux) (not (queue-empty? (counter-queue (car counters)))))
       (pass-time-helper (- min_aux (counter-et (car counters)))
                         (update remove-first-from-counter counters (counter-index (car counters)))
                         (append afterO (pass-time1 (car counters)))))
      ; Daca et-ul counter-ului este mai mare decat minutele primite sau min-aux este egal cu mins, atunci
      ; doar va trece timpul in casa curenta si va continua pe toate casele mai putin pe prima.
      ((or (> (counter-et (car counters)) min_aux) (= min_aux mins)) (pass-time-helper mins (cdr counters) (append afterO ((pass-time2 min_aux) (car counters)))))
      ; Daca counter-et este 0, inseamna ca nu este nimeni la coada, deci va continua pe toate casele mai putin pe prima.
      ((zero? (counter-et (car counters))) (pass-time-helper mins (cdr counters) (append afterO (pass-time3 (car counters)))))))

  ; Functie cu ajutorul careia obtinem casele dupa ce am facut modificarile aferente trecerii timpului,
  ; adica atunci cand vom avea invalid la nume in first-out in contextul aceleiasi case, asa va
  ; arata casa dupa modificari.
  (define (get-counters afterO)
    (foldl (λ (x acc) (if (equal? (first-out-name (after-out-first-out x)) 'invalid)
                          (append acc (list (after-out-counter x)))
                          acc)) '() afterO))

  ; Functie cu ajutorul careia obtinem lista de persoane care au iesit (nume diferit de invalid), sortate
  ; dupa criteriile necesare.
  (define (get-list afterO)
    (define listOut (foldl (λ (x acc) (if (not (equal? (first-out-name (after-out-first-out x)) 'invalid))
                                       (append acc (list (after-out-first-out x)))
                                       acc)) '() afterO))
    (foldl (λ (x acc) (append acc (list (cons (first-out-index x) (first-out-name x))))) '() (sort-ins-et listOut)))

  (define fastO (pass-time-helper mins fast-counters '()))
  (define slowO (pass-time-helper mins slow-counters '()))
  
  (serve-helper (cdr requests) (get-counters fastO) (get-counters slowO) (append acc (get-list (append fastO slowO))))) 

; sort-ins-et si insert sunt functii care ajuta la sortarea listei cu oameni care au iesit de la casa.
(define sort-ins-et
  (λ (listOut)
    (if (null? listOut)
        listOut
        (insert (car listOut) (sort-ins-et (cdr listOut))))))

(define (insert x L)
  (if (or (null? L)
          (and (> (first-out-tt x) (first-out-tt (car L)))
               (= (first-out-index x) (first-out-index (car L))))
          (and  (< (first-out-et x) (first-out-et (car L)))
                (not (= (first-out-index x) (first-out-index (car L)))))
          (and (< (first-out-index x) (first-out-index (car L)))
               (= (first-out-et x)(first-out-et (car L)))))
      (cons x L)
      (cons (car L) (insert x (cdr L)))))

; Daca indexul casei este regasit in fast-counters atunci se face update cu et+ si tt+ casei din
; fast-counters, altfel se face casei din slow-counters.
(define (delay requests fast-counters slow-counters index minutes acc)
  (if (exist? fast-counters index)
      (serve-helper (cdr requests) (update (et+ minutes) (update (tt+ minutes) fast-counters index) index)
                    slow-counters acc)
      (serve-helper (cdr requests) fast-counters
                    (update (et+ minutes) (update (tt+ minutes) slow-counters index) index) acc)))

; Daca persoana care trebuie pusa la coada are mai putine produse decat ITEMS si exista in fast
; counters, se adauga la casa cu tt minim din fast-counters, altfel, daca una dintre cele doua
; conditii este falsa, se adauga la casa cu tt minim din slow-counters.
(define (add-to-queue requests fast-counters slow-counters name n-items acc)
  (if (and (<= n-items ITEMS)
           (exist? fast-counters (car (min-tt (append fast-counters slow-counters)))))
      (serve-helper (cdr requests)
                    (update (add-to-counter name n-items) fast-counters (car (min-tt fast-counters)))
                    slow-counters acc)
      (serve-helper (cdr requests) fast-counters
                    (update (add-to-counter name n-items) slow-counters (car (min-tt slow-counters))) acc)))

; Se calculeaza media tt-urilor si daca este mai mica decat average, nu se intampla nimic, iar daca
; este mai mare, se adauga o noua casa caselor din slow-counters.
(define (ensure-average requests fast-counters slow-counters average acc)
  (if (<= (/ (compute-sum-tt (append fast-counters slow-counters) 0)
             (length (append fast-counters slow-counters))) average)
      (serve-helper (cdr requests) fast-counters slow-counters acc)
      (ensure-average requests fast-counters
                      (append slow-counters
                              (list (empty-counter (+ 1 (last-index slow-counters))))) average acc)))

; Functia last-index gaseste indexul ultimei case existente.                                                
(define (last-index counters)
  (if (null? (cdr counters))
      (counter-index (car counters))
      (last-index (cdr counters))))


; Functia compute-sum-tt calculeaza suma tt-urilor tuturor caselor.
(define (compute-sum-tt counters acc)
  (if (null? counters)
      acc
      (compute-sum-tt (cdr counters) (+ (counter-tt (car counters)) acc))))