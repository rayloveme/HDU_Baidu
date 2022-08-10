#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
// #include "uart_test.hpp"
//#include "self_serial.h"
#include <stdio.h>
#include <stdlib.h>
#include "Image_process.h"
#include "ring.h"
#include "detection.hpp"
#include "json.hpp"
#include <unistd.h>
#include "my_serial.hpp"
#include "predictor.hpp"
#include "sancha.h"
#include "zsh_construction.h"
#include <cmath>
#include <time.h>

using namespace std;
using namespace cv;

std::shared_ptr<Driver> driver = nullptr; //类初始化

std::string model_path = "/root/workspace/yolov3_mobilenet_v1";
// std::string model_path = "/root/workspace/ssd_mobilenet";

// 实例化检测器，并初始�?
// 对于这种非单例方式的Detection启动，需要三步：1. 初始化类�?2.init�?3.start线程
std::shared_ptr<Detection> detection = std::make_shared<Detection>(true);

std::shared_ptr<My_serial> my_serial = std::make_shared<My_serial>(true);

extern uint Pixle[Row][Col];
extern int leftline[Row];
extern int midline[Row];
extern int rightline[Row];

extern uint leftlineflag[Row];
extern uint rightlineflag[Row];

extern uint endline;

//全局变量  原始图像
Mat frame;
cv::Mat frame_ROI; //截取部分图像
cv::Rect rect_select = cv::Rect(10, 20, 100, 50);

//起止相关标志
uint start_key = 0;
uint end_key = 0;

int start_end_statues = 0; // 0为停下，1为运�?

int lap_flag = 0;

//速度相关参数
unsigned char STOP_SPEED = 130; //设置巡航速度

unsigned char NORMAL_SPEED_COPY = 80;
unsigned char START_SPEED = 200; // 45
unsigned char SPEED = 80;
unsigned char START_ANGLE = 70; // 159
unsigned char END_ANGLE = 45;
unsigned char ANGLE = 127;
// unsigned long START_TIME = 1000;    //起步延时1000ms
// unsigned int START_TIME = 1;  //起步延时1s
useconds_t START_TIME = 1050000;
useconds_t END_TIME = 2500000;
float LAST_ANGLE = 128;

/**********************************************
加权平均
***********************************************/

//正常加权
int Weight[60] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-9行，基本用不�?
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-19行，基本用不�?
    1, 1, 1, 2, 2, 3, 4, 4, 5, 5, // 20-29�? 2,2,2,2,3,4,4,4,5,5,   6,6,7,7,8,8,9,9,9,9,
    5, 5, 8, 7, 8, 9, 8, 8, 9, 9, // 30-39�?
    8, 8, 7, 7, 6, 6, 5, 5, 4, 4, // 40-49�?
    4, 3, 3, 2, 2, 1, 0, 0, 0, 0, //最近十�?
};

/********************main control*****************************/
int Track_Line_Time = 0;
int Track_Line_Num = 0;
uint Foresight = 0;
uint last_Foresight = 0;
int Foresight_hang = 0;
int cutroute_time = 0;
uint endline_Foresight = 5; // 5  20    15a
uint Foresight_up = 33;     // 28       还没用到
uint ALL_flag = 0;
uint start_flag = 0;
uint en_start_flag = 0;
uint end_flag = 0;
uint EA = 1;
uint EB = 0;
// uint sancha_static;
// uint SC_flag;
uint Go_L_or_R;
// uint banmaxian_flag;

uint you_find_hang;
uint black_count = 0;
uint black_cntcnt = 0;
uint en_cnt = 0;
uint turn_flag = 0;

//参数
float line_error = 0;
float Last_error = 0;
float WeightSum = 0;
float CenterSum = 0;

uint test = 0;
int camera_count = 0;
uint camera_count_flag = 0;
uint cnt_black = 0;
uint shang_cnt = 0;
uint start_cnt = 0;

// uint stop_flag;
// uint cnt_EA;
uint SC_flag_2 = 0;
// int white_zuo;
// int white_you;
uint endflag;

/***********************************************************/

/************************cross********************************/

