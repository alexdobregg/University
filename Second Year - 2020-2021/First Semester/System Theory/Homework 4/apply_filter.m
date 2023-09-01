function y = apply_filter(H, u, fs)
    t = 0 : (1 / fs) : (10 - 1 / fs);
    y = lsim(H, u, t);
end