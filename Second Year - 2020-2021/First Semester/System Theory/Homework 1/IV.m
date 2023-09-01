function IV()
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
    u3 = u1 + u2;
    y3 = ts_continous_conv(u3, h, t);
    subplot(2,1,1);
    plot(t,u3);
    xlabel("Timp[s]");
    ylabel("u3(t)");
    subplot(2,1,2);
    plot(t,y3);
    xlabel("Timp[s]");
    ylabel("Viteza[m/s]");
    % Caracterul de sistem liniar al modelului automobilului provine din
    %faptul ca u3(t) = (u1 + u2)(t) = u1(t) + u2(t) si din
    %u1(t) -> y1(t); u2(t) ->y2(t) => y3(t) = y1(t) + y2(t)
end