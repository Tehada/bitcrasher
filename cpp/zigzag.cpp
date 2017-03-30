#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

Mat zigzag_shervin(int size) {
    int num_dia, number, k;
    //double number_previous_elements, number;
    unsigned int x, y;
    Mat result = Mat::zeros(size, size, CV_32S);
    //Scalar value;

    for (k = 1; k != size + 1; ++k) {
        //number_previous_elements = (k - 1) * k / 2.0;
        number = (k - 1) * k / 2 + 1;
        if (k % 2 == 0) {
            for (x = 0; x != k; ++x) {
                y = k - x - 1;
                //std::cout << "first, number = " << number << ", x = " << x << ", y = " << y << std::endl;
                result.at<int>(x, y) = number;
                ++number;
            }
        } else {
            for (y = 0; y != k; ++y) {
                x = k - y - 1;
                result.at<int>(x, y) = number;
                //std::cout << "second, number = " << number << ", x = " << x << ", y = " << y << std::endl;
                ++number;
            }
        }
    }

    for (k = size + 1; k != 2 * size; ++k) {
        num_dia = 2 * size + 1 - k;
        //number_previous_elements = std::pow(size, 2) - (2 * size - k + 2) * (2 * size - k + 1) / 2.0;
        //number = number_previous_elements + 1;
        if (k % 2 == 1) {
            for (x = size - 1; x != size - num_dia; --x) {
                y = k - x - 1;
                result.at<int>(x, y) = number;
                //std::cout << "first, number = " << number << ", x = " << x << ", y = " << y << std::endl;
                ++number;
            }
        } else {
            for (y = size - 1; y != size - num_dia; --y) {
                x = k - y - 1;
                result.at<int>(x, y) = number;
                //std::cout << "second, number = " << number << ", x = " << x << ", y = " << y << std::endl;
                ++number;
            }
        }
    }

    return result;
}
