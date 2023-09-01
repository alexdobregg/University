close all
clear all
clc
s = tf('s');
H = 100 / (2 * s + 1);
%   I. Polii. Sistemul este stabil deoarece are un singur pol si anume -0.5
%care se afla in partea stabila datorita semnului - a partii reale..
pole(H);

%   II. Propunerea unui H' care ar face sistemul instabil. Polii acestui
%sistem sunt -0.5 si 0.5 si prin urmare sistemul a devenit instabil
%deoarece are un pol in partea instabila datorita semnului + a partii reale.
Hp = 100 / (4*s ^ 2 - 1);
pole(Hp);

%   III. Raspunsul unui propulsor cand este folosit pentru transport.
rsp_trpt = step(H);
subplot(2, 1, 1);
plot(rsp_trpt);
title('Raspuns la treapta pentru H');
xlabel('Timp');
ylabel('Amplitudine');

rsp_trptp = step(Hp);
subplot(2, 1, 2);
plot(rsp_trptp);
title('Raspuns la treapta pentru Hp');
xlabel('Timp');
ylabel('Amplitudine');
%   Se poate observa din grafic ca raspunsul de la punctul b este corect
%deoarece in comparatie cu raspunsul la treapta al lui H, cel al lui Hp nu
%corespunde cu semnalul de intrare treapta.

%   IV.Raspunsul unui propulsor atunci cand este folosit ofensiv.
rsp_ofensiv = impulse(H);
figure;
plot(rsp_ofensiv);
title('Raspuns la impuls');
xlabel('Timp');
ylabel('Amplitudine');

%   V.Raspunsul unui propulsor atunci cand este folosit pentru mentine
%costumul in aer cu o altitudine medie constanta, dar nu stationar.
t = 0:0.1:10;
u = zeros(1, length(t));
for i = 1 : length(t)
    if t(i) >= 0 && t(i) <= 1
            u(i) = 1;
    end
    if t(i) >= 2 && t(i) <= 3
            u(i) = 1;
    end
    if t(i) >= 4 && t(i) < 5
            u(i) = 1;
    end
end
rsp_dr = lsim(H,u,t);
figure(3);
plot(t, rsp_dr);
hold on;
plot(t, u);
title('Raspuns la un semnal de intrare dreptunghiular');
legend({'Intrare','Iesire'});
xlabel('Timp');
ylabel('Amplitudine');
grid on;