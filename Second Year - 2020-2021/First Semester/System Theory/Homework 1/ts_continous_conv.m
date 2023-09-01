function [ y ] = ts_continous_conv( u,h,t )
    %% ts_continous_conv 
    % Outputs the convolution result between u and h and shows up its plot vs
    % time t

    % u - input function
    % h - impulse response function
    % t - time steps array

    dt = t(2) - t(1);

    y = conv(u,h)*dt 
    y = y(1:length(t));

end