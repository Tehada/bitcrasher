#include <cmath>
#include <opencv2/opencv.hpp>

#include "functions.hpp"

using namespace cv;

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
