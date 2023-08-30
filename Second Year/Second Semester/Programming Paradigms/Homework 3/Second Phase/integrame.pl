:- ensure_loaded('checker.pl').

%test_mode(detailed).

% Considerăm următoarele reprezentări:
%
% O integramă este reprezentată prin structura (compusul)
% integ(H, W, Lista, Vocab), unde:
% H este înălțimea integramei
% W este lățimea integramei
% Lista este o listă de tupluri (Poz, Valoare), unde
%   Poz este un tuplu (R, C) conținând rândul și coloana (0-based)
%   Valoare este una dintre:
%     x - dacă celula este neagră (nu poate fi completată cu litere)
%     o literă, dacă celula este completată cu o literă
%     o listă de întrebări, reprezentate ca tupluri (Text, Dir, ID), cu
%       Text - un srting, textul întrebării
%       Dir - una dintre valorile j sau d, indicând direcția întrebării
%       ID - un identificator numeric al întrebării
% Vocab este o listă de stringuri reprezentând cuvinte disponibile
% pentru a rezolva întrebarea.
%
% În ieșirea predicatului intrebări, o întrebare este reprezentată ca
% ((R, C), Text, Dir, ID), unde
% R este rândul căsuței cu întrebarea (0-based)
% C este coloana căsuței cu întrebarea (0-based)
% Text este textul întrebării (un string)
% Dir este j sau d, reprezentând direcția în care trebuie să fie plasat
% răspunsul (jos sau dreapta)
% ID este un identificator numeric al întrebării.

% Puteți vizualiza integramele cu:
% integrama(0, W), print_integrama(W).
% integrama(1, W), print_integrama(W).
% integrama(2, W), print_integrama(W).
% integrama(3, W), print_integrama(W).
%
% Testați cu
% vmtest.
% Testați teste individuale (vedeți predicatul tt din checker.pl) cu
% vmtest(Test).
% de exemplu cu vmtest(intrebari).


% intrebari/2
% intrebari(integ(+H, +W, +Lista, +Vocab), -Lista_intrebari)
% Este adevărat atunci când Lista_intrebari este o lista de tupluri
% ((R, C), Text, Dir, ID), fiecare tuplu corespunzând unei întrebări din
% integramă (rândul, coloana, textul întrebării, direcția (j/d),
% identificatorul).
% BONUS: intrebari are o singură soluție (o singură listă) pentru o
% anumită integramă.
intrebari(integ(_, _, [], _), []).
intrebari(integ(_, _, [(_, []) | Lista], _), Lista_intrebari) :- intrebari(integ(_, _, Lista, _), Lista_intrebari).
intrebari(integ(_, _, [(Poz, [(Text, Dir, ID) | Quest]) | Lista], _), [(Poz, Text, Dir, ID) | Lista_intrebari]) :-
    intrebari(integ(_, _, [(Poz, Quest) | Lista], _), Lista_intrebari), !.
intrebari(integ(_, _, [_ | Lista], _), Lista_intrebari) :- intrebari(integ(_, _, Lista, _), Lista_intrebari).

% id_intrebare/2
% id_intrebare(+Integ, ?Intrebare, ?Q_ID)
% Este adevărat dacă în integrama reprezentată ca integ(...), Intrebare
% este un text iar Q_ID este un identificator care corespund aceleași
% întrebări.
idInt([(_, Intrebare, _, Q_ID) | _], Intrebare, Q_ID).
idInt([(_, _, _, _) | Lista_intrebari], Intrebare, X) :- idInt(Lista_intrebari, Intrebare, X).

intID([(_, Intrebare, _, Q_ID) | _], Q_ID, Intrebare).
intID([(_, _, _, _) | Lista_intrebari], Q_ID, X) :- intID(Lista_intrebari, Q_ID, X).

id_intrebare(Integul, Intrebare, Q_ID) :- var(Q_ID), intrebari(Integul, Lista_intrebari), idInt(Lista_intrebari, Intrebare, Q_ID).
id_intrebare(Integul, Intrebare, Q_ID) :- var(Intrebare), intrebari(Integul, Lista_intrebari), intID(Lista_intrebari, Q_ID, Intrebare).

% completare/3
% completare(+Integ, +Sol, -Integrama)
% Predicatul produce Integrama, o structură de forma integ(...),
% pornind de la Integ, în care au fost completate celule conform cu
% soluția Sol.
% Soluția este reprezentată ca o listă de perechi (Întrebare, Răspuns),
% unde Întrebarea este textul unei întrebări, iar Răspuns este un cuvând
% de completat; ambele sunt stringuri.
% De exemplu, o soluție parțială pentru integrama 0 poate fi:
% [('Din care plouă', 'NOR'), ('Al doilea număr', 'DOI')]
% BONUS: lungime_spatiu are o singură soluție pentru o anumită
% întrebare.
% Puteți testa manual predicatul cu o interogare de forma:
% integrama(0, W), solutie(0, Sol), completare(W, Sol, W2),
%   print_integrama(W2).

