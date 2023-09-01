%!test
%! c = dlmread("ref/jf/c_3.txt");
%! values = dlmread("ref/mtc/values_3.txt");
%! colind = dlmread("ref/mtc/colind_3.txt");
%! rowptr = dlmread("ref/mtc/rowptr_3.txt");
%! x_ref = dlmread("ref/js/x_3.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_5.txt");
%! values = dlmread("ref/mtc/values_5.txt");
%! colind = dlmread("ref/mtc/colind_5.txt");
%! rowptr = dlmread("ref/mtc/rowptr_5.txt");
%! x_ref = dlmread("ref/js/x_5.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_7.txt");
%! values = dlmread("ref/mtc/values_7.txt");
%! colind = dlmread("ref/mtc/colind_7.txt");
%! rowptr = dlmread("ref/mtc/rowptr_7.txt");
%! x_ref = dlmread("ref/js/x_7.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_10.txt");
%! values = dlmread("ref/mtc/values_10.txt");
%! colind = dlmread("ref/mtc/colind_10.txt");
%! rowptr = dlmread("ref/mtc/rowptr_10.txt");
%! x_ref = dlmread("ref/js/x_10.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_13.txt");
%! values = dlmread("ref/mtc/values_13.txt");
%! colind = dlmread("ref/mtc/colind_13.txt");
%! rowptr = dlmread("ref/mtc/rowptr_13.txt");
%! x_ref = dlmread("ref/js/x_13.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_15.txt");
%! values = dlmread("ref/mtc/values_15.txt");
%! colind = dlmread("ref/mtc/colind_15.txt");
%! rowptr = dlmread("ref/mtc/rowptr_15.txt");
%! x_ref = dlmread("ref/js/x_15.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_17.txt");
%! values = dlmread("ref/mtc/values_17.txt");
%! colind = dlmread("ref/mtc/colind_17.txt");
%! rowptr = dlmread("ref/mtc/rowptr_17.txt");
%! x_ref = dlmread("ref/js/x_17.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_20.txt");
%! values = dlmread("ref/mtc/values_20.txt");
%! colind = dlmread("ref/mtc/colind_20.txt");
%! rowptr = dlmread("ref/mtc/rowptr_20.txt");
%! x_ref = dlmread("ref/js/x_20.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_25.txt");
%! values = dlmread("ref/mtc/values_25.txt");
%! colind = dlmread("ref/mtc/colind_25.txt");
%! rowptr = dlmread("ref/mtc/rowptr_25.txt");
%! x_ref = dlmread("ref/js/x_25.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);

%!test
%! c = dlmread("ref/jf/c_30.txt");
%! values = dlmread("ref/mtc/values_30.txt");
%! colind = dlmread("ref/mtc/colind_30.txt");
%! rowptr = dlmread("ref/mtc/rowptr_30.txt");
%! x_ref = dlmread("ref/js/x_30.txt");
%! [x] = Jacobi_sparse(values, colind, rowptr, c, 1e-6);
%! assert(norm(x - x_ref) <= 1e-3);