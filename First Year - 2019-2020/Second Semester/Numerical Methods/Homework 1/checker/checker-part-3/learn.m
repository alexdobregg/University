function [w] = learn(X, y)
  [rows, col] = size(X);
  % Adaugam o coloana de 1 la finalul lui X.
  v(1 : rows, 1) = 1;
  Xt = [X, v];
  % Obtinem Q, R cu ajutorul functiei Householder.
  [Q, R] = Householder(Xt);
  % Aplicam SST.
  w = SST(R' * R , R' * Q' * y);
endfunction