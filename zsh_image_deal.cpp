/*
zsh_image_deal.c
code by zsh
date : 2022/07/13
*/
#include "zsh_construction.h"

using namespace std;
using namespace cv;

uint redPixle[Row][Col];// no use

uint8_t zsh_get_red_mask_pixle(cv::Mat frame){
    for(int i = 0; i < Row; i++){
        for(int j = 0; j < Col; j++){
            if(frame.at<uchar>(i,j)==0){
                Pixle[i][j] = black;
            }
            else{
                Pixle[i][j] = white;
            }
        }
    }
}

int zshConstructionBeginaAxis_X = 0;
int zshConstructionBeginaAxis_Y = 0;

int construction_out_cnt = 0;           //出施工区计数器

uint8_t zsh_find_right_bottom_pixle(void){
    // uint8_t _x,_y;
    int _x_max = 0,_y_max = 0,_xy_max = 0;
    for(int i = 20; i < Row-10; i++){
        for(int j = 20; j < 105; j++){//TODO 值用来选择选中区域
            if(Pixle[i][j] == white && i*j>_xy_max){
                _y_max=i;
                _x_max = j;
                _xy_max = i*j;
            }
        }
    }
    zshConstructionBeginaAxis_X = _x_max-10;
    zshConstructionBeginaAxis_Y = _y_max;
    cout<<"_x_max:"<<_x_max<<"_y_max:"<<_y_max<<endl;
    
}