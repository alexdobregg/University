function [Q, R] = Householder(A)
  [m,n] = size(A);
  Q = eye(m);
  if m <= n
    aux = m - 1;
  else 
    aux = n;
  endif
  for i = 1 : aux
    H = eye(m);
    x = zeros(m, 1);
    for j = i + 1 : m
      x(j) = A(j, i);
    endfor
    sigma = sign(A(i, i)) * norm(A(i : m, i));
    x(i) = sigma + A(i, i);
    H = H - 2 * (x * x') / (x' * x);
    Q = Q * H;
    A = H * A;
  endfor
    R = A;
endfunction