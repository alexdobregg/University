function IRectangular()
    clear all
    close all
    clc
    t = -10:0.01:10;
    u = zeros(1, length(t));
    a = -5;
    b = 5;
    for i = 1 : length(t)
        if t(i) >= a && t(i) <= b
            u(i) = 1;
        end
    end
    figure;
    plot(t, u);
    xlabel("Timp[s]");
    ylabel("u(t)");
    title("Rectangular");
end
