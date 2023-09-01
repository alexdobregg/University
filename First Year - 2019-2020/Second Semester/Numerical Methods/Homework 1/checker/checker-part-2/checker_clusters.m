%!test
%!  [points_out centroids_out] = clustering("in/test-part2-1.param", "in/test-part2-1.points");
%!  load("refs/test-part2-1.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-2.param", "in/test-part2-2.points");
%!  load("refs/test-part2-2.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-3.param", "in/test-part2-3.points");
%!  load("refs/test-part2-3.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-4.param", "in/test-part2-4.points");
%!  load("refs/test-part2-4.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-5.param", "in/test-part2-5.points");
%!  load("refs/test-part2-5.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-6.param", "in/test-part2-6.points");
%!  load("refs/test-part2-6.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-7.param", "in/test-part2-7.points");
%!  load("refs/test-part2-7.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-8.param", "in/test-part2-8.points");
%!  load("refs/test-part2-8.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-9.param", "in/test-part2-9.points");
%!  load("refs/test-part2-9.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-10.param", "in/test-part2-10.points");
%!  load("refs/test-part2-10.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-11.param", "in/test-part2-11.points");
%!  load("refs/test-part2-11.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);

%!test
%!  [points_out centroids_out] = clustering("in/test-part2-12.param", "in/test-part2-12.points");
%!  load("refs/test-part2-12.ref");
%!  centroids_out = reorder_by_x(centroids_out);
%!  centroids = reorder_by_x(centroids);
%!  centroids_diff = centroids - centroids_out;
%!  assert(max(norm(centroids_diff, 2, "rows")) <= 1e-5);