//记录
int SZ_start_L = Row - 1, SZ_start_R = Row - 1, SZ_xie_L = Row - 1, SZ_xie_R = Row - 1, SZ_end_L = Row - 1, SZ_end_R = Row - 1;
//记标�?
int SZ_start_L_Flag = 0, SZ_start_R_Flag = 0, SZ_xie_L_Flag = 0, SZ_xie_R_Flag = 0, SZ_end_L_Flag = 0, SZ_end_R_Flag = 0;
int ru_cross_flag = 0, xie_cross_flag = 0, chu_cross_flag = 0;
int cross_white[Row];
int ru_num = 0;
int chu_num = 0;
int xie_white = 0;
int ru_point = 0;
int SZ_xie_point = 0;
int SZ_hang = 0;
int SZ_search_L_lie = 0;
int SZ_search_R_lie = 0;
int SZ_xie_L_UP = 0;
int SZ_xie_R_UP = 0;
int SZ_xie_L_UP_Flag = 0;
int SZ_xie_R_UP_Flag = 0;
int SZ_xie_L_UP_lie = 0;
int SZ_xie_R_UP_lie = 0;
uint cross_en = 0;
uint search_en = 0;
uint search_hang = 0;
uint coner_left[Col / 2];
uint coner_right[Col / 2];

/************************************************************/

/**************************ring******************************/

extern int ringflag;
extern int chu_flag;

/************************************************************/

/*****************************************sancha****************************************/
int sancha_flag = 0;
int sancha_find_cnt = 0;
int sancha_lost_cnt = 0;

int is_sancha = 0; //只是用来看看图片中是不是有三�?

/**************************************************************************************/

/***************************************cheku*****************************************/

int cheku_flag = 0;
int is_cheku = 0;
int cheku_find_cnt = 0;
int cheku_miss_cnt = 0;

/*************************************************************************************/

/********************************************podao************************************/
int podao_flag = 0;
int podao_send_flag = 0;
int podao_cnt = 0;
int podao_speed_cnt1 = 0;
int podao_speed_cnt2 = 0;

/*************************************************************************************/

/*****************************************shigongqu************************************/
// int shigongqu_send_flag = 0;

// int shigongqu_cnt = 0;
int shigongqu_find_cnt = 0;
int shigongqu_lost_cnt = 0;

/**************************************** detection *********************************/

std::mutex m_mutex; //给图像加�?

/**********************************************************/

void Get_Pixle(Mat frame)
{
  for (int i = 0; i < Row; i++)
  {
    for (int j = 0; j < Col; j++)
    {
      if (frame.at<uchar>(i, j) == 0)
      {
        Pixle[i][j] = black;
      }
      else
      {
        Pixle[i][j] = white;
      }
    }
  }
}

void get_route()
{
  //基础寻线
  get_route_one(Row - 1, Col / 2);
  get_route_all();
  //赛道信息提取
  GetBlackEndParam();
  //截止行判�?
  get_route_endline();
  zsh_construction_deal();

  if (ringflag == 0 && sancha_flag == 0 && zsh_construction_flag == CONSTRUCTION_NULL)
  {
    ring_find(); //检测圆�?
  }
  // else  cout<<"ringflag == "<<ringflag<<endl;
  if (ringflag != 0)
  {
    cout << "ringflag == " << ringflag << endl;
  }
  ring_start();
  ring_ru();
  ring_chu();

  if (sancha_flag == 1)
  {
    // cout<<"准备进入三叉"<< endl;
    sancha_ru();
  }
  if (sancha_flag == 2)
  {
    sancha_zhong();
  }
  if (sancha_flag == 3)
  {
    sancha_chu();
  }
  // cout<<ringflag<<endl;
  //拟合中线
  get_route_midline();
}

void get_deviation(void)
{
  CenterSum = 0; //中线值加�?
  WeightSum = 0; //中线权重加和
  // line_error是中线各个点的偏离中值的加权平均

  if (ringflag == 1) //这里！！！！！！
  {
    if (ringflag == 1)
    { //进入左圆环时给定偏差
      line_error = 20;
    }
    // else if(ringflag==3&&endline>=40)
    //   line_error=50;

    // else if(chu_flag == 1){
    //   line_error=20;
    // }
  }
  else if (ringflag == 2) //进入右圆环时给定偏差
  {
    if (ringflag == 2)
    {
      line_error = 100;
    }
    // else if(ringflag==4&&endline>=40)
    //   line_error=70;

    // else if(chu_flag == 1){
    //   line_error=100;
    // }
  }
  else if (ringflag == 0 || ringflag == 3 || ringflag == 4) //在圆环外或者圆环内，正常巡�?
  {
    if (ringflag == 3 && chu_flag == 1)
    { //出圆环时给定偏差
      line_error = 20;
    }
    else if (ringflag == 4 && chu_flag == 1)
    { //出圆环时给定偏差
      line_error = 100;
    }
    else if (endline < 30) //如果截止行在图像的上半部�?
    {
      // cout<<"正常求偏差�?"<<endl;
      for (uint hang = 55; hang > endline; hang--) // 50
      {
        CenterSum += midline[hang] * Weight[hang];
        WeightSum += Weight[hang];
      }
      //算出加权平均后中线的�?
      if (WeightSum != 0)
      {
        line_error = (CenterSum / WeightSum) + 5; //+补偿�?            //甚至目前是向右补偿的，给多点会怎样
      }
      else
      {
        line_error = 60;
      }
    }
    else
    {
      line_error = midline[hstart];
    }
  }

  // cout<<"line_error == "<<line_error<<endl;
  // if(line_error > 60)
  //     line_error+=20;
}

