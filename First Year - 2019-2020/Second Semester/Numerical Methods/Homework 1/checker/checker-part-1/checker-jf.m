%!test
%! A = dlmread("ref/gps/A_3.txt");
%! b = dlmread("ref/gps/b_3.txt");
%! G_ref = dlmread("ref/jf/G_3.txt");
%! c_ref = dlmread("ref/jf/c_3.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_5.txt");
%! b = dlmread("ref/gps/b_5.txt");
%! G_ref = dlmread("ref/jf/G_5.txt");
%! c_ref = dlmread("ref/jf/c_5.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_7.txt");
%! b = dlmread("ref/gps/b_7.txt");
%! G_ref = dlmread("ref/jf/G_7.txt");
%! c_ref = dlmread("ref/jf/c_7.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_10.txt");
%! b = dlmread("ref/gps/b_10.txt");
%! G_ref = dlmread("ref/jf/G_10.txt");
%! c_ref = dlmread("ref/jf/c_10.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_13.txt");
%! b = dlmread("ref/gps/b_13.txt");
%! G_ref = dlmread("ref/jf/G_13.txt");
%! c_ref = dlmread("ref/jf/c_13.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_15.txt");
%! b = dlmread("ref/gps/b_15.txt");
%! G_ref = dlmread("ref/jf/G_15.txt");
%! c_ref = dlmread("ref/jf/c_15.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_17.txt");
%! b = dlmread("ref/gps/b_17.txt");
%! G_ref = dlmread("ref/jf/G_17.txt");
%! c_ref = dlmread("ref/jf/c_17.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_20.txt");
%! b = dlmread("ref/gps/b_20.txt");
%! G_ref = dlmread("ref/jf/G_20.txt");
%! c_ref = dlmread("ref/jf/c_20.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_25.txt");
%! b = dlmread("ref/gps/b_25.txt");
%! G_ref = dlmread("ref/jf/G_25.txt");
%! c_ref = dlmread("ref/jf/c_25.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);

%!test
%! A = dlmread("ref/gps/A_30.txt");
%! b = dlmread("ref/gps/b_30.txt");
%! G_ref = dlmread("ref/jf/G_30.txt");
%! c_ref = dlmread("ref/jf/c_30.txt");
%! [G, c] = Jacobi_factorization(A, b);
%! assert(norm(G - G_ref) <= 1e-3);
%! assert(norm(c - c_ref) <= 1e-3);