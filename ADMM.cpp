#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "functions.hpp"

using namespace cv;

Mat take_max(Mat a, double b) {
    return max(0, a - b) - max(0, -a - b);
}

void ADMM(Mat A, Mat &x, Mat b, double lagrangian_param, double relaxation_param, Block_data &data, int i, int total_blocks) {
    Mat D, R, w, Rtr;
    Mat Atr = Mat::zeros(A.cols, A.rows, CV_64F);
    double ABSTOL, RELTOL;
    int m, n;

    int iters = 200;
    ABSTOL = 1e-4;
    //ABSTOL = std::pow(10, -4);
    RELTOL = 1e-2;//change
    //RELTOL = std::pow(10, -2);
    m = A.rows;//check docs!
    n = A.cols;

    data.value.reserve(iters);
    data.primal_rest_norm.reserve(iters);
    data.dual_rest_norm.reserve(iters);
    data.tolerance_primal.reserve(iters);
    data.tolerance_dual.reserve(iters);

    Mat x1 = Mat::zeros(n, 1, CV_64F);
    Mat z = Mat::zeros(m, 1, CV_64F); //64??? check shape on reflect!!
    Mat u = Mat::zeros(m, 1, CV_64F);

    //if (not QUIET) {
    //    std::printf("%3s\t%10s\t%10s\t%10s\t%10s\t%10s\n", "iter",
    //      "r norm", "eps pri", "s norm", "eps dual", "objective") ;
    //}
    transpose(A, Atr);
    Mat temp = Atr * A;
    Cholesky(temp, R);
    transpose(R, Rtr);
    for (int k = 0; k != iters; ++k) {
        
        solve(Rtr, Atr * (b + z - u), x1);
        solve(R, x1, x);

        w = z;
        D = relaxation_param * A * x + (1 - relaxation_param) * (w + b);
        z = take_max(D - b + u, 1. / lagrangian_param);
        u = u + (D - z - b);

        data.value[k] = norm(z, NORM_L1, noArray());
        data.primal_rest_norm[k] = norm(A * x - z - b, NORM_L2, noArray());

        data.dual_rest_norm[k] = norm(-lagrangian_param * Atr * (z - w), NORM_L2, noArray());
        data.tolerance_primal[k] = std::sqrt(m) * ABSTOL + RELTOL * std::max(norm(A * x, NORM_L2, noArray()),
                                                                           std::max(norm(-z, NORM_L2, noArray()),
                                                                                    norm(b, NORM_L2, noArray())));
        data.tolerance_dual[k] = std::sqrt(n) * ABSTOL + RELTOL * norm(lagrangian_param * Atr * u, NORM_L2, noArray());
        std::cout << "block " << i << "/" << total_blocks << "\t"
                  //<< "iter " << k << "/" << iters << "\t"
                  << data.primal_rest_norm[k] << "\t"
                  << data.tolerance_primal[k] << "\t"
                  << data.dual_rest_norm[k] << "\t"
                  << data.tolerance_dual[k] << "\t"
                  << data.value[k] << std::endl;

        if (data.primal_rest_norm[k] < data.tolerance_primal[k] &&
            data.dual_rest_norm[k] < data.tolerance_dual[k]) {
            break;
        }
    }
}
