PROJECT = bitcrasher
CXX = clang++
CXXFLAGS = -std=c++11 `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`
SRCS = src/ADMM.cpp src/order_basis.cpp src/DCT_function.cpp src/cholesky.cpp src/im2col.cpp src/main.cpp

all: $(PROJECT)

$(PROJECT): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
