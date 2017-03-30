//g++ -o a main.cpp `pkg-config --cflags --libs opencv`

#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "functions.hpp"

using namespace cv;

int main(int argc, char** argv) {
    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    int block_size = 64;

    /*resize to divide into 64x64 blocks*/
    Size new_size = image.size();
    //std::cout << new_size.height << " " << new_size.width << std::endl;
    new_size.width -= new_size.width % 64;
    new_size.height -= new_size.height % 64;
    resize(image, image, new_size, 0, 0, INTER_NEAREST);//check
    int height = image.rows, width = image.cols;

    /*create matrix path walk*/
    Mat order = order_basis_functions(block_size);

    Mat DCT_1d = Mat::zeros(block_size * block_size, block_size * block_size, CV_64F);
    Mat temp, reshaped;
    double p, q; //may be int???
    int index;

    for (int i = 0; i != std::pow(block_size, 2); ++i) {
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
    double lagrangian_param = 1, relaxation_param = 1;

    for (int i = 0; i != total_blocks; ++i) {
        b = oneD_version.col(i);
        ADMM(A, x, b, lagrangian_param, relaxation_param, data, i, total_blocks);
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
    imwrite(argv[2], result, compression_params);
}
