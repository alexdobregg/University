function [X, y] = preprocess(path_to_dataset, histogram, count_bins)
  % Creez pathul catre pozele cu pisici si catre cele fara si obtin numele
  % tuturor pozelor.
  catsStr = 'cats/';
  notcatsStr = 'not_cats/';
  
  path_to_cats = strcat(path_to_dataset, catsStr);
  path_to_not_cats = strcat(path_to_dataset, notcatsStr);
  
  imgcats = getImgNames(path_to_cats);
  imgnotcats = getImgNames(path_to_not_cats);
  
  [nrC, var1] = size(imgcats);
  [nrNC, var2] = size(imgnotcats);
  
  X = zeros(nrC + nrNC, count_bins * 3);
  y = zeros(nrC + nrNC, 1);
  % Punem 1 pentru pozele cu pisici si -1 pentru cele fara.
  y(1 : nrC, 1) = 1;
  y(nrC + 1 : end, 1) = -1;
  % In functie de tipul histogramei, vom completa X-ul cu primele linii
  % corespunzatoare pozelor cu pisici si cu urmatoarele celor fara.
  if strcmp(histogram, 'RGB') == 1
    for i = 1 : nrC
      path1 = strcat(path_to_cats, imgcats(i, :));
      X(i, :) = rgbHistogram(path1, count_bins);
    endfor
    for i = 1 : nrNC
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      X(nrC + i, :) = rgbHistogram(path2, count_bins);
    endfor
  endif
  
  if strcmp(histogram, 'HSV') == 1
    for i = 1 : nrC
      path1 = strcat(path_to_cats, imgcats(i, :));
      X(i, :) = hsvHistogram(path1, count_bins);
    endfor
    for i = 1 : nrNC
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      X(nrC + i, :) = hsvHistogram(path2, count_bins);
    endfor
  endif
endfunction
