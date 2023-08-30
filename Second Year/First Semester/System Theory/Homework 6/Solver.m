clear all;
close all;
clc;
load CAR_TSA.mat;
t = 0 : 100;
% TASK I
system1 = ss(A, B, C, D);
u1 = 100;
u2 = 24;
for i = 1 : length(t)
    u(i, :) = [u1, u2];
end
y1 = lsim(system1, u, t);
pozitia = y1(25, 1);

figure(1);
subplot(3, 1, 1);
plot(t, y1(:, 1));
xlabel("Timp[s]");
ylabel("Pozitia masinii in SR inertial[km]");

subplot(3, 1, 2);
plot(t, y1(:, 2));
xlabel("Timp[s]");
ylabel("Viteza masinii in SR inertia[km/h]");

subplot(3, 1, 3);
plot(t, y1(:, 3));
xlabel("Timp[s]");
ylabel("Temperatura din habitaclul masinii [C]");
% Dupa 25 de secunde, automobilul se afla la pozitia 0.6111 km fata de
% punctul de plecare. Se poate observa din iesirea 3 ca dupa 19-20 de
% secunde, graficul capata un caracter asimptotic, moment in care aproape
% se atinge temperatura din habitaclu (nu este atinsa niciodata).

% TASK II
R = ctrb(A, B);
Q = obsv(A, C);
rangR = rank(R);
rangQ = rank(Q);
% rangR = 3 = n (obtinut din length(A)), deci sistemul este controlabil.
% rangQ = 3 = n (obtinut din length(A)), deci sistemul este observabil.

% TASK III
Bf = [0, 0; 0, 0; 0, 0.33];
system2 = ss(A, Bf, C, D);
y2 = lsim(system2, u, t);

figure(2);
subplot(3, 1, 1);
plot(t, y2(:, 1));
xlabel("Timp[s]");
ylabel("Pozitia masinii in SR inertial[km]");

subplot(3, 1, 2);
plot(t, y2(:, 2));
xlabel("Timp[s]");
ylabel("Viteza masinii in SR inertia[km/h]");

subplot(3, 1, 3);
plot(t, y2(:, 3));
xlabel("Timp[s]");
ylabel("Temperatura din habitaclul masinii [C]");

% Avand in vedere ca primele 2 iesiri alea sistemului nu au nicio valoare
% diferita de 0, sunt 2 lucruri posibile: fie GPS-ul si Encoder-ul Rotii
% sunt stricate, fie acceleratia. Deoarece matricea modificata in Bf este
% B, acest lucru inseamna ca problema este la una dintre elementele de
% actionare descrise de B: acceleratia sau temperatura de referinta pentru
% sistemul de climatizare. Totusi, coloana modificata este prima,
% corespunzatoare primei intrari, si, dupa cum am precizat si anterior,
% aceasta este acceleratia.
R2 =ctrb(A, Bf);
rangR2 = rank(R2);
% rangR2 = 1 != n (obtinut din length(A)), deci sistemul este incontrolabil.
% Q ramane neschimbata deoarece se modifica doar B in Bf, deci sistemul
% este observabil.

% TASK IV
Cf = [1, 0, 0; 0, 1, 0; 0, 0, 0];
system3 = ss(A, B, Cf, D);
y3 = lsim(system3, u, t);

figure(3);
subplot(3, 1, 1);
plot(t, y3(:, 1));
xlabel("Timp[s]");
ylabel("Pozitia masinii in SR inertial[km]");

subplot(3, 1, 2);
plot(t, y3(:, 2));
xlabel("Timp[s]");
ylabel("Viteza masinii in SR inertia[km/h]");

subplot(3, 1, 3);
plot(t, y3(:, 3));
xlabel("Timp[s]");
ylabel("Temperatura din habitaclul masinii [C]");

% Avand in vedere ca 3-a iesire a sistemului nu are nicio valoare
% diferita de 0, si singurul lucru modificat este folosirea matricei Cf
% in locul matricei C, inseamna ca Senzorul de temperatura s-a stricat
% deoarece coloana modificata este cea de-a treia, corespunzatoare acestui
% senzor.

Q3 = obsv(A, Cf);
rangQ3 = rank(Q3);
% R ramane neschimbata deoarece se modifica doar C in Cf, deci sistemul
% este controlabil.
% rangQ3 = 2 != n (obtinut din length(n)), deci sistemul este neobservabil.