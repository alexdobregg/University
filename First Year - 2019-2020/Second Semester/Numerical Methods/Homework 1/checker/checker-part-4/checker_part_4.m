function [score] = checker_part_4()

    [n_prediction, n_prediction_max] = test("checker_prediction");
    printf("[Mini-batch Gradient Descent] Cat prediction: %d / %d teste trecute.\n", n_prediction, n_prediction_max);
  
    score = 20 * (n_prediction) / (n_prediction_max);
    printf("Punctaj total: %.2f\n", score);

    fout = fopen("results", "w");
    fprintf(fout, "%.2f", score);
    fclose(fout);
endfunction
