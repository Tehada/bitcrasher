#include <cmath>
#include <opencv2/opencv.hpp>

#include "functions.hpp"

using namespace cv;

Mat DCT_function(int p, int q, int size) {
    const double pi = std::acos(-1);
    double alpha_p, alpha_q;
    int m, n;
    Mat result = Mat::zeros(size, size, CV_64F);

    for (m = 0; m != size; ++m) {
        for (n = 0; n != size; ++n){
            result.at<double>(m, n) =
            std::cos(pi * (2 * m + 1) * p / (2 * (double)size)) *
            std::cos(pi * (2 * n + 1) * q / (2 * (double)size));
        }
    }

    p == 0 ? alpha_p = std::sqrt(1 / (double)size) : alpha_p = std::sqrt(2 / (double)size);
    q == 0 ? alpha_q = std::sqrt(1 / (double)size) : alpha_q = std::sqrt(2 / (double)size);

    result *= alpha_q * alpha_p;
    return result;
}
