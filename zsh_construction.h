/*
zsh_construction.h
code by zsh
date : 2022/07/13
*/

#ifndef __ZSH_CONSTRUCTION__
#define __ZSH_CONSTRUCTION__

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <time.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "Image_process.h"

#define zsh_MAX(a,b) a>b?a:b

using namespace std;

typedef enum ZSH_CONSTRUCTION_e{
    CONSTRUCTION_NULL = 0x00,//没有识别到
    CONSTRUCTION_BEGIN = 0x01,//开始准备进入
    CONSTRUCTION_IN = 0x02,//进入施工区
    CONSTRUCTION_INSIDE=0x03,//在施工区里面
    CONSTRUCTION_OUT=0x04,//出施工区
    CONSTRUCTION_RESET,//释放施工区状态，回归巡线模式
}ZSH_CONSTRUCTION;
extern ZSH_CONSTRUCTION zsh_construction_flag;
extern uint redPixle[Row][Col];
extern cv::Mat red_mask;
extern int zshConstructionBeginaAxis_X;
extern int zshConstructionBeginaAxis_Y;
extern int leftline[Row];
extern int rightline[Row];
extern int midline[Row];
extern uint leftlineflag[Row];
extern uint rightlineflag[Row];
extern uint Pixle[Row][Col];
extern int construction_out_cnt;
extern uint8_t ZSH_CONSTRUCTION_SPEED;
uint8_t zsh_construction_deal(void);

uint8_t zsh_construction_init(void);


//
uint8_t zsh_get_red_mask_pixle(cv::Mat frame);
uint8_t zsh_find_right_bottom_pixle(void);

#endif