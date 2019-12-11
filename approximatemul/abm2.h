//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_ABM2_H
#define MTCNN_ABM2_H

#include <iostream>
#include "../commom/basicop.h"

int abm2(int x, int y) {
    using std::fill;
    int Ypp[6][18] = {0};
    int dec[6][5] = {0};
    partialProcudtAbm2(x, y, Ypp, dec);
    int sum_pr[2][31] = {0};
    int carry[2] = {0};
    int X_[31] = {0};
    int Y_[31] = {0};

    X_[10] = ~Ypp[0][0] & 1;
    X_[11] = Ypp[0][0];
    X_[12] = Ypp[0][0];
    for (int i = 0; i < 3; ++i)
        X_[13 + i] = Ypp[0][i];

    Y_[8] = 1;
    Y_[9] = 1;
    Y_[10] = ~Ypp[1][0] & 1;
    for (int i = 1; i < 6; ++i)
        Y_[10 + i] = Ypp[1][i];
    nbitAdder(X_, Y_, 0, 31, sum_pr[0], &carry[0]);

    fill(X_, X_ + 31, 0);
    fill(Y_, Y_ + 31, 0);
    Y_[5] = 1;
    Y_[6] = 1;
    Y_[7] = ~Ypp[2][0] & 1;
    for (int i = 1; i < 9; ++i)
        Y_[7 + i] = Ypp[2][i];
    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);

    fill(Y_, Y_ + 31, 0);
    Y_[2] = 1;
    Y_[3] = 1;
    Y_[4] = ~Ypp[3][0] & 1;
    for (int i = 1; i < 12; ++i)
        Y_[4 + i] = Ypp[3][i];
    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);


    fill(Y_, Y_ + 31, 0);
    Y_[0] = 1;
    Y_[1] = ~Ypp[4][0] & 1;
    for (int i = 1; i < 15; ++i)
        Y_[1 + i] = Ypp[4][i];
    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);

    fill(Y_, Y_ + 31, 0);
    for (int i = 0; i < 16; ++i)
        Y_[i] = Ypp[5][i + 2];
    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);

    fill(Y_, Y_ + 31, 0);
    Y_[15] = dec[5][4];
    nbitAdder(sum_pr[0], Y_, 0, 31, sum_pr[1], &carry[1]);

    fill(Y_, Y_ + 31, 0);
    Y_[14] = 1;
    nbitAdder(sum_pr[1], Y_, 0, 31, sum_pr[0], &carry[0]);

    int mul[31] = {0};
    memcpy(mul, sum_pr[0], 31 * sizeof(int));
    int result = 0;
    result = -1 * (1 << 30) * mul[0];
    for (int i = 1; i < 31; ++i)
        result += (1 << (30 - i)) * mul[i];
    return result;

}

#endif //MTCNN_ABM2_H
