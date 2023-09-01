function helper_check_hsv(path_to_in, path_to_ref, i, count_bins)
	x = getImgNames(path_to_in);
	[n d] = size(x);
	img = x(i, 1:d);
	in = strcat(path_to_in, img);
	ref = strcat(path_to_ref, strcat(img(1:length(img)-4), '.hsv' ));
	
	x_out = hsvHistogram(in, count_bins);
	x_ref = dlmread(ref);

	x_out = x_out / sum(x_out);
	x_ref = x_ref / sum(x_ref);
	
	assert(length(x_out) == length(x_ref));
	assert(norm(x_out - x_ref) <= 2e-2)
endfunction