function [x] = Jacobi_sparse(G_values, G_colind, G_rowptr, c, tol)
    % Obtinem lungimea solutiei si initializam valorile primului x la 0.
    n = length(G_rowptr) - 1;
    prev_x = zeros(n, 1);
    % Vom calcula pana cand vom gasi o solutie satisfacatoare.
    while 1
        y = csr_multiplication(G_values, G_colind, G_rowptr, prev_x);
        x = y + c;
        % Calculam eroarea.
        err = norm(x - prev_x);
        if err < tol
            return;
        endif
        prev_x = x;
    endwhile
endfunction