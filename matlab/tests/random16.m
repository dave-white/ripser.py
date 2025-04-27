addpath('..')

D = single(readmatrix('random16.lower_distance_matrix', 'FileType', 'text', 'NumHeaderLines', 0));
Db = D(tril(true(size(D))));

rips_opts = struct('dim_max', 4, 'modulus', 2, 'threshold', 80, 'do_cocycles', false);

[barcodes, rep_cocycles, edges_removed] = rips(Db, rips_opts);

