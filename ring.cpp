#include "Image_process.h"
#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

extern uint Pixle[Row][Col];
extern int leftline[Row];
extern int midline[Row];
extern int rightline[Row];

extern uint leftlineflag[Row];
extern uint rightlineflag[Row];

extern uint DropRow;
extern uint endline;

extern float line_error;

/*************************各种标志*******************************/
uint slope_L = 35, slope_R = 35, r_cnt0 = 2, r_startU = 26, r_startD = 45, r_error = 40;
int R_lost_cnt = 0, L_lost_cnt = 0, R_find_cnt = 0, L_find_cnt = 0, ALL_find_cnt = 0;
int ring_start_L = Row - 1, ring_start_R = Row - 1, ring_end_L = Row - 1, ring_end_R = Row - 1, ring_xie_L = Row - 1, ring_xie_R = Row - 1;
int ring_start_L_Flag = 0, ring_start_R_Flag = 0, ring_end_L_Flag = 0, ring_end_R_Flag = 0, ring_xie_L_Flag = 0, ring_xie_R_Flag = 0;
int ring_cnt = 0;
volatile int ringflag = 0;
int ring_tiaobian = 0;
int ring_white = 0, ring_white_di = 0;
int ring_B_L = 0, ring_B_R = 0;
int smallbigflag = 0;
int ring_previous = 0;
int ring_time = 0;

int chu_flag = 0;
int chu_cnt = 0;

int ringfind_zuo_cnt = 0;

int ringstarttime = 0;
void ring_start(void)
{
  if (ringflag == 0)
    ringstarttime = 0;
  if (ringflag == 6)
  {
    ringstarttime++;
    ring_start_L = Row - 1, ring_start_R = Row - 1;
    //左起始点
    for (uint hang = hstart - 2; hang > DropRow + 2; hang--)
    {
      if (leftlineflag[hang] != 0 && leftlineflag[hang - 1] == 0 && leftlineflag[hang - 2] == 0) //上面连续两点丢线
      {
        ring_start_L = hang;
        cout << "左起始点 = " << ring_start_L << endl;
        break;
      }
    }
    if (ringstarttime <= 30 || 1) //防止误判                                                 //          ||1   ！！！！！！！！
    {
      if (ring_start_L > r_startU && ring_start_L < r_startD) //调接行数来控制打�?  zuo  &&Error>45&&Error<75
      {
        ringflag = 1;
      }
    }
    else
      ringflag = 0;
  }
  if (ringflag == 7)
  {
    ringstarttime++;
    ring_start_L = Row - 1, ring_start_R = Row - 1;
    //右起始点
    for (uint hang = hstart - 2; hang > DropRow + 2; hang--)
    {
      if (rightlineflag[hang] != 0 && rightlineflag[hang - 1] == 0 && rightlineflag[hang - 2] == 0)
      {
        ring_start_R = hang;
        break;
      }
    }
    if (ringstarttime <= 30 || 1) //防止误判                                            //               ||1   ！！！！！！！！
    {
      if (ring_start_R > r_startU && ring_start_R < r_startD) //  //  you&&Error>38&&Error<72

      {
        ringflag = 2;
      }
    }
    else
      ringflag = 0;
  }
}

