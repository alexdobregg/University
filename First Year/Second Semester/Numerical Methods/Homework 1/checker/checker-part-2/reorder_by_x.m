% reorders a matrix representing 3D points by the 1st column
function [points_out] = reorder_by_x(points_in)
	[unused indexes] = sort(points_in(:, 1));
	points_out = points_in(indexes, :);
endfunction
