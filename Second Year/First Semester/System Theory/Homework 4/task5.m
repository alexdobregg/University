function yr = task5()
    alpha = 45/50;
    [y, fs] = merge_sound_files("supernova.wav", "beep.wav", alpha);
    [ybeep, fsbeep] = play_file("beep.wav", 0);
    figure(1);
    plot_fft(ybeep, fsbeep);
    % Din grafic observam ca frecventa beep-ului este cuprinsa intre 15995
    % si 16005.
    fl = 15995;
    fh = 16005;
    f0 = sqrt(fl * fh);
    w0 = f0 * 2 * pi;
    s = tf('s');
    % Am folosit o functie de transfer specifica filtrului notch pentru a
    % opri frecventele beep-ului.
    H = (s ^ 2 + w0 ^ 2) / (s ^ 2 +  4 * s * w0 + w0 ^ 2);
    figure(2);
    bode(H);
    figure(3);
    plot_fft(y, fs);
    yr = apply_filter(H, y, fs);
    figure(4);
    plot_fft(yr, fs);
end