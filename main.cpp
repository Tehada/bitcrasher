//g++ -o a main.cpp `pkg-config --cflags --libs opencv`

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "functions.h"
#include <cmath>
#include <cstdio>

using namespace cv;

struct Block_data {
    std::vector<double> objval;
    std::vector<double> r_norm;
    std::vector<double> s_norm;
    std::vector<double> eps_pri;
    std::vector<double> eps_dual;
};

Mat order_basis_functions(int size) {
    int num_dia, number, k;
    //double number_previous_elements, number;
    unsigned int x, y;
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

/*reverse to im2col*/
Mat col2im(Mat &input, int block_size, int height, int width) {
    int size = input.rows * input.cols, column = 0;
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

Mat take_max(Mat a, double b) {
    return max(0, a - b) - max(0, -a - b);
}

/* Calculates upper triangular matrix S, where A is a symmetrical matrix A=S'*S */
void Cholesky(Mat& A, Mat& S) { // change to return Mat!
    int dim = A.rows;
    S.create(dim, dim, CV_64F);

    int i, j, k;

    for( i = 0; i < dim; i++ )
    {
        for( j = 0; j < i; j++ )
            S.at<double>(i,j) = 0.;

        double sum = 0.;
        for( k = 0; k < i; k++ )
        {
            double val = S.at<double>(k,i);
            sum += val*val;
        }

        S.at<double>(i,i) = std::sqrt(std::max(A.at<double>(i,i) - sum, 0.));
        double ival = 1./S.at<double>(i, i);

        for( j = i + 1; j < dim; j++ )
        {
            sum = 0;
            for( k = 0; k < i; k++ )
                sum += S.at<double>(k, i) * S.at<double>(k, j);

            S.at<double>(i, j) = (A.at<double>(i, j) - sum)*ival;
        }
    }
}

void ADMM(Mat A, Mat &x, Mat b, double rho, double alpha, Block_data &data) {
    Mat Ax_hat, R, zold, Rtr;
    Mat Atr = Mat::zeros(A.cols, A.rows, CV_64F);
    double ABSTOL, RELTOL;
    int m, n;

    int iters = 200;
    //ABSTOL = 1e-4;
    ABSTOL = std::pow(10, -4);
    //RELTOL = 1e-2;//change
    RELTOL = std::pow(10, -2);
    m = A.rows;//check docs!
    n = A.cols;

    data.objval.reserve(iters);
    data.r_norm.reserve(iters);
    data.s_norm.reserve(iters);
    data.eps_pri.reserve(iters);
    data.eps_dual.reserve(iters);

    Mat x1 = Mat::zeros(n, 1, CV_64F);
    Mat z = Mat::zeros(m, 1, CV_64F); //64??? check shape on reflect!!
    Mat u = Mat::zeros(m, 1, CV_64F);

    //if (not QUIET) {
    //    std::printf("%3s\t%10s\t%10s\t%10s\t%10s\t%10s\n", "iter",
    //      "r norm", "eps pri", "s norm", "eps dual", "objective") ;
    //}
    transpose(A, Atr);
    transpose(R, Rtr);
    Cholesky(Atr * A, R);

    for (int k = 0; k != iters; ++k) {
        solve(Rtr, Atr * (b + z - u), x1);
        solve(R, x1, x);

        zold = z;
        Ax_hat = alpha * A * x + (1 - alpha) * (zold + b);
        z = shrinkage(Ax_hat - b + u, 1. / rho);
        u = u + (Ax_hat - z - b);

        data.objval[k] = norm(z, NORM_L1, noArray());
        data.r_norm[k] = norm(A * x - z - b, NORM_L2, noArray());

        data.s_norm[k] = norm(-rho * Atr * (z - zold), NORM_L2, noArray());
        data.eps_pri[k] = std::sqrt(m) * ABSTOL + RELTOL * std::max(norm(A * x, NORM_L2, noArray()),
                                                                           std::max(norm(-z, NORM_L2, noArray()),
                                                                                    norm(b, NORM_L2, noArray())));
        data.eps_dual[k] = std::sqrt(n) * ABSTOL + RELTOL * norm(rho * Atr * u, NORM_L2, noArray());

        std::cout << data.r_norm[k] << "\t"
                  << data.eps_pri[k] << "\t"
                  << data.s_norm[k] << "\t"
                  << data.eps_dual[k] << "\t"
                  << data.objval[k] << "\n";

        if (history.r_norm[k] < history.eps_pri[k] &&
            history.s_norm[k] < history.eps_dual[k]) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    int block_size = 64;

    /*resize to divide into 64x64 blocks*/
    Size new_size = image.size();
    //std::cout << new_size.height << " " << new_size.width << std::endl;
    int height = new_size.height, width = new_size.width;
    new_size.width -= new_size.width % 64;
    new_size.height -= new_size.height % 64;
    resize(image, image, new_size, 0, 0, INTER_NEAREST);//check

    /*create matrix path walk*/
    Mat order = order_basis_functions(block_size);

    Mat DCT_1d = Mat::zeros(block_size * block_size, block_size * block_size, CV_64F);
    Mat temp, reshaped;
    double p, q; //may be int???
    int index, k;

    for (i = 0; i != std::pow(block_size, 2); ++i) {
        q = std::floor((double)i / block_size);
        p = i - q * block_size;

        temp = DCT_function(p, q, block_size);
        temp = temp.t();
        reshaped = temp.reshape(0, block_size * block_size);
        index = order.at<int>(p, q) - 1;
        reshaped.col(0).copyTo(DCT_1d.col(index)); // first row necessary?
    }

    int M = 10;
    Mat A = DCT_1d(Range::all(), Range(0, 10));// problem!!!!!

    Mat oneD_version = im2col(image, block_size);

    //s1 = oneD_version.rows;
    //s2 = oneD_version.cols;

    int total_blocks = image.rows * image.cols / (std::pow(block_size, 2));

    Mat Rec_image = Mat::zeros(image.size(), CV_64F);
    Mat err_image = Mat::zeros(image.size(), CV_64F);

    Mat one_D_rec = Mat::zeros(oneD_version.size(), CV_64F);
    Mat one_D_error = Mat::zeros(oneD_version.size(), CV_64F);

    Mat alpha_whole = Mat::zeros(M, total_blocks, CV_64F); //?? che eta

    Mat b = Mat::zeros(std::pow(block_size, 2), 1, CV_64F);
    //int curr_iter = 0;
    Mat x = Mat::zeros(A.cols, 1, CV_64F);
    Block_data data;
    double rho = 1, alpha = 1;

    for (int i = 0; i != total_blocks; ++i) {
        b = oneD_version.col(i);
        ADMM(A, x, b, rho, alpha, data);
        one_D_rec.col(i) = A * x;
        one_D_error.col(i) = b - A * x;
        //++curr_iter;
        //std::cout << i << "\n";
    }

    Rec_image = col2im(one_D_rec, block_size, height, width);
    err_image = col2im(one_D_error, block_size, height, width);

    /*threshold result and save*/
    double thresh = 14, maxval = 255; //unsigned??
    Mat map = abs(err_image), result;
    map.convertTo(map, CV_32F);
    threshold(map, result, thresh, maxval, THRESH_BINARY);

    std::cout << "map size: " << map.rows << " " << map.cols << std::endl;

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PXM_BINARY);
    imwrite("out.pbm", result, compression_params);
}