%remove_dups(+List, -NewList): % New List isbound to List, but with duplicate items removed. remove_dups([], []).
remove_dups([], []).
remove_dups([First | Rest], NewRest) :- member(First, Rest), remove_dups(Rest, NewRest).
remove_dups([First | Rest], [First | NewRest]) :- not(member(First, Rest)), remove_dups(Rest, NewRest).

completareHelper(_, _, [], []).
completareHelper((R, C), j, [Lit | Litere], [((R1, C), Lit) | List]) :- R1 is R + 1,
    completareHelper((R1, C), j, Litere, List).
completareHelper((R, C), d, [Lit | Litere], [((R, C1), Lit) | List]) :- C1 is C + 1,
    completareHelper((R, C1), d, Litere, List).

completare(Integul, [], Integul).
completare(integ(H, W, Lista, Vocab), [(Intrebare, Raspuns) | Rasp], integ(H, W, Cp, Vocab)) :-
    intrebari(integ(H, W, Lista, Vocab), Lista_intrebari), member(((R, C), Intrebare, Dir, _), Lista_intrebari),
    atom_chars(Raspuns, Litere), completareHelper((R, C), Dir, Litere, Lista2),
    append(Lista, Lista2, Lista3), remove_dups(Lista3, Lista4),
    completare(integ(H, W, Lista4, Vocab), Rasp, integ(H, W, Cp, Vocab)).

% lungime_spatiu/3
% lungime_spatiu(integ(+H, +W, +Lista, +Vocab), +Intrebare, -Lungime)
% Returnează lungimea spațiului asociat întrebării date.
% Întrebarea este indicată prin textul ei. De exemplu:
% lungime_spatiu pentru integrama 0 și întrebarea 'Al doilea număr'
% trebuie să lege Lungime la 3.
% BONUS: lungime_spatiu are o singură soluție pentru o anumită
% întrebare.
% Puteți testa manual predicatul cu o interogare de forma:
% integrama(0, W), id_intrebare(W, Text, 3), lungime_spatiu(W, Text, X).

lungime_helper(integ(H, _, _, _), (H, _), j, _, 0).
lungime_helper(integ(_, W, _, _), (_, W), d, _, 0).

lungime_helper(integ(_, _, Lista, _), (R, C), j, _, 0) :-
    R1 is R + 1, member(((R1, C), x), Lista).
lungime_helper(_, (R, C), j, Lista_intrebari, 0) :-
    R1 is R + 1, member(((R1, C), _, _, _), Lista_intrebari).

lungime_helper(integ(_, _, Lista, _), (R, C), d, _, 0) :-
    C1 is C + 1, member(((R, C1), x), Lista).
lungime_helper(_, (R, C), d, Lista_intrebari, 0) :-
    C1 is C + 1, member(((R, C1), _, _, _), Lista_intrebari).

lungime_helper(Integul, (R, C), j, Lista_intrebari, Lungime2) :-
    R1 is R + 1,
    lungime_helper(Integul, (R1, C), j, Lista_intrebari, Lungime), Lungime2 is Lungime + 1, !.

lungime_helper(Integul, (R, C), d, Lista_intrebari, Lungime2) :-
    C1 is C + 1,
    lungime_helper(Integul, (R, C1), d, Lista_intrebari, Lungime), Lungime2 is Lungime + 1, !.


lungime_spatiu(Integul, Intrebare, Lungime) :-
    intrebari(Integul, Lista_intrebari), member(((R, C), Intrebare, Dir, _), Lista_intrebari),
    lungime_helper(Integul, (R, C), Dir, Lista_intrebari, Lungime).

% intersectie/5
% intersectie(integ(+H, +W, +Lista, +Voc), +I1, -Poz1, +I2, -Poz2)
% Pentru o integramă și două întrebări date prin textul lor (I1 și I2),
% al căror răspunsuri se intersectează, întoarce în Poz1 indicele din
% răspunsul la I1 la care este intersecția, și în Poz2 indicele din
% răspunsul la I2 la care este intersecția. Indecșii incep de la 0.
%
% De exemplu, în integrama 0:
%  █       █       2↓      3↓      █
%  █       0↓,1→   -       -       █
%  4→      -       -       -       █
%  5→      -       -       -       █
%  █       █       █       █       █
%
%  Întrebările 'Primii 3 din artă' și 'Afirmativ' (3, respectiv 1) se
%  intersectează la pozițiile 0, respectiv 2 (va fi litera A, de la
%  ART, respectiv DA).

