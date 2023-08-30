% reads input data from files, outputs points and computed centroids
function [points centroids] = clustering(file_params, file_points)
	[NC dims points] = read_input_data(file_params, file_points);
	centroids = clustering_pc(points, NC);
endfunction

