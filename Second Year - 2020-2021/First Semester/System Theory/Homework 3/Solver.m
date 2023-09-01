classdef Solver
    methods
        % 1
        function T = solve_1(self)
        % Find the parameter T of a first-order transfer function
        % with k = 1000 and settling time of 0.4 seconds on impulse
            s = tf('s');
            k = 1000;
            t_settle = 0.4;
            T = t_settle / 4;
            % T nu se modifica daca k creste de 10 ori.
        end
        
        % 2
        function w = solve_2(self)
        % Find the w parameter of a second-order transfer function
        % with zeta = 0.2 and a peak time of 0.4 seconds on step
            s = tf('s');
            z = 0.2;
            t_peak = 0.4;
            w = pi / (t_peak * sqrt(1 - z ^ 2));
            % z = 0 => oscilatii la infinit
            % z cat mai mare => sistemul se apropie de unul de ordin 1
        end
        
        % 3
        function t_rise = solve_3(self, w, zetas)
        % Compute the rise times of a second-order function
        % with given w for each value zeta in zetas
            s = tf('s');
            for i = 1 : length(zetas)
                a = pi - acos(zetas(i));
                b = w * sqrt(1 - zetas(i) ^ 2);
                t_rise(i) = a / b;
            end
        end
        
        % 4
        function overshoots = solve_4(self, w, zetas)
        % Compute the overshoots of a second-order function
        % with given w for each value zeta in zetas
        % (don't convert to percents)
            s = tf('s');
            for i = 1 : length(zetas)
                a = - zetas(i) * pi;
                b = sqrt(1 - zetas(i)^2);
                overshoots(i) = exp(a / b);
            end
        end
        
        % 5
        function t_stationary = solve_5(self)
        % Compute the time it takes for the Iron Man suit to stop.
        % Time and input are provided.
            s = tf('s');
            w = 12;
            z = 0.2;
            H = w^2 / (s^2 + 2*w*z*s + w^2);
            t = [0:0.1:10];         % Use this time vector
            u = [t <= 3] - [t > 3]; % Use this as input
            y = lsim(H, u, t);
            for i = 1 : length(y)
                if y(i) > - 0.1 & y(i) < 0.1
                    t_stationary = i / 10;
                end
            end
        end
    end
end