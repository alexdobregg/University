#lang racket
(require racket/match)
(require "queue.rkt")

(provide (all-defined-out))

(define ITEMS 5)


; TODO
; Aveți libertatea să vă structurați programul cum doriți (dar cu restricțiile
; de mai jos), astfel încât funcția serve să funcționeze conform specificației.

(define-struct counter (index tt et queue closed?) #:transparent)

; Restricții (impuse de checker):
; - trebuie să existe în continuare funcția (empty-counter index)
; - cozile de la case trebuie implementate folosind noul TDA queue

(define (empty-counter index)   
  (make-counter index 0 0 empty-queue 0))

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

(define (add-to-counter name items)
  (λ (C)
    (if (queue-empty? (counter-queue C))
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

(define remove-first-from-counter
  (λ (C)
    (define queueAR (dequeue (counter-queue C)))
    (cond
      ((queue-empty? queueAR) (struct-copy counter C
                                           [et 0]
                                           [tt 0]
                                           [queue queueAR]))
      (else (struct-copy counter C
                         [tt (- (counter-tt C) (counter-et C))]
                         [et (cdr (top queueAR))]
                         [queue queueAR])))))



(define (pass-time-through-counter minutes)
  (λ (C)
    (cond
      ((and (< minutes (counter-et C)) (< minutes (counter-tt C))) ((et+ (- 0 minutes)) ((tt+ (- 0 minutes)) C)))
      ((and (>= minutes (counter-et C)) (< minutes (counter-tt C))) ((et+ (- 0 (counter-et C))) ((tt+ (- 0 minutes)) C)))
      ((and (< minutes (counter-et C)) (>= minutes (counter-tt C))) ((et+ (- 0 minutes)) ((tt+ (- 0 (counter-tt C))) C)))
      ((and (>= minutes (counter-et C)) (>= minutes (counter-tt C))) ((et+ (- 0 (counter-et C))) ((tt+ (- 0 (counter-tt C))) C))))))

  
  
; TODO
; Implementați funcția care simulează fluxul clienților pe la case.
; ATENȚIE: Față de etapa 3, apare un nou tip de cerere, așadar
; requests conține 5 tipuri de cereri (cele moștenite din etapa 3 plus una nouă):
;   - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă              (ca înainte)
;   - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute         (ca înainte)
;   - (ensure <average>) - cât timp tt-ul mediu al caselor este mai mare decât
;                          <average>, se adaugă case fără restricții (case slow)           (ca înainte)
;   - <x> - trec <x> minute de la ultima cerere, iar starea caselor se actualizează
;           corespunzător (cu efect asupra câmpurilor tt, et, queue)                       (ca înainte)
;   - (close <index>) - casa index este închisă                                            (   NOU!   )
; Sistemul trebuie să proceseze cele 5 tipuri de cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele DESCHISE cu tt minim; nu se va întâmpla
;   niciodată ca o persoană să nu poată fi distribuită la nicio casă                       (mică modificare)
; - când o casă suferă o întârziere, tt-ul și et-ul ei cresc (chiar dacă nu are clienți);
;   nu aplicați vreun tratament special caselor închise                                    (ca înainte)
; - tt-ul mediu (ttmed) se calculează pentru toate casele DESCHISE, 
;   iar la nevoie veți adăuga case slow una câte una, până când ttmed <= <average>         (mică modificare)
; - când timpul prin sistem avansează cu <x> minute, tt-ul, et-ul și queue-ul tuturor 
;   caselor se actualizează pentru a reflecta trecerea timpului; dacă unul sau mai mulți 
;   clienți termină de stat la coadă, ieșirile lor sunt contorizate în ordine cronologică. (ca înainte)
; - când o casă se închide, ea nu mai primește clienți noi; clienții care erau deja acolo
;   avansează normal, până la ieșirea din supermarket                                    
; Rezultatul funcției serve va fi o pereche cu punct între:
; - lista sortată cronologic a clienților care au părăsit supermarketul:
;   - fiecare element din listă va avea forma (index_casă . nume)
;   - dacă mai mulți clienți ies simultan, sortați-i crescător după indexul casei
; - lista cozilor (de la case DESCHISE sau ÎNCHISE) care încă au clienți:
;   - fiecare element va avea forma (index_casă . coadă) (coada este de tip queue)
;   - lista este sortată după indexul casei
(define (serve requests fast-counters slow-counters)
  (serve-helper requests fast-counters slow-counters '()))

(define (serve-helper requests fast-counters slow-counters acc)
  (if (null? requests)
      (cons acc (append (counter->pair (filter notemptyQ? fast-counters) '()) (counter->pair (filter notemptyQ? slow-counters) '())))
      (match (car requests)
        [(list 'delay index minutes) (delay requests fast-counters slow-counters index minutes acc)]
        [(list 'ensure average) (ensure-average requests fast-counters slow-counters average acc)]
        [(list 'close index) (close-counter requests fast-counters slow-counters index acc)]
        [(list name n-items) (add-to-queue requests fast-counters slow-counters name n-items acc)]
        [minutes (pass-time requests fast-counters slow-counters minutes acc)])))

  
(define notemptyQ?
  (λ (C)
    (not (queue-empty? (counter-queue C)))))

(define (counter->pair counters acc)
  (if (null? counters)
      acc
      (counter->pair (cdr counters) (append acc (list (cons (counter-index (car counters)) (counter-queue (car counters))))))))

(define close
  (λ (C)
    (struct-copy counter C [closed? 1])))

(define (close-counter requests fast-counters slow-counters index acc)
  (if (exist? fast-counters index)
      (serve-helper (cdr requests) (update close fast-counters index)
                    slow-counters acc)
      (serve-helper (cdr requests) fast-counters
                    (update close slow-counters index) acc))) 

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
    (foldl (λ (x acc) (append acc (list (cons (first-out-index x) (first-out-name x))))) '()  (sort-ins-et listOut)))

  (define fastO (pass-time-helper mins fast-counters '()))
  (define slowO (pass-time-helper mins slow-counters '()))
  (serve-helper (cdr requests) (get-counters fastO) (get-counters slowO) (append acc (get-list (append slowO fastO))))) 

; sort-ins-et si insert sunt functii care ajuta la sortarea listei cu oameni care au iesit de la casa.
(define sort-ins-et
  (λ (listOut)
    (if (null? listOut)
        listOut
        (insert (car listOut) (sort-ins-et (cdr listOut))))))

(define (insert x L)
  (if (or (null? L)
          (< (first-out-et x) (first-out-et (car L)))
          (and (= (first-out-et x) (first-out-et (car L)))
               (< (first-out-index x) (first-out-index (car L))))
          (and (= (first-out-et x) (first-out-et (car L)))
               (= (first-out-index x) (first-out-index (car L)))
               (> (first-out-tt x) (first-out-tt (car L)))))
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
           (exist? fast-counters (car (min-tt (filter not-closed? (append fast-counters slow-counters))))))
      (serve-helper (cdr requests)
                    (update (add-to-counter name n-items) fast-counters (car (min-tt (filter not-closed? fast-counters))))
                    slow-counters acc)
      (serve-helper (cdr requests) fast-counters
                    (update (add-to-counter name n-items) slow-counters (car (min-tt (filter not-closed? slow-counters)))) acc)))

(define not-closed?
  (λ (C)
    (equal? (counter-closed? C) 0)))

; Se calculeaza media tt-urilor si daca este mai mica decat average, nu se intampla nimic, iar daca
; este mai mare, se adauga o noua casa caselor din slow-counters.
(define (ensure-average requests fast-counters slow-counters average acc)
  (if (<= (/ (compute-sum-tt (filter not-closed? (append fast-counters slow-counters)) 0)
             (length (filter not-closed?(append fast-counters slow-counters)))) average)
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