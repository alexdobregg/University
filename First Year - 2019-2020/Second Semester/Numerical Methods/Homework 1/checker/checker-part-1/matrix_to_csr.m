function [values, colind, rowptr] = matrix_to_csr(A)
  n = size(A, 1);
  % Obtinem indicii pentru coloane si pentru linii, precum
  % si valorile nenule din matricea A.  
  [colind, rowind, values] = find(A');
  nz = length(values);
  colind = colind';
  rowind = rowind';
  values = values';
  rowptr = zeros(1, n + 1);
  rowptr(n + 1) = nz + 1;
  rowptr(1) = 1;
  p = rowind(1);
  index = 2;
  % Parcurgem vectorul de indici pentru a gasi locul din colind a primei
  % valori de pe fiecare rand.
  for i = 2 : nz
    if rowind(i) ~= p
      p = rowind(i);
      rowptr(index) = i;
      index += 1;
    endif
  endfor
endfunction