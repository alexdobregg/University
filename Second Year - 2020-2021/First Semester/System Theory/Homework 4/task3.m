function param_Gr = task3()
    s = tf('s');
    [y, fs] = play_file("bass_test.wav", 0);
    H = (s ^ 2) / (s ^ 2 + 62.83 * s + 394800);
    figure(1);
    bode(H);
    y = apply_filter(H, y, fs);
    figure(3);
    plot_fft(y, fs);
    % Fiind un sistem de ordin 2, la numitor avem formula
    %s^2 + 2 * zeta * wn + wn^2. wn = sqrt(394800) = 628.3.
    %2 * zeta * wn = 62.83 => zeta = 0.05, mult prea mic.
    param_Gr = 0.05;
    % O solutie ar fi ca zeta sa fie cat mai aproape de 1.
end