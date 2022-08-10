#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "Image_process.h"
#include "ring.h"
#include "zsh_construction.h"

using namespace std;
using namespace cv;

uint Pixle[Row][Col];

//存左右中边线数组 1-118
int leftline[Row] = {0};
int midline[Row] = {0};
int rightline[Row] = {0};
//存储标志 2可信 1不一定可信 0丢线
uint leftlineflag[Row];
uint rightlineflag[Row];
//截止行 55-0
uint endline = 0;

uint endline_chu_hang_min = 60;

//真实赛道宽度
// uint track_width[Row]={                                             //十二楼赛道宽度
//                         13 ,14 ,15 ,16 ,17 ,18 ,20 ,22 ,24 ,27 ,
//                         27 ,29 ,30 ,32 ,33 ,34 ,36 ,38 ,39 ,41 ,
//                         42 ,44 ,46 ,47 ,49 ,51 ,51 ,53 ,55 ,55 ,
//                         57 ,60 ,62 ,64 ,65 ,66 ,67 ,69 ,71 ,71 ,
//                         73 ,75 ,77 ,78 ,79 ,81 ,83 ,84 ,86 ,87 ,
//                         89 ,90 ,92 ,94 ,95 ,97 ,99 ,100 ,100 ,100
//                         };

int track_width[Row] = { // 7.19 赛道宽度
    8, 9, 10, 11, 12, 13, 14, 17, 19, 21,
    23, 24, 26, 28, 30, 32, 34, 36, 38, 39,
    42, 44, 46, 48, 50, 51, 53, 56, 57, 59,
    61, 63, 65, 67, 69, 71, 74, 76, 78, 80,
    82, 84, 86, 88, 91, 93, 96, 97, 99, 101,
    103, 105, 107, 108, 109, 112, 113, 114, 114, 115};

// int track_width[Row] = { //6.23 咖啡屋 赛道宽度
//     9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
//     19, 20, 21, 23, 25, 27, 28, 30, 32, 33,
//     35, 37, 38, 40, 42, 44, 46, 48, 49, 51,
//     52, 54, 56, 58, 59, 61, 63, 65, 67, 69,
//     69, 71, 73, 75, 77, 79, 80, 82, 84, 86,
//     88, 90, 91, 93, 95, 97, 99, 100, 100, 100};

// uint track_width[Row]={
//                         0X05 ,0X06 ,0X07 ,0X08 ,0X09 ,0X0A ,0X0B ,0X0C ,0X0D ,0X0E ,
//                         0X11 ,0X13 ,0X14 ,0X16 ,0X18 ,0X1A ,0X1C ,0X1E ,0X20 ,0X21 ,
//                         0X24 ,0X26 ,0X27 ,0X29 ,0X2B ,0X2D ,0X2F ,0X30 ,0X32 ,0X34 ,
//                         0X36 ,0X38 ,0X3A ,0X3C ,0X3D ,0X3F ,0X41 ,0X44 ,0X46 ,0X48 ,
//                         0X4A ,0X4B ,0X4D ,0X4F ,0X51 ,0X53 ,0X55 ,0X57 ,0X5A ,0X5A ,
//                         0X5D ,0X5F ,0X61 ,0X62 ,0X64 ,0X66 ,0X67 ,0X69 ,0X6B ,0X6B
//                         };

uint BlackEndMR = 0;
uint BlackEndML = 0;
uint BlackEndLL = 0;
uint BlackEndRR = 0;
uint BlackEndL = 0;
uint BlackEndM = 0;
uint BlackEndR = 0;
uint BlackEndMaxMax = 0;
uint BlackEndMax = 0;
uint DropRow = 0;

//外部参数
extern uint ALL_flag;
extern uint EB;
extern uint cross_en;

extern int ringflag;
extern int sancha_flag;
extern int cheku_flag;

extern int cheku_miss_cnt;
extern int is_cheku;

uint white_flag = 0;

