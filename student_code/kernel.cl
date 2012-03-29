
kernel void update(global float *in, global float *out) {
	int WIDTH = get_global_size(0);
	int HEIGHT = get_global_size(1);
	// Don't do anything if we are on the edge.
	if (get_global_id(0) == 0 || get_global_id(1) == 0)
		return;
	if (get_global_id(0) == (WIDTH-1) || get_global_id(1) == (HEIGHT-1))
		return;
	int y = get_global_id(1);	
	int x = get_global_id(0); 
	// Load the data
	float a = in[WIDTH*(y-1)+(x)];
	float b = in[WIDTH*(y)+(x-1)];
	float c = in[WIDTH*(y+1)+(x)];
	float d = in[WIDTH*(y)+(x+1)];
	float e = in[WIDTH*y+x];
	// Do the computation and write back the results
	out[WIDTH*y+x] = (0.1*a+0.2*b+0.2*c+0.1*d+0.4*e);
}

