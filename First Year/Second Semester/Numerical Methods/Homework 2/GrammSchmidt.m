function [Q, R] = GrammSchmidt(A)
    [m,n] = size(A);
    Q = zeros(m);
    R = zeros(m, n);
    for i = 1 : n
    u = zeros(m, 1);
    s = zeros(m, 1);
        for k = 1 : i - 1
            s += dot(A(:, i), Q(:, k)) * Q(:, k);
        endfor
        u = A(:, i) - s;
        Q(:, i) = u / norm(u);
    endfor
    R = Q' * A;
endfunction