void cheku_find()
{
  int tiaobian = 0;
  int cheku_hang = 33; //找斑马线跳变的行
  for (uint lie = 20; lie < 100; lie++)
  {
    if (Pixle[cheku_hang][lie] == white && Pixle[cheku_hang][lie + 1] == black)
      tiaobian++;
    else if (Pixle[cheku_hang][lie] == black && Pixle[cheku_hang][lie + 1] == white)
      tiaobian++;
  }

  if (tiaobian > 11)
  {
    is_cheku = 1;
    if (cheku_flag == 0)
    {
      cheku_flag = 1;
    }
    if (cheku_flag == 2)
    {
      cheku_flag = 3;
    }
  }
  else if (cheku_flag == 1)
  {
    cheku_miss_cnt++;
    is_cheku = 0;
  }
  else
  {
    is_cheku = 0;
  }

  if (cheku_miss_cnt > 30)
  {
    cheku_flag = 2;
    cheku_miss_cnt = 0;
  }
}

void scratch_white(void) //获取白点个数      好像没用到？？
{
  uint white_hang = 0;
  white_flag = 0;
  for (uint hang = Row - 1; hang > 45; hang++)
  {
    white_hang = 0;
    for (uint lie = 0; lie < Col; lie++)
    {
      if (Pixle[hang][lie] == white)
        white_hang++;
    }
    if (white_hang > 110)
    {
      white_flag = 1;
      break;
    }
  }
}

void get_route_one(uint hang, uint start) //传入需要检索的那一行
{
  //清零
  for (uint hang = 0; hang < Row; hang++)
  {
    leftline[hang] = 0;
    rightline[hang] = Col - 1;
    leftlineflag[hang] = 0;
    rightlineflag[hang] = 0;
    midline[hang] = Col / 2;
  }
  endline = 0;
  //检测本行起跑线标志位
  int baihei = 0;
  // for(uint lie=0;lie<Col-1;lie++)                                  //起跑线检测
  // {
  //   if(Pixle[hang][lie]==white&&Pixle[hang][lie+1]==black)
  //     baihei++;
  // }
  //起跑线寻线
  if (baihei > 3)
  {
    if (Pixle[hang][5] == white)
    {
      //向左找
      for (uint lie = 5; lie >= 1; lie--)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 1;
          break;
        }
      }
    }
    else
    {
      //向右找
      for (uint lie = 5; lie <= Col - 2; lie++)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie + 1] == white)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 1;
          break;
        }
      }
    }
    //如果没找到，直接给定值
    if (leftlineflag[hang] == 0)
    {
      leftline[hang] = 1;
      leftlineflag[hang] = 1;
    }
    //右边线
    if (Pixle[hang][Col - 5] == white)
    {
      //向右找
      for (uint lie = Col - 5; lie <= Col - 2; lie++)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 2;
          break;
        }
      }
    }
    else
    {
      //向左找
      for (uint lie = Col - 5; lie >= 1; lie--)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie - 1] == white)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 1;
          break;
        }
      }
    }
    //如果没找到，直接给定值
    if (rightlineflag[hang] == 0)
    {
      rightline[hang] = Col - 2;
      rightlineflag[hang] = 2;
    }
  }
  else
  {
    //左边线
    if (Pixle[hang][start] == white)
    {
      //向左找
      for (uint lie = start; lie >= 1; lie--)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 2;
          break;
        }
      }
    }
    else
    {
      //向右找
      for (uint lie = start; lie <= Col - 2; lie++)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie + 1] == white)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 1;
          break;
        }
      }
    }
    //如果没找到，直接给定值
    if (leftlineflag[hang] == 0)
    {
      leftline[hang] = 1;
      leftlineflag[hang] = 2;
    }
    //右边线
    if (Pixle[hang][start] == white)
    {
      //向右找
      for (uint lie = start; lie <= Col - 2; lie++)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 2;
          break;
        }
      }
    }
    else
    {
      //向左找
      for (uint lie = start; lie >= 1; lie--)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie - 1] == white)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 1;
          break;
        }
      }
    }
    //如果没找到，直接给定值
    if (rightlineflag[hang] == 0)
    {
      rightline[hang] = Col - 2;
      rightlineflag[hang] = 2;
    }
    //如果左右边线都不绝对可信
    if (rightlineflag[hang] == 1 && leftlineflag[hang] == 1)
    {
      leftline[hang] = 1;
      rightline[hang] = Col - 2;
    }
    leftlineflag[hang] = 1;
    rightlineflag[hang] = 1;
  }
}

