//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_BASICOP_H
#define MTCNN_BASICOP_H


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
void decoder(const int *X, int *E) {
    int a1 = (X[0] & 1) ^(X[1] & 1);
    int a2 = (X[0] & 1) ^(X[2] & 1);
    E[3] = (~((a1 & 1) | (a2 & 1)) & 1);
    E[2] = (X[2] & 1) ^ (X[1] & 1);
    E[1] = (a1 & 1) & (a2 & 1);
    E[0] = X[0] & 1;
}

/**
 *一位加法器
 * @param X 二进制0,1
 * @param Y 二进制0,1
 * @param C0 上一位的进位
 * @param sum 该位的结果
 * @param cout 给上一位的进位
 */
void bit1FA(const int X, const int Y, const int C0, int &sum, int &cout) {
    sum = (((X & 1) ^ (Y & 1)) ^ (C0 & 1)) & 1;
    cout = ((((X & 1) & (Y & 1)) | ((X & 1) & (C0 & 1))) | ((Y & 1) & (C0 & 1))) & 1;
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

    int * c = new int[n + 1];
    c[n] = cin;
    for (int i = n -1; i >= 0; --i) {
        bit1FA(X[i], Y[i], c[i + 1], sum[i], c[i]);
    }
    carry = c[0];
}

/**
 * generate the sign of partial products,don't need jfm_dec
 * @param x 被乘数
 * @param y　乘数
 * @param Ypp　产生的partial product保存的结果
 */
void partialProduct(const int x, const int y, int  (*Ypp)[17]) {
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
        decoder(&xtemp[2 * i], jfm_dec[7 - i]);
        for (int j = 17; j >= 1; --j) {
            Ypp[7 - i][j - 1] = int(
                    (((jfm_dec[7 - i][2] & 1) & (YP[j - 1]) & 1) | ((jfm_dec[7 - i][1] & 1) & (YP[j] & 1))) ^
                    (jfm_dec[7 - i][0] & 1));
        }
    }
}


/**
 * generate the sign of partial products, need jfm_dec
 * @param x 被乘数
 * @param y 乘数
 * @param Ypp partial product
 * @param jfm_dec decode
 */
void partialProduct(const int x, const int y, int (*Ypp)[17], int (*jfm_dec)[4]) {
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
#endif //MTCNN_BASICOP_H#ifndef SIGNEDM_BASICOP_H
#define SIGNEDM_BASICOP_H
