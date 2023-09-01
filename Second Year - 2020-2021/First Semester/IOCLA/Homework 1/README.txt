Dobre Gigi-Alexandru 322CD

	Pentru a putea rezolva tema a fost nevoie, pe langa functia write_stdout,
de functiile swap, itoa si utoa.

	swap: functie pentru a inversa 2 caractere intre ele.

	itoa: functie pentru a schimba dintr-un numar int intr-un sir de
caractere pentru baza 10, cazul %d.
https://www.techiedelight.com/implement-itoa-function-in-c/ -- Inspiratia
pentru codul de la itoa.

	utoa: functie pentru a schimba dintr-un numar unsigned int intr-un
sir de caractere in functie de baza, folosit si pentru hexa in schimbul
lui itoa.

	iocla_printf: am luat cate o variabila pentru fiecare caz, precum si
un string alocat dinamic cu o valoarea maxima N definita la inceput ca 
10000. Parcurgem format cu iterator si printam ce e diferit de % sau pana
ajungem la finalul sirului. Verificam si daca sunt 2 sau mai multe %
consecutive pentru a le printa pe toate mai putin ultimul. Dupa aceea
verificam in care dintre cazurile cerute se incadreaza si in functie
de acesta, folosim itoa sau utoa, iar pentru char si string, sunt introduse
direct in stringul alocat. Folosim write_stdout pentru fiecare caz si 
dezalocam memoria alocata.