function [R1 R2] = PageRank(nume, d, eps)
  % Folosim textread pentru a obtine direct val1 si val2.
  A = textread(nume);
  val1 = A(end - 1);
  val2 = A(end);
  % Obtinem R1 prin metoda Iterative si R2 prin metoda Algebraic.
  R1 = Iterative(nume, d, eps);
  R2 = Algebraic(nume, d);
  % Obtinem numele fisierului in care trebuie sa scriem si il deschidem.
  numeOut = strcat(nume, '.out');
  out = fopen(numeOut, 'w');
  % Printam lungimea, R1, R2.
  fprintf(out, "%d\n\n", length(R1));
  fprintf(out, "%f\n", R1);
  fprintf(out, "\n");
  fprintf(out, "%f\n", R2);
  fprintf(out, "\n");
  % Obtinem vectorul PR2 ordonat crescator si ordinea in care se afla in functie
  % de pozitia initiala.
  [PR2, order] = sort(R2);
  % Le ordonam descrescator.
  PR2 = PR2(end : -1 : 1);
  order = order(end : -1 : 1);
  % Folosim functia Apartenenta pentru a obtine vectorul y.
  y = Apartenenta(PR2, val1, val2);
  for i = 1 : length(R1);
    % Printam locul obtinut, ce nod a obtinut acest loc si gradul de apartenenta
    % al acestui nod la multimea paginilor importante.
    fprintf(out, "%d ", i);
    fprintf(out, "%d ", order(i));
    fprintf(out, "%f\n", y(i));
  endfor
  fclose(out);
endfunction