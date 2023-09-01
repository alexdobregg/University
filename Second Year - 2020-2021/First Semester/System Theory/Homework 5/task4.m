y4 = lsim(sys, u3, t, x0);
figure(3);
subplot(2, 2, 1);
plot(t, y4(:, 1));
title("Raspunsul total al sistemului");
xlabel("Timp[s]");
ylabel("Rata de tangaj[grd/s]");
subplot(2, 2, 2);
plot(t, y4(:, 2));
title("Raspunsul total al sistemului");
xlabel("Timp[s]");
ylabel("Viteza pe axa OZ a SR atasat Pamantului[m/s]");
subplot(2, 2, 3);
plot(t, y4(:, 3));
title("Raspunsul total al sistemului");
xlabel("Timp[s]");
ylabel("Viteza totala a aeronavei[m/s]");

% Se poate observa din fiecare grafic ca, pentru acelasi moment de timp,
% daca este adunata valoarea din liber cu cea din fortat, va iesi
% aproximativ cu o eroare foarte mica cea din total.