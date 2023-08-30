function [M] = citire(nume)
  % Deschidem fisierul si obtinem numarul de noduri.
  fileID = fopen(nume, 'r');
  N = fscanf(fileID, "%f", 1);
  % Initializam matricea A cu toate elementele 0.
  A = zeros(N);
  % Parcurgem fisierul pentru a completa matricea de adiacenta.
  for i = 1 : N
    % Obtinem numarul nodului.
    Nod = fscanf(fileID, "%f", 1);
    % Obtinem numarul de vecini ai nodului.
    nrVecini = fscanf(fileID, "%f", 1);
    % Se completeaza vectorul L cu acest numar.
    L(i) = nrVecini;
    % Obtinem vectorul care contine vecinii.
    vecini = fscanf(fileID, "%f", nrVecini);
    % Pentru nodurile care ii sunt vecine punem 1.
    A(i, vecini) = 1;
    nr = sum(vecini == Nod);
    % Scadem de pe pozitia din L de cate ori un nod are link catre el insusi.
    L(i) -= nr;
    % Setam pe diagonala 0 deoarece nu ne intereseaza aceste valori.
    A(i, i) = 0;
  endfor
  fclose(fileID);
  % Calculam matricea M a probabilitatii transitiei.
  M = A' ./ L;
endfunction