void Track_Line(void) //获得截止�?
{
  uint endline_flag = 0, endline_flag2 = 1, endline_cnt = 0;

  endline_Foresight = 8;

  //前瞻
  // if(endline<Foresight_up)
  //   {
  //         Foresight_hang=Foresight_up;
  //   }
  // else
  // {

  // if(ringflag == 1 || ringflag == 3 || ringflag == 2 || ringflag == 4){              //圆环改前�?
  //   endline_Foresight = 16;
  // }
  // else{
  //   endline_Foresight = 8;
  // }
  if (sancha_find_cnt > 0 || is_sancha == 1)
  { //防止看到三叉后的抖动
    endline = 40;
  }

  if (sancha_flag == 2)
  {
    endline_Foresight = 15;
  }

  if (ringflag == 3 || ringflag == 4)
  {
    endline_Foresight = 3;
  }
  if (zsh_construction_flag == CONSTRUCTION_IN)
  { //进入施工区的时候更改endline
    endline = zshConstructionBeginaAxis_Y;
  }

  Foresight_hang = endline + endline_Foresight;

  //  cout<<"Foresight_hang = "<<Foresight_hang<<endl;
  // }

  // if(Foresight_hang>50)       //原来�?50
  //   Foresight_hang=50;

  // if(Foresight_hang>54)
  // Foresight_hang = 53;
  // if(Foresight_hang < endline)
  //   Foresight_hang = endline;
  // if(start_flag==0)
  // {
  //    Foresight_hang=endline_chu_hang_min+3;
  // }

  // cout<<"Foresight_hang = "<<Foresight_hang<<endl;

  //   if(ringflag==0)
  //   {

  //       if(start_flag==3)
  //       {
  // //              Foresight_hang=33;
  //           Foresight_hang=55;
  //       }

  //       }

  // if(ALL_flag==11)   Foresight_hang=Pro_value(endline+22);               //????????????????????????????

  // cout<<"Foresight_hang == "<<Foresight_hang<<endl;
  // cout<<"midline[Foresight_hang] == "<< midline[Foresight_hang] <<endl;
  // cout<<"leftline = "<<leftline[Foresight_hang]<<endl;
  // cout<<"rightline = "<<rightline[Foresight_hang]<<endl;
  // cout<<"leftlineflag = "<<leftlineflag[Foresight_hang]<<endl;
  // cout<<"rightlineflag = "<<rightlineflag[Foresight_hang]<<endl;
  // Foresight=(uint)(0.2*line_error+0.8*midline[Foresight_hang]);             //这里
  Foresight = (uint)midline[Foresight_hang];
  // cout<<"误差 == "<< Foresight <<endl;

  // if(ringflag!=0){
  //   if((Foresight>last_Foresight) && (Foresight - last_Foresight > 5)){
  //     Foresight = last_Foresight + 5;
  //     cout<<"限幅"<<endl;
  //   }

  //   if((Foresight<last_Foresight) && (last_Foresight - Foresight > 5)){
  //     Foresight = last_Foresight - 5;
  //   }

  //   last_Foresight = Foresight;
  // }
  // else{
  //   last_Foresight = Foresight;
  // }

  // if(start_flag==3)
  // {
  //     if(Go_L_or_R==0)
  //     {
  //         if(BlackEndM>30&&Foresight<65&&endline_flag2==1)
  //             endline_flag=1;

  //         if(endline_flag==1)
  //         {
  //             Foresight=65;
  //             endline_cnt++;
  //         }
  //         if(endline_cnt>10)
  //         {
  //             endline_flag2=0;
  //         }
  //     }
  // }

  if (Foresight > 120) //限幅
    Foresight = 119;

  if (ALL_flag != 241) //没有冲出赛道�?
  {

    // ALL_flag=ringflag /*+sanchaflag+rampflag*/ ;

    if (ALL_flag == 11)
    {
      SC_flag_2 = 1;
    }
    if (SC_flag_2)
      ALL_flag = 11;

    if (start_flag == 0)
    {
      ALL_flag = 242;
    }
    if (start_flag == 3 && you_find_hang > 39)
      ALL_flag = 243;
  }
}

