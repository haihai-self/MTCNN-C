//
// Created by mo on 2019/12/7.
// Github: https://github.com/mo-hai
// Email: moh19@mails.tsinghua.edu.cn
//

#ifndef MTCNN_PEBM_H
#define MTCNN_PEBM_H


#include "../commom/basicop.h"
#include <iostream>
#include <cstring>

int pebm(int x, int y){
    using std::fill;
    int Ypp[8][17] = {0};
    int jfm_dec[8][4] = {0};
    partialProduct(x, y, Ypp, jfm_dec);

    int s2 = ((~(Ypp[0][0]) & 1) | ((jfm_dec[7][0] & Ypp[7][16]) & 1)) & 1;
    int s1 = ((~(jfm_dec[7][0] & Ypp[7][16]) & 1) & Ypp[0][0]) & 1;
    int s0 = (((jfm_dec[7][0] & Ypp[7][16]) & (~(Ypp[0][0]) & 1)) | s1) & 1;
    int lamda = ~(jfm_dec[7][0] & Ypp[7][16]) & 1;
    int e7 = (jfm_dec[7][0] ^ Ypp[7][16]) & 1;
    int jfm_sum = 0;
    for(int i = 0; i < 7; ++i){
        int temp = ~jfm_dec[i][4] & 1;
        jfm_sum += temp;
    }
    int c0 = int(float(jfm_sum + e7) / 2);
    int c1 = int(float(c0 + Ypp[0][1] + Ypp[1][3] + Ypp[2][5] + Ypp[3][7] + Ypp[4][9] + Ypp[5][11] + Ypp[6][13] + Ypp[7][15] + lamda) / 2);

    int complement_c1[3] = {0};
    decToBin(c1, complement_c1, 3);





}
#endif //MTCNN_PEBM_H
