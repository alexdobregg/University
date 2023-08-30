u3 = zeros(length(t), 2);
for i = 1 : length(t)
    u3(i, :) = [2, 3];
end
y3 = lsim(sys, u3, t);
figure(2);
subplot(2, 2, 1);
plot(t, y3(:, 1));
title("Raspunsul fortat al sistemului");
xlabel("Timp[s]");
ylabel("Rata de tangaj[grd/s]");
subplot(2, 2, 2);
plot(t, y3(:, 2));
title("Raspunsul fortat al sistemului");
xlabel("Timp[s]");
ylabel("Viteza pe axa OZ a SR atasat Pamantului[m/s]");
subplot(2, 2, 3);
plot(t, y3(:, 3));
title("Raspunsul fortat al sistemului");
xlabel("Timp[s]");
ylabel("Viteza totala a aeronavei[m/s]");