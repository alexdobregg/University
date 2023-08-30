%!test
%! A_ref = dlmread("ref/gps/A_3.txt");
%! b_ref = dlmread("ref/gps/b_3.txt");
%! [A, b] = generate_probabilities_system(3);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_5.txt");
%! b_ref = dlmread("ref/gps/b_5.txt");
%! [A, b] = generate_probabilities_system(5);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_7.txt");
%! b_ref = dlmread("ref/gps/b_7.txt");
%! [A, b] = generate_probabilities_system(7);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_10.txt");
%! b_ref = dlmread("ref/gps/b_10.txt");
%! [A, b] = generate_probabilities_system(10);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_13.txt");
%! b_ref = dlmread("ref/gps/b_13.txt");
%! [A, b] = generate_probabilities_system(13);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_15.txt");
%! b_ref = dlmread("ref/gps/b_15.txt");
%! [A, b] = generate_probabilities_system(15);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_17.txt");
%! b_ref = dlmread("ref/gps/b_17.txt");
%! [A, b] = generate_probabilities_system(17);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_20.txt");
%! b_ref = dlmread("ref/gps/b_20.txt");
%! [A, b] = generate_probabilities_system(20);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_25.txt");
%! b_ref = dlmread("ref/gps/b_25.txt");
%! [A, b] = generate_probabilities_system(25);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);

%!test
%! A_ref = dlmread("ref/gps/A_30.txt");
%! b_ref = dlmread("ref/gps/b_30.txt");
%! [A, b] = generate_probabilities_system(30);
%! assert(norm(A - A_ref) <= 1e-3);
%! assert(norm(b - b_ref) <= 1e-3);