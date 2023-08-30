function [A, b] = generate_probabilities_system(rows)
  % Initializam matricea la prima posibilitate a ei si in functie de rows
  % adaugam atat linii cat si coloane.
  A = [4 -1 -1 0 0 0; -1 5 -1 -1 -1 0; -1 -1 5 0 -1 -1;
       0 -1 0 4 -1 0; 0 -1 -1 -1 5 -1; 0 0 -1 0 -1 4];
  nrmax = 6;
  oL = 4;
  oR = 6;
  % Obtinem numarul de noduri si il calculam pe b.
  nrNoduri = rows * (rows + 1) / 2;
  b = zeros(nrNoduri, 1);
  b(nrNoduri - rows + 1 : nrNoduri, 1) = 1;
  i = 4;
  while i <= rows
    nrmax += i;
    nL = oR + 1;
    nR = nrmax;
    iterator = nL;
    % Iteram prin pozitiile care trebuie completate si punem valorile corespunzatoare.
    j = oL;
    while j <= oR
      A(j, j) += 1;
      A(j, iterator : iterator + 1) = A(iterator : iterator + 1, j) = -1;
      iterator += 1;
      j += 1;
    endwhile
    k = nL + 1;
    while k <= nR - 1
      A(k, k + 1) = A(k + 1, k) = -1;
      A(k, k) = 5;
      k += 1;
    endwhile
    A(nL, nL) = A(nR, nR) = 4;
    A(nL, nL + 1) = A(nL + 1, nL) = -1;
    oL = nL;
    oR = nR;
    i += 1;
  endwhile
endfunction