//圆环识别
void ring_find(void)
{
  R_lost_cnt = 0, L_lost_cnt = 0, ALL_find_cnt = 0, R_find_cnt = 0, L_find_cnt = 0; //变量初始�?
  ring_B_L = 0, ring_B_R = 0;
  ring_cnt = 0;
  ring_start_L = Row - 1, ring_start_R = Row - 1, ring_end_L = Row - 1, ring_end_R = Row - 1, ring_xie_L = Row - 1, ring_xie_R = Row - 1;
  ring_start_L_Flag = 0, ring_start_R_Flag = 0, ring_end_L_Flag = 0, ring_end_R_Flag = 0, ring_xie_L_Flag = 0, ring_xie_R_Flag = 0;
  ring_tiaobian = 0;
  ring_white = 0, ring_white_di = 0;

  //左起始点
  for (uint hang = Row - 2; hang > DropRow + 2; hang--)
  {
    if (leftlineflag[hang] != 0 && leftlineflag[hang - 1] == 0 && leftlineflag[hang - 2] == 0)
    {
      ring_start_L = hang;
      ring_start_L_Flag = 1;
      //  cout<<"ring_start_L = "<< ring_start_L<<endl;
      break;
    }
  }
  //左终�?
  if (ring_start_L_Flag == 1) //找到左起始点之后找左终点
  {
    //左终�?
    for (uint hang = ring_start_L; hang > DropRow; hang--)
    {
      if (leftlineflag[hang] == 0 && leftlineflag[hang - 1] == 0 && leftlineflag[hang - 2] != 0)
      {
        ring_end_L = hang;
        ring_end_L_Flag = 1;
        // cout<<"ring_end_L = "<<ring_end_L<<endl;
        break;
      }
    }
    //左斜�?
    for (uint hang = Row - 2; hang > ring_start_L; hang--) //左斜�?
    {
      if (leftlineflag[hang] != 0 && leftlineflag[hang - 1] != 0)
      {
        if (leftline[hang] - leftline[hang - 1] > 1)
        {
          ring_xie_L = hang;
          ring_xie_L_Flag = 1;
          // cout<<"ring_xie_L = "<<ring_xie_L<<endl;
          break;
        }
      }
    }
  }
  //右起始点
  for (uint hang = Row - 2; hang > DropRow + 2; hang--)
  {
    if (rightlineflag[hang] != 0 && rightlineflag[hang - 1] == 0 && rightlineflag[hang - 2] == 0)
    {
      ring_start_R = hang;
      ring_start_R_Flag = 1;
      // cout<<"ring_start_R"<<ring_start_R<<endl;
      break;
    }
  }
  //右终�?
  if (ring_start_R_Flag == 1)
  {
    //右终�?
    for (uint hang = ring_start_R; hang > DropRow; hang--)
    {
      if (rightlineflag[hang] == 0 && rightlineflag[hang - 1] == 0 && rightlineflag[hang - 2] != 0)
      {
        ring_end_R = hang;
        ring_end_R_Flag = 1;
        // cout<<"ring_end_R"<<ring_end_R<<endl;
        break;
      }
    }
    //右斜�?
    for (uint hang = Row - 2; hang > ring_start_R; hang--)
    {
      if (rightlineflag[hang] != 0 && rightlineflag[hang - 1] != 0)
        if (rightline[hang - 1] - rightline[hang] > 1)
        {
          ring_xie_R = hang;
          //  cout<<"ring_xie_R"<<ring_xie_R<<endl;
          ring_xie_R_Flag = 1;
          break;
        }
    }
  }

  //左圆�? 三点或者两点找到找�?
  /*
   * 找丢线情况，正常情况应该左面丢线，右面不丢线
   * 防止斜入十字，去找右面的点，不应该出现白�?
   * 防止小弯误判，向上找线，应该在一个合理的区间。小弯误判还要用到右线斜率，斜率不可过大。重要�?
   * 找跳变点，黑白跳变，应该是两�?
   */
  //左圆�? 三点都找�?
  if (ring_start_L_Flag == 1 && ring_xie_L_Flag == 1 && ring_start_R_Flag == 0 && ring_end_R_Flag == 0 && ring_xie_R_Flag == 0 && DropRow < 30 && line_error > 15 && line_error < 75) //
  {
    // cout<<"找到左边两个�?"<<endl;
    if (ring_end_L_Flag == 1) //小圆�?
    {
      // cout<<"找小圆环"<<endl;
      //入口左右丢线 左全丢右全不�?
      for (uint hang = ring_start_L; hang > ring_end_L; hang--)
      {
        if (rightlineflag[hang] == 0) //不应该出现的情况
          R_lost_cnt++;
        if (leftlineflag[hang] == 1)
          L_find_cnt++;
        //最底行白点数较�? 防止斜入十字误判
        for (uint lie = 110; lie < Col - 1; lie++)
        {
          if (Pixle[hang][lie] == white) //最右边不应该出现白�?
            ring_white++;
        }
      }
      //截止行向上找全线 防止小弯误判
      // for(uint hang=ring_end_L;hang>DropRow;hang--)                           //用了新截止行之后，想上找全线可能有问�?
      // {
      //   if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
      //     ALL_find_cnt++;
      // }
      for (uint hang = ring_start_L; hang > DropRow; hang--)
      {
        int R_find = 0, L_find = 0;
        for (uint lie = Col / 2; lie < Col - 3; lie++)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black && Pixle[hang][lie + 2] == black)
            R_find = 1;
        }
        for (uint lie = Col / 2; lie > 3; lie--)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black && Pixle[hang][lie - 2] == black)
            L_find = 1;
        }
        if (R_find == 1 && L_find == 1)
          ALL_find_cnt++;
      }
    }
    else //大圆�?
    {
      // cout<<"检测左大圆�?"<<endl;
      //入口左右丢线 左全丢右全不�?
      for (uint hang = ring_start_L; hang > ring_start_L - 5; hang--)
      {
        if (rightlineflag[hang] == 0)
          R_lost_cnt++;
        if (leftlineflag[hang] == 1)
          L_find_cnt++;
        //最底行白点数较�? 防止斜入十字误判
        for (uint lie = 110; lie < Col - 1; lie++)
        {
          if (Pixle[hang][lie] == white)
            ring_white++;
        }
      }
      //截止行向上找全线 防止小弯误判
      // for(uint hang=DropRow+10;hang>DropRow;hang--)
      // {
      //   if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
      //     ALL_find_cnt++;
      // }
      for (uint hang = ring_start_L; hang > DropRow; hang--)
      {
        int R_find = 0, L_find = 0;
        for (uint lie = Col / 2; lie < Col - 3; lie++)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black && Pixle[hang][lie + 2] == black)
            R_find = 1;
        }
        for (uint lie = Col / 2; lie > 3; lie--)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black && Pixle[hang][lie - 2] == black)
            L_find = 1;
        }
        if (R_find == 1 && L_find == 1)
          ALL_find_cnt++;
      }
    }
    //最底行白点数较�? 防止斜入十字误判
    for (uint hang = 35; hang > 30; hang--) //最右边不应该出现白�?
    {
      for (uint lie = 110; lie < Col - 1; lie++)
      {
        if (Pixle[hang][lie] == white)
          ring_white_di++;
      }
    }
    //找跳�? 白黑黑白跳变 圆环第一特征
    for (uint hang = ring_xie_L; hang > DropRow; hang--)
    {
      if (Pixle[hang][leftline[ring_xie_L] / 2] == black && Pixle[hang - 1][leftline[ring_xie_L] / 2] == white)
        ring_tiaobian++;
      if (Pixle[hang][leftline[ring_xie_L] / 2] == white && Pixle[hang - 1][leftline[ring_xie_L] / 2] == black)
        ring_tiaobian++;
    }

    //进一步检�?
    //判断条件找小环标志位
    // if(ring_tiaobian>=2&& R_lost_cnt<=1&&L_find_cnt<=2
    //         &&ring_white<15&&ring_white_di<49
    //         &&ALL_find_cnt>=12&&ring_start_L-ring_end_L>=2)//&&ring_start_L-ring_end_L>=2                                                ？？�?
    // if(ALL_find_cnt>=5)   cout<<"ALL_find_cnt满足"<<endl;
    // if(R_lost_cnt<=1)   cout<<"R_lost_cnt<=1满足"<<endl;
    // if(L_find_cnt<=1)   cout<<"L_find_cnt<=1满足"<<endl;
    if (ring_tiaobian >= 2 && R_lost_cnt <= 1 && L_find_cnt <= 2 && ring_white < 15 && ring_white_di < 49 && ALL_find_cnt >= 4) //&&ring_start_L-ring_end_L>=2
    {
      // cout<<"进一步检测判断小环标志位"<<endl;
      for (uint hang = ring_start_L; hang < ring_start_L + 8; hang++) //向下�?8�?
      {
        if ((leftline[hang] - leftline[hang + 1]) < 0)                                            //    "\"这个方向
          if ((leftline[hang] - leftline[hang + 1]) <= (leftline[hang + 1] - leftline[hang + 2])) //差值逐渐增大
            ring_cnt++;
      }
      ring_B_R = regression(DropRow, Row - 2, rightline);

      if (ring_cnt >= r_cnt0 && ring_B_R < slope_L)
      {
        ringflag = 6;
      }
    }
  }
  // cout<<"DropRow == "<<DropRow<<endl;
  // cout<<"line_error == "<<line_error<<endl;

  //右圆�?
  if (ring_start_L_Flag == 0 && ring_end_L_Flag == 0 && ring_xie_L_Flag == 0 && ring_start_R_Flag == 1 && ring_xie_R_Flag == 1 && DropRow < 30 && line_error > 15 && line_error < 75) //
  {
    cout << "11111111" << endl;
    if (ring_end_R_Flag == 1)
    {
      cout << "222222222" << endl;
      //入口左右丢线 左全丢右全不�?
      for (uint hang = ring_start_R; hang > ring_end_R; hang--)
      {
        if (rightlineflag[hang] == 1)
          R_find_cnt++;
        if (leftlineflag[hang] == 0)
          L_lost_cnt++;
        for (uint lie = 10; lie > 0; lie--)
        {
          if (Pixle[hang][lie] == white)
            ring_white++;
        }
      }
      //截止行向上找全线 防止小弯误判
      //  for(uint hang=ring_end_R;hang>DropRow;hang--)
      //  {
      //    if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
      //      ALL_find_cnt++;
      //  }
      for (uint hang = ring_start_R; hang > DropRow; hang--)
      {
        int R_find = 0, L_find = 0;
        for (uint lie = Col / 2; lie < Col - 3; lie++)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black && Pixle[hang][lie + 2] == black)
            R_find = 1;
        }
        for (uint lie = Col / 2; lie > 3; lie--)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black && Pixle[hang][lie - 2] == black)
            L_find = 1;
        }
        if (R_find == 1 && L_find == 1)
          ALL_find_cnt++;
      }
    }
    else
    { //是否是减五，以后再调
      cout << "检测右大圆�?" << endl;
      for (uint hang = ring_start_R; hang > ring_start_R - 5; hang--)
      {
        if (rightlineflag[hang] == 1)
          R_find_cnt++;
        if (leftlineflag[hang] == 0)
          L_lost_cnt++;
        for (uint lie = 10; lie > 0; lie--)
        {
          if (Pixle[hang][lie] == white)
            ring_white++;
        }
      }
      //截止行向上找全线 防止小弯误判
      // for(uint hang=DropRow+10;hang>DropRow;hang--)
      // {
      //   if(leftlineflag[hang]==1&&rightlineflag[hang]==1)
      //     ALL_find_cnt++;
      // }
      for (uint hang = ring_start_R; hang > DropRow; hang--)
      {
        int R_find = 0, L_find = 0;
        for (uint lie = Col / 2; lie < Col - 3; lie++)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie + 1] == black && Pixle[hang][lie + 2] == black)
            R_find = 1;
        }
        for (uint lie = Col / 2; lie > 3; lie--)
        {
          if (Pixle[hang][lie] == white && Pixle[hang][lie - 1] == black && Pixle[hang][lie - 2] == black)
            L_find = 1;
        }
        if (R_find == 1 && L_find == 1)
          ALL_find_cnt++;
      }
    }

    //最底行白点数较�? 防止斜入十字误判
    for (uint hang = 35; hang > 30; hang--)
    {
      for (uint lie = 10; lie > 0; lie--)
      {
        if (Pixle[hang][lie] == white)
          ring_white_di++;
      }
    }
    //找跳�? 白黑黑白跳变 圆环第一特征
    for (uint hang = ring_xie_R; hang > DropRow; hang--)
    {
      if (Pixle[hang][(Col + rightline[ring_xie_R]) / 2] == black && Pixle[hang - 1][(Col + rightline[ring_xie_R]) / 2] == white)
        ring_tiaobian++;
      if (Pixle[hang][(Col + rightline[ring_xie_R]) / 2] == white && Pixle[hang - 1][(Col + rightline[ring_xie_R]) / 2] == black)
        ring_tiaobian++;
    }

    cout << R_find_cnt << " " << L_lost_cnt << " " << ring_white << " " << ALL_find_cnt << " " << ring_white_di << endl;
    //判断条件找小环标志位
    if (ring_tiaobian >= 2 && R_find_cnt <= 1 &&
        L_lost_cnt <= 2 && ring_white < 25 && ALL_find_cnt >= 10 && ring_white_di < 49) // 小弯误判�?4->8
    {
      for (uint hang = ring_start_R; hang < ring_start_R + 8; hang++)
      {
        if ((rightline[hang + 1] - rightline[hang]) < 0)
          if ((rightline[hang + 1] - rightline[hang]) <= (rightline[hang + 2] - rightline[hang + 1]))
            ring_cnt++;
      }
      ring_B_L = regression(DropRow, Row - 2, leftline);

      cout << " ring_cnt == " << ring_cnt << endl;
      cout << " ring_B_L == " << ring_B_L << endl;

      if (ring_cnt >= r_cnt0 && ring_B_L > -slope_R)
      {
        ringflag = 7;
      }
    }
  }
}
uint ring_R = Col - 1, ring_L = Col - 1;
uint xieflag = 0;
uint ring_hang_last = 0;
//入圆环补�?
void ring_ru(void)
{
  //左圆�?
  if (ringflag == 1)
  {
    ring_R = Col / 2, ring_L = Col / 2;

    ring_end_L = Row - 1, ring_end_L_Flag = 0;
    uint ring_hang_L[Col];
    //    uint ru_bed_point=0; //默认合格，检查不合格的点
    xieflag = 0;
    //找终�?
    for (uint hang = Row - 2; hang > endline + 3; hang--)
    {
      if (leftlineflag[hang] == 0 && leftlineflag[hang - 1] == 0 && leftlineflag[hang - 2] != 0)
      {
        ring_end_L = hang;
        ring_end_L_Flag = 1;
        break;
      }
    }
    //截止行查�?
    //找左边白点数
    int lost = 0;
    for (uint hang = 55; hang > 45; hang--) //找最底下10行，左线的丢线数
    {
      if (leftlineflag[hang] == 0 || leftline[hang] < 5)
      {
        lost++;
      }
    }
    if (lost >= 7)
    {
      // cout<<"截止行标�?"<<endl;
      //截止行标�? (可行度极�?)
      for (uint hang = 45; hang > 5; hang--) // 50 .以第0列为截止行参考列
      {
        if (Pixle[hang][30] == black && Pixle[hang + 1][30] == black && Pixle[hang + 2][30] == white)
        {
          endline = hang + 2;
          break;
        }
        // cout<<"截止�? = "<< endline <<endl;
      }
    }
    else
    {
      //截止行标�? (可行度极�?)
      for (uint hang = 45; hang > 5; hang--) // 50 ,以第45列为截止行参考列   原本搜到endline，这里直接改�?5，防止原始endline的影�?
      {
        if (Pixle[hang][45] == black && Pixle[hang + 1][45] == black && Pixle[hang + 2][45] == white)
        {
          endline = hang + 2;
          // cout<<"截止�? = "<< endline <<endl;
          break;
        }
      }
    }

    for (uint lie = 119; lie > 60; lie--)
    {
      for (uint hang = 55; hang > endline; hang--)
      {
        if (Pixle[hang][lie] == black)
        {
          ring_hang_L[lie] = hang; //这里是lie->hang
          break;
        }
        else
        {
          ring_hang_L[lie] = 0;
        }
      }
    }

    for (uint lie = 110; lie > 60; lie--)
    {
      if (ring_hang_L[118] == 0 && ring_hang_L[117] == 0) // 118�? & 117�? 找不到黑�?
      {
        if (ring_hang_L[lie] != 0 && ring_hang_L[lie - 1] != 0) //最先找到黑点的�?
        {
          ring_L = lie;
          break;
        }
      }
    }

    if (ring_L > 100) //最先看到黑点的列大�?100�?   //90
      ringflag = 3;

    //连线
    rightline[endline] = (int)(119 - endline * 2.7); // 90

    if (rightline[endline] < 0)
    {
      rightline[endline] = 0;
    }
    if (rightline[endline] > 120)
    {
      rightline[endline] = 120;
    }
    // rightline[endline]=10;//90                                                �?
    // cout<<"rightline[endline] = "<<rightline[endline]<<endl;
    rightline[59] = 99;
    connect(59, endline, rightline);

    // for(int i = 59; i>endline; i--){
    //   cout<< rightline[i]<<endl;
    // }
    // cout<<"end"<<endl;

    for (uint hang = 59; hang > endline; hang--)
    {
      rightlineflag[hang] = 1;
      leftlineflag[hang] = 1;
      leftline[hang] = 0;
    }
  }
  //右圆�?
  else if (ringflag == 2)
  {
    // cout<<"检测到右圆�?"<<endl;
    ring_R = Col / 2, ring_L = Col / 2;
    ring_end_R = Row - 1, ring_end_R_Flag = 0;
    uint ring_hang_R[Col];
    xieflag = 0;
    for (uint hang = Row - 2; hang > endline + 3; hang--)
    {
      if (rightlineflag[hang] == 0 && rightlineflag[hang - 1] == 0 && rightlineflag[hang - 2] != 0)
      {
        ring_end_R = hang;
        ring_end_R_Flag = 1;
        break;
      }
    }
    //截止行查�?
    //找左边白点数
    int lost = 0;
    for (uint hang = 55; hang > 45; hang--)
    {
      if (rightlineflag[hang] == 0 || rightline[hang] > Col - 5)
      {
        lost++;
      }
    }
    if (lost >= 7)
    {
      //截止行标�? (可行度极�?)
      for (uint hang = 50; hang > endline; hang--)
      {
        if (Pixle[hang][Col - 20] == black && Pixle[hang + 1][Col - 20] == black)
        {
          endline = hang + 2;
          break;
        }
      }
    }
    else
    {
      //截止行标�? (可行度极�?)
      for (uint hang = 50; hang > endline; hang--)
      {
        if (Pixle[hang][Col - 45] == black && Pixle[hang + 1][Col - 45] == black)
        {
          endline = hang + 2;
          break;
        }
      }
    }

    //找拐点清标志�?
    //原始图像列扫�? 局部不正确有些点没找到 以后优化
    for (uint lie = 0; lie < 60; lie++)
    {
      for (uint hang = 55; hang > endline; hang--)
      {
        if (Pixle[hang][lie] == black)
        {
          ring_hang_R[lie] = hang;
          break;
        }
        else
        {
          ring_hang_R[lie] = 0;
        }
      }
    }

    for (uint lie = 10; lie < 60; lie++)
    {
      if (ring_hang_R[2] == 0 && ring_hang_R[3] == 0)
        if (ring_hang_R[lie] != 0 && ring_hang_R[lie + 1] != 0)
        {
          ring_R = lie;
          break;
        }
    }
    if (ring_R < 30) // 30
      ringflag = 4;
    //连线
    leftline[endline] = (int)(0 + endline * 2.9); // 30
    if (leftline[endline] >= 119)
    {
      leftline[endline] = 119;
    }
    if (leftline[endline] < 0)
    {
      leftline[endline] = 0;
    }
    // leftline[endline]= 110;
    leftline[59] = 40;
    connect(59, endline, leftline); // 59
    for (uint hang = 59; hang > endline; hang--)
    {
      leftlineflag[hang] = 1;
      rightlineflag[hang] = 1;
      rightline[hang] = Col - 1; //这里注意
    }
  }
}

