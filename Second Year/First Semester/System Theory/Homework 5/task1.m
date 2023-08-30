sys = ss(A_lon, B_lon, C_lon, D_lon);
H = tf(sys);
syspoles = pole(H);
specA = eig(A_lon);
% Se poate observa ca pentru pole(H) vom obtine polii lui H care sunt
% dubli. Folosind eig(A_lon) vom obtine aceleasi valori ca si polii lui H.

% Nu se poate facea aceasta transformare din model pe stare in functie de
% transfer mereu deoarece din H(s) = (s * I - A)^ (-1) * B + D => ca
% s * I - A trebuie sa fie inversabila, lucru care depinde de s, ceea ce nu
% este intodeauna adevarat, deci prin urmare, nu se poate face mereu
% transfoamrea.

% Sistemul este stabil deoarece se poate observa din valorile proprii ale
% lui A_lon ca se afla in semiplanul stang, partea reala a
% fiecareia fiind negativa.