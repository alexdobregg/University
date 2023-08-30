% reads cluster count and points from input files 
function [NC dims points] = read_input_data(file_params, file_points)
	load (file_params);
	load (file_points);
  
  NC;
  dims;
  points;
endfunction

