//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_BBM_H
#define MTCNN_BBM_H
#include "../commom/basicop.h"
#include <iostream>
#include <cstring>



/**
 * bbm algorithm
 * @param x Multiplicand
 * @param y multiplier
 * @return Results calculated using bbm algorithm
 */
int bbm(int x, int y){
    int Ypp[8][17] = {0};
    partialProduct(x, y, Ypp);

    int sum_pr[2][31] = {0};
    int carry[2] = {0};
    int X_[31] = {0};
    int Y_[31] = {0};

    X_[12] = ~(Ypp[0][0] & 1) & 1;
    X_[13] = Ypp[0][0];
    X_[14] = Ypp[0][0];
    X_[15] = Ypp[0][1];

    Y_[11] = 1;
    Y_[12] = ~(Ypp[1][0] & 1) & 1;
    for(int i =1; i < 4; ++ i)
        Y_[12 + i] = Ypp[1][i];
    nbitAdder(X_, Y_, 0, 31, sum_pr[0], &carry[0]);

    fillZero(X_, 31, 0);
    fillZero(Y_, 31, 0);
    Y_[9] = 1;
    Y_[10] = ~(Ypp[2][0] & 1) & 1;
    for(int i =1; i< 6; ++i)
        Y_[i + 10] = Ypp[2][i];
    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);

    fillZero(Y_, 31, 0);
    Y_[7] = 1;
    Y_[8] = ~(Ypp[3][0] & 1) & 1;
    for(int i = 1; i< 8; ++i)
        Y_[i+ 8] = Ypp[3][i];
    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);

    fillZero(Y_, 31, 0);
    Y_[5] = 1;
    Y_[6] = ~(Ypp[4][0] & 1) & 1;
    for(int i = 1; i < 10; ++i)
        Y_[i + 6] = Ypp[4][i];

    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);

    fillZero(Y_, 31, 0);
    Y_[3] = 1;
    Y_[4] = ~(Ypp[5][0] & 1) & 1;
    for(int i =1; i < 12 ; ++i)
        Y_[i + 4] = Ypp[5][i];
    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);

    fillZero(Y_, 31, 0);
    Y_[1] = 1;
    Y_[2] = ~(Ypp[6][0] & 1) & 1;
    for(int i =1; i< 14; ++i)
        Y_[i + 2] = Ypp[6][i];
    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);


    fillZero(Y_, 31, 0);
    Y_[0] = ~(Ypp[6][0] & 1) & 1;
    for(int i = 1; i< 16; ++ i)
        Y_[i] = Ypp[7][i];

    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);

    int mul[31] = {0};
    memcpy(mul, sum_pr[0], 31 * sizeof(int));
    int result = 0;
    result = -1 * (1 << 30) * mul[0];
    for (int i = 1; i < 16; ++i)
        result += (1 << (30 - i)) * mul[i];
    return result;
}

/**
 *
 * @param input_val Multiplicand
 * @param kernel_val multiplier
 * @param frac_width_input decimal places for input_val
 * @param frac_width_kernel decimal places for kernel_val
 * @return
 */
float appMultiplierBBM(const float input_val, const float kernel_val, const int frac_width_input, const int frac_width_kernel){
    int input_t = int(input_val * pow2(frac_width_input));
    int kernel_t = int(kernel_val * pow2(frac_width_kernel));
    int result = bbm(input_t, kernel_t);
    return float(result) / (pow2(frac_width_input) * pow2(frac_width_kernel));
}
#endif //MTCNN_BBM_H
