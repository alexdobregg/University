function [w] = learn(X, y, lr, epochs)
  [rows, cols] = size(X);
  batch_size = 64;
  % Scalam fiecare coloana a lui X.
  X = (X - mean(X)) ./ std(X);
  % Construim X tilda.
  v(1 : rows, 1) = 1;
  Xt = [X, v];
  colst = cols + 1;
  % Obtinem un w cu valori random in intervalul [-0.1, 0.1].
  x1 = -0.1;
  x2 = 0.1;
  w = x1 + ((x2 - x1) * rand(colst, 1));
  for epoch = 1 : epochs
    % Obtinem batch_size linii random si construim Xbatch si ybatch cu ele.
    randrows = randperm(rows);
    Xbatch = Xt(randrows(1 : batch_size), :);
    ybatch = y(randrows(1 : batch_size));
    % Vom calcula noul w.
    for i = 1 : colst
      w(i) = w(i) - (lr / rows) * sum((Xbatch * w - ybatch) .* Xbatch(:, i));
    endfor
  endfor
endfunction