//跟踪寻线
void get_route_all(void)
{
  //初始化
  int Lstart = 0, L_max = 0, L_min = 0;
  int Rstart = 0, R_max = 0, R_min = 0;
  int range = 20; //搜线范围
  for (uint hang = Row - 1; hang > 0; hang--)
  { //丢线标志清零
    leftlineflag[hang] = 0;
    rightlineflag[hang] = 0;
  }

  for (uint hang = Row - 2; hang > 0; hang--)
  {
    //左边线
    //确定搜线范围
    //根据上一行确定本行寻线点
    if (leftlineflag[hang + 1] == 0) //上一行丢线
    {
      if (rightlineflag[hang + 1] != 0)
      {                                                          //上一行右线不丢
        L_max = rightline[hang + 1] + range - track_width[hang]; //搜线的起始条件
        L_min = rightline[hang + 1] - range - track_width[hang];
        Lstart = rightline[hang + 1] - track_width[hang];
        if (L_max > Col - 1) //限幅
          L_max = Col - 1;
        if (L_max < 0)
          L_max = 0;
        if (L_min > Col - 1)
          L_min = Col - 1;
        if (L_min < 0)
          L_min = 0;
        if (Lstart > Col - 1)
          Lstart = Col - 1;
        if (Lstart < 0)
          Lstart = 0;
      }
      else
      {
        L_max = Col - 1;
        L_min = 0;
        Lstart = Col * 1 / 2 - 10; //为什么-10           todo
      }
    }
    else //上一行不丢线
    {
      L_max = leftline[hang + 1] + range;
      L_min = leftline[hang + 1] - range;
      Lstart = leftline[hang + 1];
      if (L_max > Col - 1) //限幅
        L_max = Col - 1;
      if (L_min < 0)
        L_min = 0;
    }
    //左边线
    if (Pixle[hang][Lstart] == black)
    {
      //向右找
      for (uint lie = Lstart; lie < L_max; lie++)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie + 1] == white)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 1;
          break;
        }
      }
      //如果还没找到说明丢线
      if (leftlineflag[hang] == 0)
      {
        leftline[hang] = 1; // Col - 1
        //  leftlineflag[hang]=0;
      }
    }
    else
    {
      //向左找
      for (uint lie = Lstart; lie > L_min; lie--)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black)
        {
          leftline[hang] = lie;
          leftlineflag[hang] = 1;
          break;
        }
      }
      //如果还没找到说明丢线
      if (leftlineflag[hang] == 0)
      {
        leftline[hang] = 1;
        //  leftlineflag[hang]=0;
      }
    }

    //右边线
    //确定寻线范围
    if (rightlineflag[hang + 1] == 0) //上一行丢线
    {
      if (leftlineflag[hang + 1] != 0)
      { //上一行左线不丢
        R_max = leftline[hang + 1] + range + track_width[hang];
        R_min = leftline[hang + 1] - range + track_width[hang];
        Rstart = leftline[hang + 1] + track_width[hang];
        if (R_max > Col - 1) //限幅
          R_max = Col - 1;
        if (R_max < 0)
          R_max = 0;
        if (R_min > Col - 1)
          R_min = Col - 1;
        if (R_min < 0)
          R_min = 0;
        if (Rstart > Col - 1)
          Rstart = Col - 1;
        if (Rstart < 0)
          Rstart = 0;
      }
      else
      {
        R_max = Col - 1;
        R_min = 0;
        Rstart = Col * 1 / 2 - 10;
      }
    }
    else
    {
      Rstart = rightline[hang + 1];
      R_max = rightline[hang + 1] + range;
      R_min = rightline[hang + 1] - range;
      if (R_max > Col - 1)
        R_max = Col - 1;
      if (R_min < 0)
        R_min = 0;
    }
    //右边线
    if (Pixle[hang][Rstart] == black)
    {
      //向左找
      for (uint lie = Rstart; lie > R_min; lie--)
      {
        if (Pixle[hang][lie] == black && Pixle[hang][lie - 1] == white)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 1;
          break;
        }
      }
      //如果还没找到
      if (rightlineflag[hang] == 0)
      {
        rightline[hang] = Col - 1; // 1
                                   //  rightlineflag[hang]=0;
      }
    }
    else
    {
      //向右找
      for (uint lie = Rstart; lie < R_max; lie++)
      {
        if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black)
        {
          rightline[hang] = lie;
          rightlineflag[hang] = 1;
          break;
        }
      }
      //如果还没找到
      if (rightlineflag[hang] == 0)
      {
        rightline[hang] = Col - 2;
        // rightlineflag[hang]=0;
      }

      if (rightline[hang] >= 117) //这里有一个BUG没有解决，当右线没有找到时rightline应该是0，但是不知道为什么仍然是1，所以强制置零
      {
        rightlineflag[hang] = 0;
      }
      if (leftline[hang] <= 2)
      {
        leftlineflag[hang] = 0;
      }
    }

    // cout<<hang<<"宽度 == "<<rightline[hang]-leftline[hang]<<endl;        //量赛道宽度
  }
}

