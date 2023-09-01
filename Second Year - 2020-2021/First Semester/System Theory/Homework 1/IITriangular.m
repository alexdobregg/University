function IITriangular()
    clear all
    close all
    clc
    t = -10:0.01:10;
    u = zeros(1, length(t));
    for i = 1 : length(t)
        if t(i) >= -1 && t(i) <= 1
            u(i) = 1 - abs(t(i));
        end
    end
    figure(2);
    plot(t, u);
    xlabel("Timp[s]");
    ylabel("u(t)");
    title("Triangular");
end