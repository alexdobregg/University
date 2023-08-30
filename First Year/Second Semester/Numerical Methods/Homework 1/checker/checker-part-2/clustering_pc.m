function [centroids] = clustering_pc(points, NC)
  [N, D] = size(points);
  % Calculam centroizii initiali.
  for i = 1 : NC
    matrixpts = [];
    matrixpts(:, :) = points(i : NC : N, :);
    [rows, col] = size(matrixpts);
    if rows > 1
      prev_centroids(i, :) = sum(matrixpts) / rows;
    else if (rows ~= 0 && rows == 1)
      prev_centroids(i, :) = matrixpts(1, :);
     endif
    endif
  endfor
  % Pana cand centroizii noi nu sunt egali cu cei vechi, nu ne oprim.
  while 1
    vectpoz = zeros(N, 1);
    % Calculam fata de care centroid e mai aproape fiecare punct.
    for i = 1 : N
      min = norm(points(i, :) - prev_centroids(1, :));
      vectpoz(i) = 1;
      for j = 2 : NC
        if min > norm(points(i, :) - prev_centroids(j, :))
          min = norm(points(i, :) - prev_centroids(j, :));
          vectpoz(i) = j;
        endif
      endfor
    endfor
    % Construim matricea in care tinem minte punctele pentru fiecare centroid.
    for i = 1 : NC
      matrixpts2 = [];
      k = 1;
      for j = 1 : N
        if i == vectpoz(j)
          matrixpts2(k, :) = points(j, :);
          k += 1;
        endif
      endfor
      % Calculam centroizii noi.
      [rows2, col2] = size(matrixpts2);
      if rows2 > 1
        centroids(i, :) = sum(matrixpts2) / rows2;
      else if (rows2 ~= 0 && rows2 == 1)
        centroids(i, :) = matrixpts2(1, :);
      endif
      endif
    endfor
    if isequal(prev_centroids, centroids) == 1
      return;
    endif
    prev_centroids = centroids;
  endwhile
endfunction