//取最大值
int max(int a, int b)
{
  return a > b ? a : b;
}
//提取图像的特征
//选取几列，从图像底部往上扫描
void GetBlackEndParam(void) //获取黑线截止行
{

  unsigned char LEndFlag = 0; //标志位
  unsigned char MEndFlag = 0;
  unsigned char REndFlag = 0;
  unsigned char MREndFlag = 0;
  unsigned char MLEndFlag = 0;
  unsigned char LLEndFlag = 0;
  unsigned char RREndFlag = 0;

  int i = 0;

  BlackEndMR = 0; //清零
  BlackEndML = 0;
  BlackEndLL = 0;
  BlackEndRR = 0;
  BlackEndL = 0;
  BlackEndM = 0;
  BlackEndR = 0;

  for (i = Row - 1; i > 0; i--)
  {
    if (Pixle[i][60] == white && !MEndFlag) //
    {
      BlackEndM++; //中黑线截至行
    }
    else if (i > 1 && Pixle[i - 1][60] == black && Pixle[i - 2][Col / 2] == black) //连续两行是黑色
    {
      MEndFlag = 1;
    }
    if (Pixle[i][30] == white && !LEndFlag)
    {
      BlackEndL++; //左黑线截至行
    }
    else if (i > 1 && Pixle[i - 1][30] == black && Pixle[i - 2][Col / 4] == black)
    {
      LEndFlag = 1;
    }
    if (Pixle[i][90] == white && !REndFlag)
    {
      BlackEndR++; //右黑线截至行
    }
    else if (i > 1 && Pixle[i - 1][90] == black && Pixle[i - 2][Col * 3 / 4] == black)
    {
      REndFlag = 1;
    }
    if (Pixle[i][45] == white && !MLEndFlag)
    {
      BlackEndML++;
    }
    else if (i > 1 && Pixle[i - 1][45] == black && Pixle[i - 2][45] == black)
    {
      MLEndFlag = 1;
    }
    if (Pixle[i][75] == white && !MREndFlag)
    {
      BlackEndMR++;
    }
    else if (i > 1 && Pixle[i - 1][75] == black && Pixle[i - 2][75] == black)
    {
      MREndFlag = 1;
    }
    if (Pixle[i][5] == white && !LLEndFlag)
    {
      BlackEndLL++;
    }
    else if (i > 1 && Pixle[i - 1][5] == black && Pixle[i - 2][5] == black)
    {
      LLEndFlag = 1;
    }
    if (Pixle[i][115] == white && !RREndFlag)
    {
      BlackEndRR++;
    }
    else if (i > 1 && Pixle[i - 1][115] == black && Pixle[i - 2][115] == black)
    {
      RREndFlag = 1;
    }
  }

  if (BlackEndM < 15)
    BlackEndM = 15;

  BlackEndMax = max(BlackEndL, BlackEndM); //取大值
  BlackEndMax = max(BlackEndMax, BlackEndR);
  BlackEndMaxMax = max(BlackEndMax, BlackEndMR);
  BlackEndMaxMax = max(BlackEndMaxMax, BlackEndML);
  BlackEndMaxMax = max(BlackEndMaxMax, BlackEndLL);
  BlackEndMaxMax = max(BlackEndMaxMax, BlackEndRR);
  if (BlackEndMaxMax > 60)
  {
    BlackEndMaxMax = 58;
  }
  DropRow = 60 - BlackEndMaxMax; //封顶的行数

  if (DropRow >= 55)
    DropRow = 55;

  if (DropRow <= 6)
    DropRow = 6;

  /*******初始判断，冲出赛道停车******/
  //         if(BlackEndMaxMax < 15)
  //         {
  //             ALL_flag=241;
  //         }
  //         else if(BlackEndMaxMax>=15&&!sancha_en&&!EB&&ALL_flag!=11)
  //             ALL_flag=0;

  if (ALL_flag == 10)
    EB = 0;
}

