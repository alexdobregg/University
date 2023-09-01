function V()
    clear all
    close all
    clc
    t = 0:0.01:100;
    h = (100 / 3) * exp(-t / 3 );
    u4 = zeros(1, length(t));
    u2 = zeros(1, length(t));
    for i = 1 : length(t)
        if t(i) >= 0
            u2(i) = 1;
        end
    end
     for i = 1 : length(t)
        if t(i) >= 3
            u4(i) = 1;
        end
     end
    y2 = ts_continous_conv(u2, h, t);
    y4 = ts_continous_conv(u4, h, t);
    subplot(2,2,1);
    plot(t,u2);
    title("Semnalul treapta unitara");
    xlabel("Timp[s]");
    ylabel("u2(t)");
    subplot(2,2,2);
    plot(t,y2);
    title("Iesirea semnalului treapta unitara");
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    subplot(2,2,3);
    plot(t,u4);
    title("Semnalul treapta unitara intarziat");
    xlabel("Timp[s]");
    ylabel("u4(t)");
    subplot(2,2,4);
    plot(t,y4);
    title("Iesirea semnalului treapta unitara intarziat");
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    % Deoarece semnalul de intrare a fost intarziat cu 3 secunde si
    %deoarece acesta era de tip treapta, prin urmare si iesirea va fi
    %intarziata cu tot atatea secunde.
end