Mat red_mask;

Mat ImageProcess(Mat frame)
{ //定义图像处理函数
  Mat dst, dstt, element, resul, dst_hsv;

  cv::resize(frame, dst, Size(Col, Row), 0, 0);

  cv::cvtColor(dst, dst_hsv, cv::COLOR_RGB2HSV); //转换HSV

  cv::inRange(dst_hsv, Scalar(100, 100, 100), Scalar(140, 255, 255), red_mask); //得到掩膜

  if (zsh_construction_flag != CONSTRUCTION_NULL)
  {                                                           //施工区内采取特殊腐蚀膨胀
    element = getStructuringElement(MORPH_RECT, Size(11, 7)); //采用腐蚀膨胀降噪
    dilate(red_mask, red_mask, element, Point(-1, -1), 1);    //扩大掩膜范围
    erode(red_mask, red_mask, element, Point(-1, -1), 1, 0);
    dilate(red_mask, red_mask, element, Point(-1, -1), 1); //扩大掩膜范围
  }
  else
  {
    element = getStructuringElement(MORPH_RECT, Size(7, 7)); //采用腐蚀膨胀降噪
    dilate(red_mask, red_mask, element, Point(-1, -1), 1);   //扩大掩膜范围
  }

  // cv::cvtColor(red_mask, red_mask, cv::COLOR_GRAY2RGB);
  // cv::bitwise_xor(dst, red_mask, dst);                                   //异或操作

  cv::cvtColor(dst, dst, cv::COLOR_RGB2GRAY); //转换灰度�?
  // cv::GaussianBlur(dst, dst, cv::Size(13, 13), 5, 5);           //高斯滤波
  cv::threshold(dst, dstt, 200, 255, CV_THRESH_BINARY | THRESH_OTSU); //二值化图像		| THRESH_OTSU
  // element = getStructuringElement(MORPH_RECT, Size(3, 3));    //采用腐蚀膨胀降噪
  // erode(dstt, dstt, element, Point(-1, -1), 1, 0);
  // dilate(dstt, dstt, element, Point(-1, -1), 1);

  cv::bitwise_or(dstt, red_mask, dstt); //或操作， 去除红色标志对巡线的影响

  if (zsh_construction_flag == CONSTRUCTION_NULL)
    Get_Pixle(dstt); //转换为二维黑白数�?
  else
    Get_Pixle(red_mask); //施工区内用红色掩膜来获得黑白数组
  // Get_Pixle(dstt);

  cheku_find(); //检测车库斑马线

  if (sancha_flag == 2 || zsh_construction_flag == CONSTRUCTION_INSIDE)
  { //在三叉、施工区内，执行黑白颜色转换
    sancha_switch();
  }

  get_route();

  get_deviation();

  Track_Line();

  cv::cvtColor(dstt, dstt, cv::COLOR_GRAY2RGB);     //转回三通道
  cvtColor(red_mask, red_mask, cv::COLOR_GRAY2RGB); //转回三通道
  if (zsh_construction_flag != CONSTRUCTION_NULL)
  {
    circle(red_mask, Point(zshConstructionBeginaAxis_X, zshConstructionBeginaAxis_Y), 0.1, Scalar(0xE8, 0xAA, 0x42), -1);
    for (uint i = 59; i > zshConstructionBeginaAxis_Y; i--)
    {
      circle(red_mask, Point(midline[i], i), 0.1, Scalar(0, 255, 0), -1);
      circle(red_mask, Point(leftline[i], i), 0.1, Scalar(255, 0, 0), -1);
      circle(red_mask, Point(rightline[i], i), 0.1, Scalar(0, 0, 255), -1);
    }
  }
  else
  {
    for (uint i = 59; i > endline; i--)
    {
      circle(dstt, Point(midline[i], i), 0.1, Scalar(0, 255, 0), -1);
      circle(dstt, Point(leftline[i], i), 0.1, Scalar(255, 0, 0), -1);
      circle(dstt, Point(rightline[i], i), 0.1, Scalar(0, 0, 255), -1);
    }
  }
  // circle(dstt, Point(25,25), 0.1, Scalar(128, 255, 43), -1);
  return dstt;
}

