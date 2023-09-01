function y = Apartenenta(x, val1, val2)
  % Obtinem valorile a si b pentru care functia este continua.
	a = 1 / (val2 - val1);
  b = 1 - a * val2;
  n = length(x);
  % Pentru fiecare x vom verifica unde se incadreaza si ce valoare va lua y.
  for i = 1 : n
    if (0 <= x(i)) && (x(i) < val1)
      y(i) = 0;
    endif
    if(val1 <= x(i)) && (x(i) <= val2)
      y(i) = a * x(i) + b;
    endif
    if(val2 < x(i)) && (x(i) <= 1)
      y(i) = 1;
    endif
  endfor
endfunction