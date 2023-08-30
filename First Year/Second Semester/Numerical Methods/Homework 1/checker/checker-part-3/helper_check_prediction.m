function helper_check_prediction(hist_type, count_bins)
    path_to_dataset  = 'dataset/';
    path_to_testset  = 'testset/';

    [X, y] = preprocess(path_to_dataset, hist_type, count_bins);
    w = learn(X, y);
    p = evaluate(path_to_testset, w, hist_type, count_bins);
    assert(p >= 0.68);
    assert(p <= 1);
endfunction