void getSpeed()
{
  // float k_s=0.6;  //图像偏差等比例影响速度
  // if(Foresight<=59){
  //   {
  //     std::unique_lock<std::mutex> lock(m_mutex);   //加锁
  //     SPEED = NORMAL_SPEED ;//- (unsigned char)(59 - Foresight);
  //   }
  // }
  // else{
  //   {
  //     std::unique_lock<std::mutex> lock(m_mutex);   //加锁
  //     SPEED = NORMAL_SPEED ;//- (unsigned char)(Foresight - 59);
  //   }
  // }
  // cout<<"Foresight == "<<Foresight<<endl;
}

void getAngle()
{
  float k = 2.13; //比例放大偏差
  float angle = k * Foresight + 127 - k * 59;
  // cout<<"angle == "<< angle <<endl;
  {
    std::unique_lock<std::mutex> lock(m_mutex);          //加锁
    ANGLE = (unsigned char)(1 * angle + 0 * LAST_ANGLE); //一阶互补滤�?
  }
  cout << "ANGLE == " << angle << endl;
  LAST_ANGLE = angle;
}

void wait_key()
{
  ringflag = 0;                              //圆环状态初始化
  sancha_flag = 0;                           //三叉状态初始化
  zsh_construction_flag = CONSTRUCTION_NULL; //施工区状态初始化
  cheku_flag = 0;
  cout << "car waiting......" << endl;
  while (1)
  {
    if (start_key == 1 && end_key == 0)
    {
      ANGLE = 128;
      SPEED = 0;
      // sleep(2);
      // ANGLE = START_ANGLE;
      // SPEED = START_SPEED;
      // usleep(START_TIME);
      start_end_statues = 1;
      // start_key = 0;
      break;
    }
    else
      continue;
  }
}

int main(int argc, char *argv[])
{

  my_serial->ser_Init(); //串口初始�?
  my_serial->start();    //开启串口线�?

  /*注意�?
    使用 0 �? /dev/video0 的分辨率不同�?
      0           : opencv 内部的采集，可能是基�? V4L2, 分辨率：1280 * 960
      /dev/video0 : 基于Gstreamer �? 分辨率：640 * 480
  */
  VideoCapture capture("/dev/video0");
  if (!capture.isOpened())
  {
    std::cout << "can not open video device " << std::endl;
    return 1;
  }
  if (!capture.read(frame))
  {
    std::cout << "no video frame" << std::endl;
    // continue;
  }

  // double rate = capture.get(CAP_PROP_FPS);
  // double width = capture.get(CAP_PROP_FRAME_WIDTH);
  // double height = capture.get(CAP_PROP_FRAME_HEIGHT);
  // std::cout << "Camera Param: frame rate = " << rate << " width = " << width
  //           << " height = " << height << std::endl;

  std::string window_name = "usbcamera";
  namedWindow(window_name, WINDOW_NORMAL);

  int width_ = 640;
  int height_ = 480;

  int ret = detection->init(model_path); //检测线程初始化
  if (ret != 0)
  {
    std::cout << "Detection init failed. " << std::endl;
    return -1;
  }

  // cout<<"1111"<<endl;
  detection->start(); //开启检测线�?

  Mat dst;

  clock_t start, finish, _next;

  while (1)
  {
    if (start_key == 0 && end_key == 0 || start_key == 0 && end_key == 1)
    {
      start_end_statues = 0;
      wait_key();
    }
    if (cheku_flag == 3)
    {
      SPEED = STOP_SPEED;
      ANGLE = END_ANGLE;

      usleep(END_TIME);

      SPEED = 0;
      ANGLE = 128;

      start_end_statues = 0;
      start_key = 0;
      end_key = 1;
      // wait_key();
    }

    start = clock();
    
    {
      std::unique_lock<std::mutex> lock(m_mutex); //加锁
      if (!capture.read(frame))
      {
        std::cout << "no video frame" << std::endl;
        continue;
      }
    }

    dst = ImageProcess(frame);

    getSpeed();
    getAngle();

    cv::imshow(window_name, dst);

    finish = clock();

    // int delay_t;
    // double time_cost = double(finish - start) / CLOCKS_PER_SEC;
    // cout <<endl<<"the time cost is:" << time_cost<<endl;
    // if(time_cost < 0.033333){
    // delay_t = int(33333 - time_cost*1000000);
    // usleep(100000);
    // cout<< "delay_t = " << delay_t << endl;
    // }

    // _next = clock();
    // cout <<endl<<"the holl_time cost is:" << double(finish - start) / CLOCKS_PER_SEC <<endl;
    waitKey(1);
  }
  capture.release();
  return 0;
}
