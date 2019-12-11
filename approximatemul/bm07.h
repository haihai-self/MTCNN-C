//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_BM07_H
#define MTCNN_BM07_H

#include "../commom/basicop.h"
#include <iostream>
#include <cstring>

/**
 * bm07算法
 * @param x 被乘数
 * @param y 乘数
 * @return 使用bm07算法计算出的结果
 */
int bm07(int x, int y){
    using std::fill;
    int Ypp[8][17] = {0};
    partialProduct(x, y, Ypp);

    int ec = Ypp[0][2] + Ypp[1][4] + Ypp[2][6] + Ypp[3][8] + Ypp[4][10] + Ypp[5][12] + Ypp[6][14] + Ypp[7][16];
    int c0 = 0;
    if(ec == 0)
        c0 = 1;
    else
        c0 = 0;
    int cc = int((c0 + ec + Ypp[0][1] + Ypp[1][3] + Ypp[2][5] + Ypp[3][7] + Ypp[4][9] + Ypp[5][11] + Ypp[6][13] + Ypp[7][15]) / 2);
    int complement_cc[4] = {0};
    decToBin(cc, complement_cc, 4);

    int sum_pr[2][32] = {0};
    int carry[2] = {0};
    int X_[32] = {0};
    int Y_[32] = {0};

    X_[14] = 1;
    X_[15] = ~(Ypp[0][0] & 1) & 1;

    sumPartialProduct(Ypp,X_,Y_,sum_pr,carry,32);

    fill(Y_, Y_ + 32, 0);
    Y_[13] = complement_cc[1];
    nbitAdder(sum_pr[0], Y_, 0, 32, sum_pr[1], &carry[1]);

    fill(Y_, Y_ + 32, 0);
    Y_[14] = complement_cc[2];
    nbitAdder(sum_pr[1], Y_, 0, 32, sum_pr[0], &carry[0]);

    fill(Y_, Y_ + 32, 0);
    Y_[15] = complement_cc[3];
    nbitAdder(sum_pr[0], Y_, 0, 32, sum_pr[1], &carry[1]);

    fill(Y_, Y_ + 32, 0);
    Y_[15] = 1;
    nbitAdder(sum_pr[1], Y_, 0, 32, sum_pr[0], &carry[0]);

    fill(Y_, Y_ + 32, 0);
    Y_[15] = complement_cc[0];
    nbitAdder(sum_pr[0], Y_, 0, 32, sum_pr[1], &carry[1]);

    int mul[32] = {0};
    memcpy(mul, sum_pr[1], 32 * sizeof(int));
    int result = 0;
    result = -1 * (1 << 31) * mul[0];
    for (int i = 1; i < 16; ++i)
        result += (1 << (31 - i)) * mul[i];
    return result;
}
#endif //MTCNN_BM07_H
