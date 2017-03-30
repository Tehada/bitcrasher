//g++ -o a main.cpp `pkg-config --cflags --libs opencv`

//#include "mconvert.h"
//#include <cmath>
//#include <armadillo>
//#include <cv.h>
//#include <highgui.h>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "functions.h"
using namespace cv;

int main(int argc, char** argv) {
    /*
    TYPE A, B, DCT_Bases, X, history, ind_check, input_image, input_image_rgb, oneD_version, tot_time, total_time, x_estimate, zig_zag ;
    double num_blocks;
    int M, N, curr_iter, i, inlier_threshold, k, map, s1, s2, sh, sv ;
    mat DCT_1d, P, Rec_image, alpha_whole, err_image, one_D_error, one_D_rec ;
    rowvec _aux_rowvec_1, _aux_rowvec_2, _aux_rowvec_3, _aux_rowvec_4, _aux_rowvec_5 ;
    */

    char* imageName = argv[1];

    Mat image = imread(imageName, 1);

    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PXM_BINARY);

    std::cout << "all right\n";

    Size new_size = image.size();
    new_size.width -= new_size.width % 64;
    new_size.height -= new_size.height % 64;
    resize(image, image, new_size, 0 ,0, INTER_NEAREST);//check

    imwrite("NEW.pgm", image, compression_params);


    //input_image = double(A) ; // expand!!
    //unsigned int sh = input_image.rows(); // potential bug
    //unsigned int sv = input_image.columns();

    int size = 64;
    Mat zig_zag = zigzag_shervin(size);
    //std::cout << "matrix\n\n" << zig_zag << "\n";

    Mat DCT_1d = Mat::zeros(size * size, size * size, CV_64F);
    Mat temp, reshaped;
    double p, q; //may be int???
    int index, k;

    for (k = 0; k != size * size - 1; ++k) {
        q = std::floor((double)k / size);
        p = k - q * size;

        //DCT_Bases(m2cpp::span<uvec>(0, DCT_Bases.n_rows-1), m2cpp::span<uvec>(0, DCT_Bases.n_cols - 1), k + 1) =
        //shervin_2dctillus(p, q, size);

        temp = cosin(p, q, size);
        reshaped = temp.reshape(0, size * size);
        //std::cout << reshaped.rows << " " << reshaped.cols << std::endl;
        index = zig_zag.at<int>(p, q) - 1;
        //std::cout << index << std::endl;
        reshaped.col(0).copyTo(DCT_1d.col(index)); // first row necessary?

        //DCT_1d(m2cpp::span<uvec>(0, DCT_1d.n_rows - 1) - 1, zig_zag(p + 1, q + 1) - 1) =
        //reshape(DCT_Bases(m2cpp::span<uvec>(0, DCT_Bases.n_rows - 1),
        //    m2cpp::span<uvec>(0, DCT_Bases.n_cols -1 ), k + 1), size * size, 1);

        //ind_check(k + 1) = zig_zag(p + 1, q + 1);
    }

    std::cout << "\n\n" << DCT_1d << std::endl;
/*
    M = 10;
    P = DCT_1d.cols(arma::span(0, M - 1)) ;
    double __aux_rowvec_1 [] = {size, size} ;
    _aux_rowvec_1 = rowvec(__aux_rowvec_1, 2, false) ;
    oneD_version = im2col(input_image, _aux_rowvec_1, "distinct") ;
    s1 = oneD_version.n_rows;
    s2 = oneD_version.n_cols;
    num_blocks = sh * sv / (pow(size, 2)) ;

    Rec_image = arma::zeros<mat>(size(input_image)) ;
    err_image = arma::zeros<mat>(size(input_image)) ;
    one_D_rec = arma::zeros<mat>(size(oneD_version)) ;
    one_D_error = arma::zeros<mat>(size(oneD_version)) ;
    alpha_whole = arma::zeros<mat>(M, num_blocks) ;

    Mat Rec_image = Mat::zeros(input_image.size());
    Mat err_image = Mat::zeros(input_image.szie());
    Mat one_D_rec = Mat::zeros(oneD_version.size());
    Mat one_D_error = Mat::zeros(oneD_version.size());
    Mat alpha_whole = Mat::zeros(M, num_blocks); //?? che eta

    Mat x = Mat::zeros(n, 1, CV_64F);
    size_t curr_iter = 0;

    size_t i;
    for (i = 1; i != num_blocks; ++i) {
        X = oneD_version.col(i - 1);
        [x_estimate, history] = lad_admm(P, X, 1, 1);
        one_D_rec.col(i - 1) = P * x_estimate;
        one_D_error.col(i-1) = X - P * x_estimate;
        curr_iter = curr_iter + 1;
    }

    double __aux_rowvec_2 [] = {N, N} ;
    _aux_rowvec_2 = rowvec(__aux_rowvec_2, 2, false) ;
    double __aux_rowvec_3 [] = {sh, sv} ;
    _aux_rowvec_3 = rowvec(__aux_rowvec_3, 2, false) ;
    Rec_image = col2im(one_D_rec, _aux_rowvec_2, _aux_rowvec_3, "distinct") ;
    double __aux_rowvec_4 [] = {N, N} ;
    _aux_rowvec_4 = rowvec(__aux_rowvec_4, 2, false) ;
    double __aux_rowvec_5 [] = {sh, sv} ;
    _aux_rowvec_5 = rowvec(__aux_rowvec_5, 2, false) ;
    err_image = col2im(one_D_error, _aux_rowvec_4, _aux_rowvec_5, "distinct") ;
    tot_time = _timer.toc() ;


    inlier_threshold = 14 ;
    map = abs(err_image) ;
    map(map<inlier_threshold) = 0 ;
    map(map>=inlier_threshold) = 255 ;
    imwrite('out.pbm') ;
    */
    return 0;
}
