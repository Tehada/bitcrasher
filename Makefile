test:
	g++ -std=c++11 -o bitcrasher functions.hpp ADMM.cpp order_basis.cpp DCT_function.cpp cholesky.cpp im2col.cpp main.cpp `pkg-config --cflags --libs opencv`
