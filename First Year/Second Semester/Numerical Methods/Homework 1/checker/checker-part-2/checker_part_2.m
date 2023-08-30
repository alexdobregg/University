function [score] = checker_part_2()
    [n_clusters, n_clusters_max] = test("checker_clusters");
    printf("Centroid checking: %d / %d teste trecute.\n", n_clusters, n_clusters_max);
    
    [n_cost, n_cost_max] = test("checker_cost");
    printf("Cost computing: %d / %d teste trecute.\n", n_cost, n_cost_max);

    score = 30 * (n_cost / n_cost_max / 2 + n_clusters / n_clusters_max / 2);
    printf("Punctaj total: %.2f\n", score);

    fout = fopen("results", "w");
    fprintf(fout, "%.2f", score);
    fclose(fout);
endfunction
