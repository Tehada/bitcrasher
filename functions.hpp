#include <opencv2/opencv.hpp>

struct Block_data {
    std::vector<double> value;
    std::vector<double> primal_rest_norm;
    std::vector<double> dual_rest_norm;
    std::vector<double> tolerance_primal;
    std::vector<double> tolerance_dual;
};

cv::Mat order_basis_functions(int block_size);

cv::Mat DCT_function(int p, int q, int size);

void Cholesky(cv::Mat& A, cv::Mat& S);

cv::Mat im2col(cv::Mat &input, int block_size);

cv::Mat col2im(cv::Mat &input, int block_size, int height, int width);

void ADMM(cv::Mat A, cv::Mat &x, cv::Mat b, double lagrangian_param, double relaxation_param, Block_data &data, int i, int total_blocks);
