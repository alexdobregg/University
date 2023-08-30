function [y] = csr_multiplication(values, colind, rowptr, x)
  n = length(x);
  y = zeros(n, 1);
  for i = 1 : n
      y(i) = sum(values(rowptr(i) : rowptr(i + 1) - 1) * 
            x(colind(rowptr(i) : rowptr(i + 1) - 1)));
  endfor
endfunction