function [sol] = hsvHistogram(path_to_image, count_bins)
  I = imread(path_to_image);
  % Obtinem vectorii coloana ale fiecarei matrici specifice unei culori.
  R = double(I(:, :, 1)'(:)) / 255;
  G = double(I(:, :, 2)'(:)) / 255;
  B = double(I(:, :, 3)'(:)) / 255;
  % Obtinem vectorul cu valori maxime si cel cu valori minime.
  Cmax = max([R, G, B]')';
  Cmin = min([R, G, B]')';
  delta = Cmax - Cmin;
  % Obtinem indicii pozitiilor in care stim valorile carei culori se afla in Cmax.
  idxR = (Cmax == R);
  idxG = (Cmax == G);
  idxB = (Cmax == B);
  % Completam vectorul H corespunzator dupa indicii aflati.
  if any(delta(idxR)) == 1
    H(idxR) = 60 * mod((G(idxR) - B(idxR)) ./ delta(idxR), 6);
  endif
  if any(delta(idxG)) == 1
    H(idxG) = 60 * ((B(idxG) - R(idxG)) ./ delta(idxG) + 2);
  endif
  if any(delta(idxB)) == 1 
    H(idxB) = 60 * ((R(idxB) - G(idxB)) ./ delta(idxB) + 4);
  endif
  H = H / 360;
  % Obtinem indicii pentru care Cmax are valori nule si pentru care nu.
  idx0 = (Cmax == 0);
  idx1 = (Cmax ~= 0);
  % Completam vectorul S corespunzator dupa indicii aflati.
  S(idx0) = 0;
  if any(Cmax(idx1)) == 1
    S(idx1) = delta(idx1) ./ Cmax(idx1);
  endif
  % V va fi Cmax;
  V = Cmax';
  sol = [];
  % Obtinem vectorii pentru H, S, V in urma folosirii lui histc.
  vecH = histc(H, 0 : double(1.01 / count_bins) : 1.01);
  vecS = histc(S, 0 : double(1.01 / count_bins) : 1.01);
  vecV = histc(V, 0 : double(1.01 / count_bins) : 1.01);
  % Eliminam ultimul element din fiecare.
  vecH = vecH(1 : end - 1);
  vecS = vecS(1 : end - 1);
  vecV = vecV(1 : end - 1);
  % Comepletam solutia.
  sol = [sol, vecH];
  sol = [sol, vecS];
  sol = [sol, vecV];
  sol = (sol(:))';
endfunction