void ring_chu(void)
{

  //左圆�?
  if (ringflag == 3)
  {
    // cout<<"出圆�?"<<endl;
    uint ring_xie_R = 0;
    uint ring_chu_num = 0;
    uint ring_end_L = Row - 1;
    uint R_lost_cnt = 0, ALL_find_cnt = 0, L_find_cnt = 0;
    uint ring_kong[Row] = {0};
    ring_end_L_Flag = 0;

    uint endline_l = 5;
    uint DropRow_l = 0;

    if (DropRow < 12)
      DropRow_l = 12;
    else
      DropRow_l = DropRow;

    if (DropRow_l < 5)
      DropRow_l = 5;

    if (chu_flag == 0)
    {
      //右线右拐
      for (uint hang = Row - 2; hang > DropRow_l; hang--) //找到右边的那个斜�?
      {
        if ((rightline[hang - 2] - rightline[hang] > 1 || rightline[hang - 4] - rightline[hang] > 1) && (rightline[hang + 3] - rightline[hang] > 1)) // &&rightlineflag[hang-1]==1&&rightlineflag[hang]==1
        {
          // cout<<"111"<<endl;
          // if(hang>5 && hang<55 && rightline[hang]<117 && Pixle[hang][rightline[hang]+2] == black
          // && (Pixle[hang-4][rightline[hang]] == white || Pixle[hang-2][rightline[hang]] == white) && Pixle[hang+4][rightline[hang]] == white)   cout<<"111"<<endl;
          if (hang > 5 && hang < 55 && rightline[hang] < 117 && Pixle[hang][rightline[hang] + 2] == black && (Pixle[hang - 4][rightline[hang]] == white || Pixle[hang - 2][rightline[hang]] == white) && Pixle[hang + 4][rightline[hang]] == white) //点周围的点的颜色
          {
            ring_xie_R = hang;
            cout << "找到斜点ring_xie_R == " << ring_xie_R << endl;
            // if(ring_xie_R > endline){
            endline = ring_xie_R;
            // }
            if (rightline[hang] > 15)
            { // 25
              xieflag = 1;
            }
            break;
          }
        }
      }

      if ((ring_xie_R == 0 || ring_xie_R > 35) && xieflag != 0) //对斜点的要求�?        ring_xie_R>35
      {
        for (uint hang = Row - 5; hang > endline_l; hang--)
        {
          if (Pixle[hang][30] == white && Pixle[hang - 1][30] == black && Pixle[hang - 2][30] == black)
          {
            endline_l = hang;
          }
        }
        for (uint hang = Row - 8; hang > DropRow; hang--)
        {
          for (uint lie = 0; lie < Col; lie++)
          {
            if (Pixle[hang][lie] == white) //&&(leftline[hang]<5||leftlineflag[hang]==0)&&(rightline[hang]>Col-5||rightlineflag[hang]==0)
              ring_kong[hang]++;           //这点为白色，所在行的左右两边都丢线
          }
          if (ring_kong[hang] > Col - 8) //因为可能出现左下弯道，范围变宽一�?      ！！！！
            ring_chu_num++;              //这一行几乎全部为白色
        }

        // cout<<"找斜�?"<<endl;
        if (ring_chu_num >= 5) //补线取决条件是全部为白色的行大于5      ！！！！
        {
          chu_flag = 1;
        }
      }
    }
    else if (chu_flag == 1)
    {
      // int close_hang = 20;
      // for(uint hang=Row-1;hang>0;hang--){
      //   if(Pixle[hang][30]==white&&Pixle[hang-1][30]==black&&Pixle[hang-2][30]==black){
      //     close_hang = hang;
      //     break;
      //   }
      // }
      for (uint hang = Row - 1; hang > endline; hang--) //直接确定左线和右�?
      {
        leftline[hang] = 0;
        // rightline[hang] = 60-close_hang;//75-endline;//110              !!!
        rightline[hang] = 10;
        leftlineflag[hang] = 1;
        rightlineflag[hang] = 1;
      }
      chu_cnt++;
      if (chu_cnt > 10)
      {
        ringflag = 5; //切换回寻线状�?
        chu_flag = 0; //计数器清�?
        chu_cnt = 0;
      }
    }
  }
  //右圆�?
  else if (ringflag == 4)
  {
    uint ring_xie_L = 0;
    uint ring_chu_num = 0;
    uint ring_end_R = Row - 1;
    uint L_lost_cnt = 0, ALL_find_cnt = 0, R_find_cnt = 0;
    uint ring_kong[Row] = {0};
    ring_end_R_Flag = 0;
    uint endline_r = 5; //后加�?
    uint DropRow_r = 0;

    if (DropRow < 20)
      DropRow_r = 20;
    else
      DropRow_r = DropRow;

    if (DropRow_r < 5)
      DropRow_r = 5;

    if (chu_flag == 0)
    {
      //左斜�?
      for (uint hang = Row - 2; hang > DropRow_r; hang--)
      {
        if ((leftline[hang] - leftline[hang - 2] > 1 || leftline[hang] - leftline[hang - 4] > 1) && leftline[hang] - leftline[hang + 3] > 1)
          // if(leftlineflag[hang]==1&&leftlineflag[hang-1]==1)
          if (hang > 5 && hang < 55 && leftline[hang] > 2 && Pixle[hang][leftline[hang] - 2] == black && (Pixle[hang - 2][leftline[hang]] == white || Pixle[hang - 4][leftline[hang]]) == white && Pixle[hang + 4][leftline[hang]] == white)
          {
            ring_xie_L = hang;
            cout << "ring_xie_L == " << ring_xie_L << endl;
            // if(ring_xie_L > endline){
            endline = ring_xie_L;
            // }
            if (leftline[hang] < 52)
            {
              xieflag = 1;
            }
            break;
          }
      }
      if ((ring_xie_L == 0 || ring_xie_L > 30) && xieflag != 0) // ring_xie_L>35     //找到斜点  ring_xie_L==0||
      {
        for (uint hang = Row - 5; hang > endline_r; hang--)
        {
          if (Pixle[hang][90] == white && Pixle[hang - 1][90] == black && Pixle[hang - 2][90] == black)
          {
            endline_r = hang;
          }
        }
        for (uint hang = Row - 5; hang > DropRow; hang--)
        {
          for (uint lie = 0; lie < Col - 1; lie++)
          {
            if (Pixle[hang][lie] == white) //&&(leftline[hang]<5||leftlineflag[hang]==0)&&(rightline[hang]>Col-5||rightlineflag[hang]==0)
              ring_kong[hang]++;
          }
          if (ring_kong[hang] > Col - 10) // 5
            ring_chu_num++;
        }
        cout << "ring_chu_num == " << ring_chu_num << endl;
        if (ring_chu_num >= 4) // 5
        {
          chu_flag = 1;
        }
      }
    }
    else if (chu_flag == 1)
    {
      // int close_hang = 20;
      // for(uint hang=Row-1;hang>0;hang--){
      //   if(Pixle[hang][90]==white&&Pixle[hang-1][90]==black&&Pixle[hang-2][90]==black){
      //     close_hang = hang;
      //     break;
      //   }
      // }
      for (uint hang = Row - 1; hang > endline; hang--) //直接确定左右�?
      {
        // leftline[hang]=60+close_hang;                      //45
        leftline[hang] = 105;
        rightline[hang] = Col - 1;
        leftlineflag[hang] = 1;
        rightlineflag[hang] = 1;
      }
      chu_cnt++;
      if (chu_cnt > 10)
      {
        ringflag = 5; //切换回寻线状�?
        chu_flag = 0; //计数器清�?
        chu_cnt = 0;
      }
    }
  }
  if (ringflag == 5)
  {
    ring_time++;
    if (ring_time > 15) // 25  20   3 2 1?
    {
      ringflag = 0;
      ring_time = 0;
    }
  }
}