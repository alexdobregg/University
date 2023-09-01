function [percentage] = evaluate(path_to_testset, w, histogram, count_bins)
  % Creez pathul catre pozele cu pisici si catre cele fara si obtin numele
  % tuturor pozelor.
  catsStr = 'cats/';
  notcatsStr = 'not_cats/';
  
  path_to_cats = strcat(path_to_testset, catsStr);
  path_to_not_cats = strcat(path_to_testset, notcatsStr);
  
  imgcats = getImgNames(path_to_cats);
  imgnotcats = getImgNames(path_to_not_cats);
  
  [nrC, var1] = size(imgcats);
  [nrNC, var2] = size(imgnotcats);
  
  yAdev = zeros(nrC + nrNC, 1);
  yPrezis = zeros(nrC + nrNC, 1);
  % Punem 1 pentru pozele cu pisici si -1 pentru cele fara pentru un y corect.
  yAdev(1 : nrC, 1) = 1;
  yAdev(nrC + 1 : end, 1) = -1;
  % In functie de tipul histogramei, vom completa y-ul cu valorile
  % corespunzatoare pozelor cu pisici si celor fara dupa formula data.
  if strcmp(histogram, 'RGB') == 1
    for i = 1 : nrC
      x = [];
      path1 = strcat(path_to_cats, imgcats(i, :));
      x(1, :) = rgbHistogram(path1, count_bins);
      x = [x 1];
      x = x';
      yPrezis(i, 1) = w' * x;
    endfor
    for i = 1 : nrNC
      x = [];
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      x(1, :) = rgbHistogram(path2, count_bins);
      x = [x 1];
      x = x';
      yPrezis(nrC + i, 1) = w' * x;
    endfor
  endif
  
  
  if strcmp(histogram, 'HSV') == 1
    for i = 1 : nrC
      x = [];
      path1 = strcat(path_to_cats, imgcats(i, :));
      x(1, :) = hsvHistogram(path1, count_bins);
      x = [x 1];
      x = x';
      yPrezis(i, 1) = w' * x;
    endfor
    for i = 1 : nrNC
      x = [];
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      x(1, :) = hsvHistogram(path2, count_bins);
      x = [x 1];
      x = x';
      yPrezis(nrC + i, 1) = w' * x;
    endfor
  endif
  % Obtinem indicii pentru care valorile din y prezis sunt mai mari si mai mici
  % decat 0, dupa care completam pozitiile mai mari decat 0 cu 1 si cele mai
  % mici cu -1.
  idxC = (yPrezis >= 0);
  idxNC = (yPrezis < 0);
  
  yPrezis(idxC) = 1;
  yPrezis(idxNC) = -1;
  % Obtinem cate poze au fost prezise corect si scoatem procentajul.
  PozePrezise = (yPrezis == yAdev);
  nrPrezise = length(PozePrezise(PozePrezise ~= 0));
  
  percentage = double(nrPrezise / (nrC + nrNC));
endfunction