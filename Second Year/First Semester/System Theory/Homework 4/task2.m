function [R, C] = task2()
    [y, fs] = play_file("bass_test.wav", 0);
    s = tf('s');
    % Am ales o rezistenta de 10 ohmi pentru a putea calcula C, avand deja
    % freceventa de cut-off
    R = 10;
    fct = 100;
    C = 1 / (2 * pi * fct * R);
    H = (s * R * C) / (1 + s * R * C);
    y = apply_filter(H, y, fs);
    figure(1);
    bode(H);
    figure(2);
    plot_fft(y, fs);
end