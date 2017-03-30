#include <cmath>
#include <opencv2/opencv.hpp>

#include "functions.hpp"

using namespace cv;

/*converts each block into column*/
Mat im2col(Mat &input, int block_size) {
    int size = input.rows * input.cols, column = 0;
    Mat result = Mat::zeros(std::pow(block_size, 2), size / std::pow(block_size, 2),CV_64F);

    for (int x = 0; x != input.cols; x += block_size) {
        for (int y = 0; y != input.rows; y += block_size) {
            for (int i = 0; i != block_size; ++i) {
                input(Range(y, y + block_size),
                      Range(x + i, x + i + 1)).copyTo(result(Range(i * 64, i * 64 + block_size),
                                                             Range(column, column + 1)));
            }
            ++column;
        }
    }
    return result;
}

/*vice versa to im2col*/
Mat col2im(Mat &input, int block_size, int height, int width) {
    int column = 0;
    Mat result = Mat::zeros(height, width, CV_64F);

    for (int x = 0; x != width; x += block_size) {
        for (int y = 0; y != height; y += block_size) {
            for (int i = 0; i != block_size; ++i) {
                input(Range(i * 64, i * 64 + block_size),
                      Range(column, column + 1)).copyTo(result(Range(y, y + block_size),
                                                               Range(x + i, x + i + 1)));
            }
            ++column;
        }
    }
    return result;
}
