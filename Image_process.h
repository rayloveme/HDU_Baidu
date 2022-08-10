#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#define  hstart     55

#define Col               120           //图像宽度
#define Row               60             //图像高度

#define black             0
#define white             1


//拟合的赛道宽度
#define Width_P_W_L 1  //1.2
#define Width_P_W_R  1.1  //1.2
#define Width_P_Z  1//1
#define Width_P_R_L  0.85  //1.25
#define Width_P_R_R  0.85  //1.25
#define Width_shancha_zhong  0.9//1.25
#define Width_shigongqu_ru  1//1.25


#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>


void cheku_find();
void scratch_white(void);
void get_route_one(uint hang,uint start);
void get_route_all(void);
int max(int a,int b);
void GetBlackEndParam(void);
void get_route_endline(void);
void get_route_midline(void);
void connect(uint hang_start,uint hang_end,int p[]);
int regression(uint startline,uint endline,int p[]);

#endif