function [G_J, c_J] = Jacobi_factorization(A, b)
  % Obtinem G si c corespunzatoare.
  N = diag(diag(A));
  P = N - A;
  inv_N = inv(N);
  G_J = inv_N * P;
  c_J = inv_N * b;
endfunction
