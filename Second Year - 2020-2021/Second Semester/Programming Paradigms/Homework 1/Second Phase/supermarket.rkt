#lang racket
(require racket/match)

(provide (all-defined-out))

(define ITEMS 5)

;; Actualizăm structura counter cu informația et:
;; Exit time (et) al unei case reprezintă timpul până la ieșirea primului client de la casa respectivă,
;; adică numărul de produse de procesat pentru acest client + întârzierile suferite de casă (dacă există).
;; Ex:
;; la C3 s-au așezat întâi Ana cu 3 produse și apoi Geo cu 7 produse,
;; și C3 a fost întârziată cu 5 minute => et pentru C3 este 3 + 5 = 8 (timpul până când va ieși Ana).


; Redefinim structura counter.
(define-struct counter (index tt et queue) #:transparent)

; Actualizați implementarea empty-counter astfel încât să conțină și câmpul et.
(define (empty-counter index)
  (make-counter index 0 0 '()))


; Implementați o funcție care aplică o transformare casei cu un anumit index.
; f = funcție unară care se aplică pe un argument de tip casă, counters = listă de case,
; index = indexul casei din counters căreia îi veți aplica transformarea f
; Veți întoarce lista actualizată de case.
; Dacă nu există în counters o casă cu acest index, veți întoarce lista nemodificată.
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

; Memento: tt+ crește tt-ul unei case cu un număr dat de minute.
; Actualizați implementarea tt+ pentru:
; - a ține cont de noua reprezentare a unei case
; - a permite ca operații de tip tt+ să fie pasate ca argument funcției update în cel mai facil mod
; Obs: Pentru compatibilitatea cu primul argument al lui update, trebuie să obținem ușor din tt+ 
; o funcție unară care se aplică pe un argument de tip casă (v. funcții curry).
; Am eliminat parametrii lui tt+ din define-ul de mai jos astfel încât voi să determinați
; cum este cel mai bine ca tt+ să își primească parametrii.
; Din acest motiv checker-ul nu testează separat această funcție, dar asistentul va verifica
; faptul că ați implementat-o conform cerințelor.
(define (tt+ minutes)
  (λ (C)
    (struct-copy counter C [tt (+ minutes (counter-tt C))])))


; Implementați o funcție care crește et-ul unei case cu un număr dat de minute.
; Păstrați formatul folosit pentru tt+.
; Checker-ul nu testează separat această funcție.
(define (et+ minutes)
  (λ (C)
    (struct-copy counter C [et (+ minutes (counter-et C))])))


; Memento: add-to-counter adaugă o persoană (reprezentată prin nume și număr de produse) la o casă. 
; Actualizați implementarea add-to-counter pentru aceleași rațiuni pentru care am modificat tt+.
; Atenție la cum se modifică tt și et!
; Checker-ul nu testează separat această funcție.
(define (add-to-counter name n-items)
  (λ (C)
    (if (null? (counter-queue C))
        (struct-copy counter ((et+ n-items) ((tt+ n-items) C))
                     [queue (append (counter-queue C)
                                    (list (cons name n-items)))])
        (struct-copy counter ((tt+ n-items) C)
                     [queue (append (counter-queue C)
                                    (list (cons name n-items)))]))))


; Întrucât vom folosi atât min-tt (implementat în etapa 1) cât și min-et (funcție nouă),
; definiți-vă o funcție mai abstractă din care să derive ușor atât min-tt cât și min-et.
; Prin analogie cu min-tt, definim min-et astfel:
; min-et = funcție care primește o listă de case și intoarce o pereche dintre:
; - indexul casei (din listă) care are cel mai mic et
; - et-ul acesteia
; (când mai multe case au același et, este preferată casa cu indexul cel mai mic)
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


; TODO
; Implementați o funcție care scoate prima persoană din coada unei case.
; Funcția presupune, fără să verifice, că există minim o persoană la coada casei C.
; Veți întoarce o nouă structură obținută prin modificarea cozii de așteptare.
; Atenție la cum se modifică tt și et!
; Dacă o casă tocmai a fost părăsită de cineva, înseamnă că ea nu mai are întârzieri.
(define remove-first-from-counter
  (λ (C)
    (if (null? (cdr (counter-queue C)))
        (struct-copy counter ((et+ (- 0 (counter-et C))) ((tt+ (- 0 (counter-tt C))) C))
                     [queue (cdr (counter-queue C))])
        (struct-copy counter C [tt (nrIt-queue (cdr (counter-queue C)) 0)]
                     [et (cdr (car (cdr (counter-queue C))))]
                     [queue (cdr (counter-queue C))]))))
; O functie ce calculeaza numarul de obiecte ale oamenilor dintr-o coada.
(define (nrIt-queue queue acc)
  (if (null? queue)
      acc
      (nrIt-queue (cdr queue) (+ acc (cdr (car queue))))))
    

; TODO
; Implementați funcția care simulează fluxul clienților pe la case.
; ATENȚIE: Față de etapa 1, funcția serve operează cu următoarele modificări:
; - nu mai avem doar 4 case, ci:
;   - fast-counters (o listă de case pentru maxim ITEMS produse)
;   - slow-counters (o listă de case fără restricții)
;   (Sugestie: folosiți funcția update pentru a obține comportamentul pe liste de case)
; - requests conține 4 tipuri de cereri (cele 2 din etapa anterioară plus 2 noi):
;   - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă
;   - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute
;   - (remove-first) - cea mai avansată persoană părăsește casa la care se află
;   - (ensure <average>) - cât timp tt-ul mediu al tuturor caselor este mai mare decât
;                          <average>, se adaugă case fără restricții (case slow)
; Sistemul trebuie să proceseze cele 4 tipuri de cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele cu tt minim (dintre casele la care au voie)
;   (ca înainte, dar acum folosiți fast-counters și slow-counters)
; - când o casă suferă o întârziere, tt-ul și et-ul ei cresc (chiar dacă nu are clienți)
;   (puțin diferit față de etapa 1, și acum folosiți fast-counters și slow-counters)
; - persoana care părăsește supermarketul este persoana fruntașă la casa cu et-ul minim
;   (dintre casele care au clienți); dacă nicio casă nu are clienți, cererea e ignorată
; - tt-ul mediu (ttmed) se calculează pentru toate casele (și cele fast, și cele slow), 
;   iar la nevoie veți adăuga case slow una câte una, până când ttmed <= <average>
; Considerați că indecșii caselor încep de la 1 și sunt mereu în ordine.
; Ex:
; fast-counters conține casele 1-2, slow-counters conține casele 3-15
; => la nevoie veți adăuga întâi casa 16, apoi casa 17, etc.
; (puteți determina matematic de câte case noi este nevoie sau
;  puteți adăuga recursiv una câte una până când condiția este îndeplinită)

(define (serve requests fast-counters slow-counters)
  (if (null? requests)
      (append fast-counters slow-counters)
      (match (car requests)
        [(list 'delay index minutes) (delay requests fast-counters slow-counters index minutes)]
        [(list remove-first) (remove-firstQ requests fast-counters slow-counters)]
        [(list 'ensure average) (ensure-average requests fast-counters slow-counters average)]
        [(list name n-items) (add-to-queue requests fast-counters slow-counters name n-items)])))

; Daca indexul casei este regasit in fast-counters atunci se face update cu et+ si tt+ casei din
; fast-counters, altfel se face casei din slow-counters.
(define (delay requests fast-counters slow-counters index minutes)
  (if (exist? fast-counters index)
      (serve (cdr requests) (update (et+ minutes) (update (tt+ minutes) fast-counters index) index)
             slow-counters)
      (serve (cdr requests) fast-counters
             (update (et+ minutes) (update (tt+ minutes) slow-counters index) index))))

; Daca persoana care trebuie pusa la coada are mai putine produse decat ITEMS si exista in fast
; counters, se adauga la casa cu tt minim din fast-counters, altfel, daca una dintre cele doua
; conditii este falsa, se adauga la casa cu tt minim din slow-counters.
(define (add-to-queue requests fast-counters slow-counters name n-items)
  (if (and (<= n-items ITEMS)
           (exist? fast-counters (car (min-tt (append fast-counters slow-counters)))))
      (serve (cdr requests)
             (update (add-to-counter name n-items) fast-counters (car (min-tt fast-counters)))
             slow-counters)
      (serve (cdr requests) fast-counters
             (update (add-to-counter name n-items) slow-counters (car (min-tt slow-counters))))))

; Se calculeaza media tt-urilor si daca este mai mica decat average, nu se intampla nimic, iar daca
; este mai mare, se adauga o noua casa caselor din slow-counters.
(define (ensure-average requests fast-counters slow-counters average)
  (if (<= (/ (compute-sum-tt (append fast-counters slow-counters) 0)
             (length (append fast-counters slow-counters))) average)
      (serve (cdr requests) fast-counters slow-counters)
      (ensure-average requests fast-counters
                      (append slow-counters
                              (list (empty-counter (+ 1 (last-index slow-counters))))) average)))

; Cu ajutorul functiei find-pair cautam casa care are persoane la coada si care are tt-ul minim. Daca nu exista,
; se va trece la urmatorul request. Daca exista si se afla in fast-counters, se va scoate de la coada casei
; la care se afla, altfel se va scoate de la coada casei din slow-counters.
(define (remove-firstQ requests fast-counters slow-counters)
  (if (null? (find-pair (append fast-counters slow-counters)))
      (serve (cdr requests) fast-counters slow-counters)
      (if (exist? fast-counters (car (find-pair (append fast-counters slow-counters))))
          (serve (cdr requests)
                 (update remove-first-from-counter fast-counters
                         (car (find-pair (append fast-counters slow-counters))))
                 slow-counters)
          (serve (cdr requests)
                 fast-counters
                 (update remove-first-from-counter slow-counters
                         (car (find-pair (append fast-counters slow-counters))))))))

; Functia find-pair intoarce perechea ditre index si et al casei cu cel mai mic et dintre casele
; care au persoane la coada. Daca nu exista, intoarce lista goala.
(define (find-pair counters)
  (if (null? (filter QNnull? counters))
      '()
      (min-et (filter QNnull? counters))))

; Functia data ca parametru lui filter pentru a obtine casele care au persoane la coada.
(define (QNnull? C)
  (if (null? (counter-queue C))
      #f
      #t))

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