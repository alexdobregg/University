function y1 = task4()
    fh = 5000;
    fl = 1000;
    [y, fs] = play_file("supernova.wav", 0);
    s = tf('s');
    R = 600;
    Ch = 1 / (2 * pi * fl * R);
    Cl = 1 / (2 * pi * fh * R);
    % Pentru a amplifica cu cel putin 5db plaja de frecvente
    %trebuie sa inmultim produsul functiilor de transfer cu k.
    %k se calculeaza din formula 5 = 20 * lg(k), de unde
    %k e aproximativ 1.78, deci putem lua si o valoare mai mare
    %spre exemplu 3.
    Hh = (s * R * Ch) / (1 + s * R * Ch);
    Hl = 1 / (1 + s * R * Cl);
    figure(1);
    bode(Hh);
    figure(2);
    bode(Hl);
    % Am folosit 2 functii de transfer, una specifica filtrului High Pass
    % si una filtrului Low Pass pe care l-am inseriat pentru a obtine o
    % amplificare de cel putin 5 db.
    %y1 = apply_filter(3 * Hh * Hl, y, fs);
    %figure(1);
    %plot_fft(y, fs);
    %figure(2);
   % plot_fft(y1, fs);
    %figure(3);
    %bode(3 * Hh * Hl);
    %figure(4);
    %bode(Hh *  Hl);
    % Am facut si diagrama bode pentru semnalul amplificat si pentru cel
    % initial pentru a observa o crestere de cel putin 5 db.
end