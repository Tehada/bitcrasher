bitcrasher:
	clang++ -std=c++11 -o bitcrasher ADMM.cpp order_basis.cpp DCT_function.cpp cholesky.cpp im2col.cpp main.cpp `pkg-config --cflags --libs opencv`
