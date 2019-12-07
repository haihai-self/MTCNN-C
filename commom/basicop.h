//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_BASICOP_H
#define MTCNN_BASICOP_H

#include <cstring>

# define LENGTH 16

/**
 * 将十进制转换为二进制补码，结果保存在result中
 * @param num 输入十进制数
 * @param result 保存转换的二进制补码
 * @param length 数组长度
 */
void decToBin(int num, int *result, int length) {
    for (int i = length - 1; i >= 0; --i) {
        result[i] = num & 1;
        num >>= 1;
    }
}

/**
 * booth编码
 * @param X　需要编码的数
 * @param E 编码的结果
 */
void decoder3(const int *X, int *E) {
    int a1 = (X[0] & 1) ^(X[1] & 1);
    int a2 = (X[0] & 1) ^(X[2] & 1);
    E[3] = (~((a1 & 1) | (a2 & 1)) & 1);
    E[2] = (X[2] & 1) ^ (X[1] & 1);
    E[1] = (a1 & 1) & (a2 & 1);
    E[0] = X[0] & 1;
}

void decoder8(const int* X, int *E){
    int a1 = (X[3] ^ X[2]) & 1;
    int a2 = (X[1] ^ X[0]) & 1;
    int a3 = ~(X[3] | X[2]) & 1;
    int a4 = (X[3] & X[2]) & 1;

    E[3] = (a1 & ~a2) & 1;

    if(X[1] == 1)
        E[2] = a3;
    else
        E[2] = a4;

    E[1] = (a1 & a2) & 1;
    E[0] = ((~X[0] & X[1] & a4) | (X[0] & ~X[1] & a3)) & 1;
    E[4] = X[0] & 1;
}

/**
 *一位加法器
 * @param X 二进制0,1
 * @param Y 二进制0,1
 * @param C0 上一位的进位
 * @param sum 该位的结果
 * @param cout 给上一位的进位
 */
void bit1FA(const int X, const int Y, const int C0, int &sum, int &carry) {
    sum = (((X & 1) ^ (Y & 1)) ^ (C0 & 1)) & 1;
    carry = ((((X & 1) & (Y & 1)) | ((X & 1) & (C0 & 1))) | ((Y & 1) & (C0 & 1))) & 1;
}

void bit2FA(const int *X, const int Cin , int & sum, int & carry){
    sum = (Cin ^ (X[1] ^ X[2])) & 1;
    carry = X[0];
}

/**
 *
 * @param X　31位数据
 * @param Y　31位数
 * @param cin　上一位进位位
 * @param sum 计算的结果
 * @param carry 下一位的进位
 */
void nbitAdder(const int *X, const int *Y, const int cin, int n, int *sum, int &carry) {

    int *c = new int[n + 1];
    c[n] = cin;
    for (int i = n - 1; i >= 0; --i) {
        bit1FA(X[i], Y[i], c[i + 1], sum[i], c[i]);
    }
    carry = c[0];
}

void nbitAppAdder(const int * X, int * sum){
    int sel[16] = {0};
    for(int i = 15; i >=9; i-=2){
        if(i != 15)
            bit2FA(&X[i-2], X[i+ 1],sum[i], sum[i-1]);
        else
            bit2FA(&X[i-2], 0, sum[i], sum[i-1]);
        if(X[i-1] == 0){
            if(i!=15)
                sel[i] = X[i] & X[i + 1] & 1;
            else
                sel[i] = 0;
        }else{
            if(i !=  15)
                sel[i] = ~((X[i] | X[i+1]) & 1) & 1;
            else
                sel[i] = ~(X[i] & 1) & 1;
        }
    }
    int count = 0;
    for(int i = 0 ;i< 10; ++i)
        count+= sel[i];
    int cin = 0;
    if(count == 0)
        cin = X[8];
    else
        cin = X[7];
    nbitAdder(&X[1], X, cin, 7, &sum[1], sum[0]);
    sum[8] = (sum[9] ^ count) & 1;

}
/**
 * generate the sign of decoder,don't need jfm_dec
 * @param x 被乘数
 * @param y　乘数
 * @param Ypp　产生的partial product保存的结果
 */
void partialProduct(const int x, const int y, int  (*Ypp)[17], void (*decoder)(const int *X, int *E)) {
    int complementx[LENGTH] = {0};
    int complementy[LENGTH] = {0};
    decToBin(x, complementx, LENGTH);
    decToBin(y, complementy, LENGTH);
    int xtemp[17] = {0};
    int YP[18] = {0};
    for (int i = 0; i < LENGTH; ++i) {
        xtemp[i] = complementx[i];
    }
    xtemp[16] = 0;
    for (int i = 1; i < LENGTH + 1; ++i) {
        YP[i] = complementy[i - 1];
    }
    YP[0] = complementy[0];
    YP[LENGTH + 1] = 0;

    int jfm_dec[8][4] = {0};
    for (int i = 7; i >= 0; --i) {
        decoder3(&xtemp[2 * i], jfm_dec[7 - i]);
        for (int j = 17; j >= 1; --j) {
            Ypp[7 - i][j - 1] = int(
                    (((jfm_dec[7 - i][2] & 1) & (YP[j - 1]) & 1) | ((jfm_dec[7 - i][1] & 1) & (YP[j] & 1))) ^
                    (jfm_dec[7 - i][0] & 1));
        }
    }
}


/**
 * generate the sign of decoder, need jfm_dec
 * @param x 被乘数
 * @param y 乘数
 * @param Ypp partial product
 * @param jfm_dec decode
 */
