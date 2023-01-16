#include <cmath>
#include <opencv2/opencv.hpp>

#include "functions.hpp"

using namespace cv;

Mat order_basis_functions(int size) {
    int num_dia, number, k;
    //double number_previous_elements, number;
    int x, y;
    Mat result = Mat::zeros(size, size, CV_32S);

    for (k = 1; k != size + 1; ++k) {
        //number_previous_elements = (k - 1) * k / 2.0;
        number = (k - 1) * k / 2 + 1;
        if (k % 2 == 0) {
            for (x = 0; x != k; ++x) {
                y = k - x - 1;
                result.at<int>(x, y) = number;
                ++number;
            }
        } else {
            for (y = 0; y != k; ++y) {
                x = k - y - 1;
                result.at<int>(x, y) = number;
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
                ++number;
            }
        } else {
            for (y = size - 1; y != size - num_dia; --y) {
                x = k - y - 1;
                result.at<int>(x, y) = number;
                ++number;
            }
        }
    }
    return result;
}
