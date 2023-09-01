function [sol] = rgbHistogram(path_to_image, count_bins)
  I = imread(path_to_image);
  I = double(I);
  sol = [];
  % Parcurgem fiecare dintre R, G si B.
  for nr = 1 : 3
    % Obtinem vectorul coloana din matricea culorii.
    color = I(:, :, nr)'(:);
    % Obtinem vectorul in urma aplicarii histc.
    colorHist = histc(color, 0 : double(256 / count_bins) : 256);
    % Eliminam ultimul element.
    colorHist = colorHist(1 : end - 1);
    % Construim solutia.
    sol = [sol, colorHist];
  endfor
  sol = (sol(:))';
endfunction