void partialProduct(const int x, const int y, int (*Ypp)[17], int (*jfm_dec)[4], void (*decoder)(const int *X, int *E)) {
    int complementx[LENGTH] = {0};
    int complementy[LENGTH] = {0};
    decToBin(x, complementx, LENGTH);
    decToBin(y, complementy, LENGTH);
    int xtemp[17] = {0};
    int YP[18] = {0};
    for (int i = 0; i < LENGTH; ++i) {
        xtemp[i] = complementx[i];
    }
    xtemp[16] = 0;
    for (int i = 1; i < LENGTH + 1; ++i) {
        YP[i] = complementy[i - 1];
    }
    YP[0] = complementy[0];
    YP[LENGTH + 1] = 0;

    for (int i = 7; i >= 0; --i) {
        decoder(&xtemp[2 * i], jfm_dec[7 - i]);
        for (int j = 17; j >= 1; --j) {
            Ypp[7 - i][j - 1] = int(
                    (((jfm_dec[7 - i][2] & 1) & (YP[j - 1]) & 1) | ((jfm_dec[7 - i][1] & 1) & (YP[j] & 1))) ^
                    (jfm_dec[7 - i][0] & 1));
        }
    }
}

void partialProcudtAbm2(int x, int y, int (*Ypp)[18], int (*dec)[5]){
    int complementx[LENGTH] = {0};
    int complementy[LENGTH] = {0};
    decToBin(x, complementx, LENGTH);
    decToBin(y, complementy, LENGTH);

    int Ytemp[20] = {0};
    int Xtemp[19] = {0};
    int YP3[18] = {0};

    Ytemp[0] = complementy[0];
    Ytemp[1] = complementy[0];
    for(int i =0; i < 16; ++i)
        Ytemp[2+i] = complementy[i];

    Xtemp[0] = complementx[0];
    Xtemp[1] = complementx[0];
    for(int i =0; i < 16; ++i)
        Xtemp[2+i] = complementx[i];

    int Y3[16] = {0};
    nbitAppAdder(complementy, Y3);
    YP3[0] = complementy[0];
    YP3[17] = complementy[15];
    for(int i =0; i< 16; ++i)
        YP3[1 + i] = Y3[i];
    for(int i = 5; i >= 0; --i){
        decoder8(&Xtemp[i*3], dec[5-i]);
        for(int j = 19; j >= 2; --j)
            Ypp[5 - i][j - 2] = (((dec[5 - i][3] & Ytemp[j - 2]) | (dec[5 - i][2] & Ytemp[j -1]) | (dec[5 - i][1] & YP3[j - 2]) | (dec[5 - i][0] & Ytemp[j])) ^ dec[5 - i][4]) & 1;
    }
}


/**
 * bm07 pebm可以通用
 * @param Ypp 生成的partial product
 * @param X_ 保存中间结果
 * @param Y_ 保存中间结果
 * @param sum_pr 保存中间结果
 * @param carry 保存中间结果的carry
 * @param length 产生的partial product 的最大长度
 */
void sumPartialProduct(int (*Ypp)[17], int *X_, int *Y_, int (*sum_pr)[32], int *carry, int length) {
    using std::fill;


    Y_[12] = 1;
    Y_[13] = ~(Ypp[1][0] & 1) & 1;
    for (int i = 1; i < 3; ++i)
        Y_[13 + i] = Ypp[1][i];
    nbitAdder(X_, Y_, 0, length, sum_pr[0], carry[0]);

    fill(X_, X_ + length, 0);
    fill(Y_, Y_ + length, 0);
    Y_[10] = 1;
    Y_[11] = ~(Ypp[2][0] & 1) & 1;
    for (int i = 1; i < 5; ++i) {
        Y_[11 + i] = Ypp[2][i];
    }
    nbitAdder(sum_pr[0], Y_, 0, length, sum_pr[1], carry[1]);

    fill(Y_, Y_ + length, 0);
    Y_[8] = 1;
    Y_[9] = ~(Ypp[3][0] & 1) & 1;
    for (int i = 1; i < 7; ++i) {
        Y_[9 + i] = Ypp[3][i];
    }
    nbitAdder(sum_pr[1], Y_, 0, length, sum_pr[0], carry[0]);

    fill(Y_, Y_ + length, 0);
    Y_[6] = 1;
    Y_[7] = ~(Ypp[4][0] & 1) & 1;
    for (int i = 1; i < 9; ++i) {
        Y_[7 + i] = Ypp[4][i];
    }
    nbitAdder(sum_pr[0], Y_, 0, length, sum_pr[1], carry[1]);

    fill(Y_, Y_ + length, 0);
    Y_[4] = 1;
    Y_[5] = ~(Ypp[5][0] & 1) & 1;
    for (int i = 1; i < 11; ++i) {
        Y_[5 + i] = Ypp[5][i];
    }
    nbitAdder(sum_pr[1], Y_, 0, length, sum_pr[0], carry[0]);

    fill(Y_, Y_ + length, 0);
    Y_[2] = 1;
    Y_[3] = ~(Ypp[6][0] & 1) & 1;
    for (int i = 1; i < 13; ++i) {
        Y_[3 + i] = Ypp[6][i];
    }
    nbitAdder(sum_pr[0], Y_, 0, length, sum_pr[1], carry[1]);

    fill(Y_, Y_ + length, 0);
    Y_[0] = 1;
    Y_[1] = ~(Ypp[7][0] & 1) & 1;
    for (int i = 1; i < 15; ++i) {
        Y_[1 + i] = Ypp[7][i];
    }
    nbitAdder(sum_pr[1], Y_, 0, length, sum_pr[0], carry[0]);

}

#endif //MTCNN_BASICOP_H
