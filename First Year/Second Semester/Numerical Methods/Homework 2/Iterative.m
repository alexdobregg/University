function R = Iterative(nume, d, eps)
  M = citire(nume);
  N = length(M);
  % Initializam last_R
  last_R = ones(N,1) / N;
  while 1
    % Calculam R.
    R = d * M * last_R + ((1 - d) / N) * ones(N, 1);
    % Obtinem eroarea.
    err = norm(R - last_R);
    % Vom iesi din iteratie cand eroare este mai mica decat eps.
    if err < eps
      % Deoarece se va face o iteratie in plus, vom returna penultimul R.
      R = last_R;
      return;
    endif
    % last_R devine R.
    last_R = R;
  endwhile
endfunction 