//寻截止行
void get_route_endline(void)
{
  //截止行的判断需要消除折点，这里不破坏原来左右线数组
  int left_line[Row], right_line[Row];

  //复制
  for (uint hang = Row - 1; hang > 0; hang--)
  {
    left_line[hang] = leftline[hang];
    right_line[hang] = rightline[hang];
  }

  //消折点（突变点）
  // for(uint hang=Row-3;hang>0;hang--)        //这是啥意思,感觉没有用啊？？？？
  // {
  //   int temp=0;
  //   //左
  //   if((left_line[hang+1]-left_line[hang])>0)
  //   {
  //       temp=left_line[hang+1];
  //       left_line[hang+1]=left_line[hang];
  //       left_line[hang]=temp;
  //   }
  //   //右
  //   if((right_line[hang]-right_line[hang+1])>0)
  //   {
  //       temp=right_line[hang+1];
  //       right_line[hang+1]=right_line[hang];
  //       right_line[hang]=temp;
  //   }
  // }

  endline = DropRow;

  // cout<<DropRow<<endl;

  //截止行判断
  for (uint hang = Row - 5; hang > endline; hang--)
  {
    //隔离赛道外的边线
    if ((right_line[hang - 1] < left_line[hang]) || (left_line[hang - 1] > right_line[hang]))
    {
      endline = hang;
      break;
    }
  }

  for (uint hang = Row - 5; hang > DropRow; hang--) //左右线是否向外跳变来看是否截止     (可能也许不一定靠谱)                 越界？
  {
    int leftend_flag = 0;
    int rightend_flag = 0;
    if ((left_line[hang] - left_line[hang + 1]) < -9 && (left_line[hang] - left_line[hang + 2]) < -9)
    { //左线发生跳变
      // cout<<"左线发生跳变 == "<<hang<<endl;
      if (leftlineflag[hang] != 0 && leftlineflag[hang + 1] != 0 //防止丢线的影响
          || leftlineflag[hang] != 0 && leftlineflag[hang + 2] != 0 || leftlineflag[hang + 1] != 0 && leftlineflag[hang + 2] != 0)
      {
        leftend_flag = 1;
      }
    }
    if ((right_line[hang] - right_line[hang + 1]) > 9 && (right_line[hang] - right_line[hang + 2]) > 9)
    {                                                              //右线发生跳变
      if (rightlineflag[hang] != 0 && rightlineflag[hang + 1] != 0 //防止丢线的影响
          || rightlineflag[hang] != 0 && rightlineflag[hang + 2] != 0 || rightlineflag[hang + 1] != 0 && rightlineflag[hang + 2] != 0)
      {
        rightend_flag = 1;
      }
    }

    if ((left_line[hang] - left_line[hang - 1]) < -2 && (left_line[hang] - left_line[hang - 2]) < -2 && (left_line[hang] - left_line[hang - 3]) < -3 && (left_line[hang] - left_line[hang + 3]) < -3 && left_line[hang] > 30)
    { //左线发生跳变
      // cout<<"左线发生跳变 == "<<hang<<endl;
      if (leftlineflag[hang] != 0 && leftlineflag[hang + 1] != 0 && leftlineflag[hang + 2] != 0)
      {
        leftend_flag = 1;
      }
    }
    if ((right_line[hang] - right_line[hang - 1]) > 1 && (right_line[hang] - right_line[hang - 2]) > 2 && (right_line[hang] - right_line[hang - 3]) > 3 && (right_line[hang] - right_line[hang + 3]) > 3 && right_line[hang] < 30)
    { //右线发生跳变
      if (rightlineflag[hang] != 0 && rightlineflag[hang + 1] != 0 && rightlineflag[hang + 2] != 0)
      {
        rightend_flag = 1;
      }
    }
    if (leftend_flag != 0 || rightend_flag != 0)
    { //跳变往下两行认为是截止行
      endline = hang + 2;
      break;
    }
  }

  // if (DropRow < 10 && BlackEndM > 50)
  //   endline = DropRow;

  if (endline > 48)
    endline = 48;

  if (zsh_construction_flag == CONSTRUCTION_BEGIN)
  {
    endline = zshConstructionBeginaAxis_Y;
  }
  for (uint hang = endline; hang > 0; hang--)
  { //截止行往上认为是丢线
    leftlineflag[hang] = 1;
    rightlineflag[hang] = 1;
  }

  // if(DropRow > endline){
  //   endline = DropRow;
  // }
  // cout<<"endline == "<<endline<<endl;
}

