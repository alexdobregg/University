%!test
%! G = dlmread("ref/jf/G_3.txt");
%! values_ref = dlmread("ref/mtc/values_3.txt");
%! colind_ref = dlmread("ref/mtc/colind_3.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_3.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_5.txt");
%! values_ref = dlmread("ref/mtc/values_5.txt");
%! colind_ref = dlmread("ref/mtc/colind_5.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_5.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_7.txt");
%! values_ref = dlmread("ref/mtc/values_7.txt");
%! colind_ref = dlmread("ref/mtc/colind_7.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_7.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_10.txt");
%! values_ref = dlmread("ref/mtc/values_10.txt");
%! colind_ref = dlmread("ref/mtc/colind_10.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_10.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_13.txt");
%! values_ref = dlmread("ref/mtc/values_13.txt");
%! colind_ref = dlmread("ref/mtc/colind_13.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_13.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_15.txt");
%! values_ref = dlmread("ref/mtc/values_15.txt");
%! colind_ref = dlmread("ref/mtc/colind_15.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_15.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_17.txt");
%! values_ref = dlmread("ref/mtc/values_17.txt");
%! colind_ref = dlmread("ref/mtc/colind_17.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_17.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_20.txt");
%! values_ref = dlmread("ref/mtc/values_20.txt");
%! colind_ref = dlmread("ref/mtc/colind_20.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_20.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_25.txt");
%! values_ref = dlmread("ref/mtc/values_25.txt");
%! colind_ref = dlmread("ref/mtc/colind_25.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_25.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);

%!test
%! G = dlmread("ref/jf/G_30.txt");
%! values_ref = dlmread("ref/mtc/values_30.txt");
%! colind_ref = dlmread("ref/mtc/colind_30.txt");
%! rowptr_ref = dlmread("ref/mtc/rowptr_30.txt");
%! [values, colind, rowptr] = matrix_to_csr(G);
%! assert(norm(values - values_ref) <= 1e-3);
%! assert(norm(colind - colind_ref) <= 1e-3);
%! assert(norm(rowptr - rowptr_ref) <= 1e-3);