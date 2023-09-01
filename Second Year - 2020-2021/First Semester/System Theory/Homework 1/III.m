function III()
    clear all
    close all
    clc
    t = 0:0.01:100;
    h = (100 / 3) * exp(-t / 3 );
    u1 = sin(10*t);
    u2 = zeros(1, length(t));
    for i = 1 : length(t)
        if t(i) >= 0
            u2(i) = 1;
        end
    end
    y1 = ts_continous_conv(u1, h, t);
    figure(1);
    plot(t,y1);
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    title("Iesirea pentru intrarea de tip sinusoidal");
    y2 = ts_continous_conv(u2, h, t);
    figure(2);
    plot(t,y2);
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    title("Iesirea pentru intrarea de tip treapta");
    % In urma analizarii graficului iesirii y2(t), masina ajunge la
    %viteza maxima dupa aproximativ 17 secunde.
    y2p = ts_continous_conv(u2/2, h, t);
    figure(3);
    plot(t, y2p);
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    title("Iesirea atunci cand pedala este apasata la 50%");
    % In urma analizarii graficului iesirii y2p(t), atunci cand pedala
    %acceleratiei este apasata la 50%, viteza maxima, injumatatita si
    %ea va fi atinsa dupa aproximativ 17 secunde, la fel ca in cazul in
    %care pedala este apasata la 100%.
end