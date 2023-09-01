function [cost] = compute_cost_pc(points, centroids)
  % Obtinem numarul de linii din points si din centroids.
  [N, D] = size(points);
  [NC, col] = size(centroids);
  vectpoz = zeros(N, 1);
  cost = 0;
  % Parcurgem prin liniile punctelor si cautam distanta minima.
  for i = 1 : N
    min = norm(points(i, :) - centroids(1, :));
    vectpoz(i) = 1;
    for j = 2 : NC
      if min > norm(points(i, :) - centroids(j, :))
        min = norm(points(i, :) - centroids(j, :));
      endif
    endfor
    % Adunam la cost acea distanta minima.
    cost += min;
  endfor
endfunction

