function R = Algebraic(nume, d)
  M = citire(nume);
  N = length(M);
  I = eye(N);
  % Am folosit operatorul \ pentru a calcula direct solutia sistemului. Solutia
  % aceasta este alternativa si nu va mai calcula inversa.
  % R = (I - d * M) \ (((1 - d) / N) * ones(N, 1));
  
  % Am folosit algoritmul PR_Inv pentru calcularea inversei, folosita in formula
  % pentru aflarea lui R.
  Inversa = PR_Inv(I - d * M);
  R = Inversa * ((1 - d) / N) * ones(N, 1);
endfunction