intersectieHelper(integ(H, W, Lista, Vocab), Intrebare1, (R1, C1), j, Intrebare2, (R2, C2), d, Pos1, Pos2) :-
    lungime_spatiu(integ(H, W, Lista, Vocab), Intrebare1, Lung1), Lung1P is Lung1 + R1, Lung1P >= R2,
    lungime_spatiu(integ(H, W, Lista, Vocab), Intrebare2, Lung2), Lung2P is Lung2 + C2, Lung2P >= C1,
    Pos1 is (R2 - R1) - 1, Pos1 >= 0,
    Pos2 is (C1 - C2) - 1, Pos2 >= 0.

intersectieHelper(integ(H, W, Lista, Vocab), Intrebare1, (R1, C1), d, Intrebare2, (R2, C2), j, Pos1, Pos2) :-
    lungime_spatiu(integ(H, W, Lista, Vocab), Intrebare1, Lung1), Lung1P is Lung1 + C1, Lung1P >= C2,
    lungime_spatiu(integ(H, W, Lista, Vocab), Intrebare2, Lung2), Lung2P is Lung2 + R2, Lung2P >= R1,
    Pos1 is (C2 - C1) - 1, Pos1 >= 0,
    Pos2 is (R1 - R2) - 1, Pos2 >= 0.

intersectie(integ(H, W, Lista, Vocab), Intrebare1, Pos1, Intrebare2, Pos2) :- 
    intrebari(integ(H, W, Lista, Vocab), Lista_intrebari),
    member((Poz1, Intrebare1, Dir1, _), Lista_intrebari),
    member((Poz2, Intrebare2, Dir2, _), Lista_intrebari),
    intersectieHelper(integ(H, W, Lista, Vocab), Intrebare1, Poz1, Dir1, Intrebare2, Poz2, Dir2, Pos1, Pos2).

% solutii_posibile/2
% solutii_posibile(integ(+H, +W, +Lista, +Vocabular), -Solutii)
% Formează o listă Solutii, conținând perechi de forma
% (Întrebare, Cuvinte), unde
% Întrebare este textul unei întrebări din integramă, iar Cuvinte este o
% listă de cuvinte sunt din Vocabular și au lungimea corectă pentru a fi
% răspuns la întrebare. Solutii conține câte o pereche pentru fiecare
% întrebare din integramă.
% Cuvintele sunt reprezentate ca liste de stringuri, fiecare string
% având lungime 1 (o singură literă).
% De exemplu, pentru integrama 0, Solutii conține 6 perechi, două dintre
% ele fiind:
% ('Afirmativ', [['D', 'A'], ['N', 'U']])
% ('Din care plouă',
% [['N','O','R'],['A','R','T'],['U','I','T'],['D','O','I']])

solutiiHelper2(_, [], []).

solutiiHelper2(Lung, [Rasp | Vocab], Sol) :-
    atom_chars(Rasp, List), length(List, LungRasp), Lung \= LungRasp, solutiiHelper2(Lung, Vocab, Sol).

solutiiHelper2(Lung, [Rasp | Vocab], [List | Sol]) :-
    atom_chars(Rasp, List), length(List, LungRasp), Lung == LungRasp, solutiiHelper2(Lung, Vocab, Sol).

solutiiHelper(_, [], []).
solutiiHelper(integ(H, W, Lista, Vocab), [(_, Intrebare, _, _) | Lista_intrebari], [(Intrebare, Sol) | Solutii]) :-
    lungime_spatiu(integ(H, W, Lista, Vocab), Intrebare, Lung),
    solutiiHelper2(Lung, Vocab, Sol),
    solutiiHelper(integ(H, W, Lista, Vocab), Lista_intrebari, Solutii).

solutii_posibile(integ(H, W, Lista, Vocab), Solutii) :-
    intrebari(integ(H, W, Lista, Vocab), Lista_intrebari), 
    solutiiHelper(integ(H, W, Lista, Vocab), Lista_intrebari, Solutii).

% rezolvare/2
% rezolvare(+Integ, -Solutie)
% Rezolvare produce în Solutie soluția integramei Integ. Soluția este
% reprezentată ca o listă de perechi de stringuri, fiecare pereche
% conținând textul unei întrebări și cuvântul (ca string) care este
% răspunsul la întrebare.
rezolvare(_, _) :- false.