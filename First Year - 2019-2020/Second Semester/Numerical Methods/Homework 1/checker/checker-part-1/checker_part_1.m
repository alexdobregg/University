function [score] = checker_part_1()
    [n_gps, n_gps_max] = test("checker-gps");
    printf("Generate probabilities system: %d / %d teste trecute.\n", n_gps, n_gps_max);

    [n_jf, n_jf_max] = test("checker-jf");
    printf("Jacobi factorization: %d / %d teste trecute.\n", n_jf, n_jf_max);

    [n_mtc, n_mtc_max] = test("checker-mtc");
    printf("Matrix to CSR: %d / %d teste trecute.\n", n_mtc, n_mtc_max);

    [n_js, n_js_max] = test("checker-js");
    printf("Jacobi sparse: %d / %d teste trecute.\n", n_js, n_js_max);
    
    score = 40 * (n_gps / n_gps_max / 4 + n_jf / n_jf_max / 4 + n_mtc / n_mtc_max / 4 + n_js / n_js_max / 4);
    printf("Punctaj total: %.2f\n", score);
    
    fout = fopen("results", "w");
    fprintf(fout, "%.2f", score);
    fclose(fout);
endfunction