//拟合中线
void get_route_midline(void)
{
  uint which_line;

  which_line = DropRow; //                endline!!!!!!!!!!

  if (zsh_construction_flag == CONSTRUCTION_IN)
    which_line = zshConstructionBeginaAxis_Y;

  if (which_line > 50) //防止补线补不到
    which_line = 50;

  // for(uint hang = 59; hang>which_line; hang--){
  //   leftline[hang] = 90;
  //   rightline[hang] = 119;
  //   leftlineflag[hang] = 1;
  //   rightlineflag[hang] = 1;
  // }

  for (uint hang = 59; hang > which_line; hang--)
  {
    //   直道补线
    if (BlackEndM >= 35) // 47                   todo
    {
      //左丢又不丢
      if ((leftlineflag[hang] == 0) && (rightlineflag[hang] != 0))
      {
        leftline[hang] = rightline[hang] - Width_P_Z * track_width[hang]; //-5?
        // cout<<"补左线  "<< leftline[hang]<<endl;
      }
      //右丢左不丢
      else if ((leftlineflag[hang] != 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = leftline[hang] + Width_P_Z * track_width[hang]; //+5?
      }
      //  全丢
      else if ((leftlineflag[hang] == 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = Col - 2;
        leftline[hang] = 1;
      }
      //全不丢
      // cout<< "直道补线？？"<<endl;
    }
    else if (ringflag == 4 || ringflag == 3)
    {
      //左丢又不丢
      if ((leftlineflag[hang] == 0) && (rightlineflag[hang] != 0))
      {
        leftline[hang] = rightline[hang] - Width_P_R_R * track_width[hang];
      }
      //右丢左不丢
      else if ((leftlineflag[hang] != 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = leftline[hang] + Width_P_R_R * track_width[hang];
      }
      //   全丢
      else if ((leftlineflag[hang] == 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = Col - 2;
        leftline[hang] = 1;
      }
    }
    else if (sancha_flag == 2)
    { //走三叉内部专用
      if (rightlineflag[hang] != 0)
      {
        leftline[hang] = rightline[hang] - Width_shancha_zhong * track_width[hang]; //权值改一下
        // if (leftline[hang] < 0)
        //   leftline[hang] = 0;
      }
      // cout<<"施工区补线"<<endl;
    }
    else if (zsh_construction_flag == CONSTRUCTION_IN)
    {                               //走施工区内部专用
      if (rightlineflag[hang] != 0) //右线补左线
      {
        leftline[hang] = rightline[hang] - Width_shigongqu_ru * track_width[hang]; //权值改一下
        // if (leftline[hang] < 0)
        //   leftline[hang] = 0;
      }
      // cout<<"施工区补线"<<endl;
    }
    else //有改动
    {
      //左丢又不丢
      if ((leftlineflag[hang] == 0) && (rightlineflag[hang] != 0))
      {
        leftline[hang] = rightline[hang] - Width_P_W_L * track_width[hang];
      }
      //右丢左不丢
      else if ((leftlineflag[hang] != 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = leftline[hang] + Width_P_W_R * track_width[hang];
      }
      //全
      else if ((leftlineflag[hang] == 0) && (rightlineflag[hang] == 0))
      {
        rightline[hang] = Col - 2;
        leftline[hang] = 1;
      }
      // cout<<"补线了？？"<<endl;
    }

    midline[hang] = (leftline[hang] + rightline[hang]) / 2;
    if (midline[hang] > 119)
      midline[hang] = 119;
    if (midline[hang] < 0)
      midline[hang] = 0;
    // cout<<"hang == "<<hang<<endl;
    // if(hang == 53){
    //     cout<<"midline[53] == "<<midline[53]<<endl;
    //     cout<<"leftline[53] == "<<leftline[53]<<endl;
    //     cout<<"rightline[53] == "<<rightline[53]<<endl;
    // }
    // cout<<"midline[53] == "<<midline[53]<<endl;
    // cout<<leftline[hang]<<endl;
    // cout<<rightline[hang]<<endl;
    // cout<<midline[hang]<<endl;
    // cout<<rightline[hang] - leftline[hang]<<endl;                  //量赛道宽度
  }

  // cout<<"aaaaaaaaaaaaaaaaaa"<<endl;
}

/**************************************
连线函数
start da
end xiao
**************************************/
void connect(uint hang_start, uint hang_end, int p[])
{
  float temp = 0;
  // if((hang_end>0)&&(hang_start>0)&&(hang_start<=Row-1)&&(hang_end<=Row-1))    //防止数组数量不足卡死
  if (hang_start > hang_end)
  {

    temp = (float)(p[hang_end] - p[hang_start]) * 1.0 / (hang_start - hang_end);

    for (uint hang = hang_start; hang > hang_end - 1; hang--)
    {
      p[hang] = (int)(p[hang_start] + temp * (hang_start - hang));
    }
  }
  // cout<<"end"<<endl;
}
/**************************************
线性回归方程计算斜率
startline小
endline大
**************************************/
int regression(uint startline, uint endline, int p[])
{
  int i, B;
  int sumX = 0, sumY = 0, avrX = 0, avrY = 0;
  int num = 0, B_up1 = 0, B_up2 = 0, B_up, B_down;
  for (i = startline; i <= endline; i++)
  {
    num++;
    sumX += i;
    sumY += p[i];
  }
  avrX = sumX / num;
  avrY = sumY / num;
  B_up = 0;
  B_down = 0;
  for (i = startline; i <= endline; i++)
  {
    B_up1 = (p[i] - avrY);
    B_up2 = i - avrX;
    B_up += 10 * (B_up1 * B_up2);
    B_up = B_up / 100 * 100;
    B_down += 10 * (i - avrX) * (i - avrX);
  }
  if (B_down == 0)
    B = 0;
  else
    B = B_up * 25 / B_down;
  return B;
}