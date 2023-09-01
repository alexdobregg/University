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
    % Construim X-ul care contine o linie pentru fiecare poza.
    for i = 1 : nrC
      path1 = strcat(path_to_cats, imgcats(i, :));
      X(i, :) = rgbHistogram(path1, count_bins);
    endfor
    for i = 1 : nrNC
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      X(i + nrC, :) = rgbHistogram(path2, count_bins);
    endfor
    % Scalam coloanele lui X.
    [rows, cols] = size(X);
    X = (X - mean(X)) ./ std(X);
    % Creez X tilda si completez y prezis dupa formula.
    v(1 : rows, 1) = 1;
    Xt = [X, v];
    for i = 1 : nrC
      yPrezis(i, 1) = w' * Xt(i, :)';
    endfor
    for i = 1 : nrNC
      yPrezis(nrC + i, 1) = w' * Xt(i + nrC, :)';
    endfor
  endif
  
  if strcmp(histogram, 'HSV') == 1
    % Construim X-ul care contine o linie pentru fiecare poza.
    for i = 1 : nrC
      path1 = strcat(path_to_cats, imgcats(i, :));
      X(i, :) = hsvHistogram(path1, count_bins);
    endfor
    for i = 1 : nrNC
      path2 = strcat(path_to_not_cats, imgnotcats(i, :));
      X(i + nrC, :) = hsvHistogram(path2, count_bins);
    endfor
    % Scalam coloanele lui X.
    [rows, cols] = size(X);
    X = (X - mean(X)) ./ std(X);
    % Creez X tilda si completez y prezis dupa formula.
    v(1 : rows, 1) = 1;
    Xt = [X, v];
    for i = 1 : nrC
      yPrezis(i, 1) = w' * Xt(i, :)';
    endfor
    for i = 1 : nrNC
      yPrezis(nrC + i, 1) = w' * Xt(i + nrC, :)';
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