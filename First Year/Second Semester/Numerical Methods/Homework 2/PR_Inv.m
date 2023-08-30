function B = PR_Inv(A)
  % GrammSchmidt si SST sunt implementari proprii de la laborator.
  n = length(A);
  [Q, R] = GrammSchmidt(A);
  B = zeros(n);
  I = eye(n);
  % Calculam inversa matricei conform algoritmului dat.
  for i = 1 : n
    B(:, i) = SST(R, Q' * I(:, i));
  endfor
endfunction