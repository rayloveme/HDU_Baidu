#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Image_process.h"


using namespace std;

extern int sancha_flag;                 
extern uint Pixle[Row][Col];
extern unsigned char SPEED;
extern uint endline;

extern int leftline[Row];
extern int midline[Row];
extern int rightline[Row];

extern uint leftlineflag[Row];
extern uint rightlineflag[Row];

void sancha_ru(){
    for(uint hang=Row-1;hang>endline;hang--)                //直接确定左线和右�?
    {
        leftline[hang]=20;                          //20
        rightline[hang] = 119;           
        leftlineflag[hang]=1;
        rightlineflag[hang]=1;
    }
    // uint sancha_white = 0;
    // for(uint hang = 59; hang > 30; hang--){                  //找最右边两列、下30行的白点
    //     for(uint lie = 105; lie>=104 ;lie--){
    //         if(Pixle[hang][lie] == white)   sancha_white++;
    //     }
    // }
    uint sancha_di_black = 0;
    for(uint hang = 57; hang >= 55; hang--){                  //找最左边两列、下30行的黑点
        for(uint lie = 0; lie<60 ;lie++){
            if(Pixle[hang][lie] == black)   sancha_di_black++;
        }
    }

    uint sancha_black = 0;
    for(uint hang = 59; hang > 30; hang--){                  //找最左边两列、下30行的黑点
        for(uint lie = 40; lie<=41 ;lie++){
            if(Pixle[hang][lie] == black)   sancha_black++;
        }
    }

    
    cout<< sancha_black <<"  "<< sancha_di_black;
    if(sancha_black > 55 && sancha_di_black > 165){
        sancha_flag = 2;
        cout<<"进入三叉"<<endl;
    }

}

void sancha_switch(){
    for(int hang = 0; hang<Row - 1; hang++){        //对图片进行黑白转�?
        for(int lie = 0; lie<Col - 1; lie++){
            if(Pixle[hang][lie] == black){
                Pixle[hang][lie] = white;
            }
            else{
                Pixle[hang][lie] = black; 
            }
        }
    }
}

void sancha_zhong(){            //三叉中看看什么时候该出去�?
    // int you_xielv = regression(endline, Row - 2, rightline);        //计算右线斜率
    // cout<<"右线斜率 == "<< you_xielv<<endl;
    cout<<"截止�? == "<< endline<<endl;

    uint white_hang = 0;    //白行
    uint white_dot = 0;     //白点
    int you_xie_flag = 0;

    for(uint hang = endline - 3; hang > endline - 8; hang--){       //截止行上边的行，左半部分必定是白色的
        for(uint lie = 0; lie<60; lie++){
            if(Pixle[hang][lie] == black){                          //这里做了黑白变换，所以是black
                white_dot++;
            }
        }
        cout<<"白点 == "<<white_dot<<endl;
        if(white_dot > 58){
            white_hang++;
        }
        white_dot = 0;
    }
    cout<<"白行 =="<<white_hang<<endl;

    for(uint hang = 55;hang>50;hang--){                                                     //出三叉的时候右线向内收
        if(rightline[hang]-rightline[hang-1]>1 && rightline[hang-1]-rightline[hang-2]>1)                
            if(rightlineflag[hang]!=0&&rightlineflag[hang-1]!=0&&rightlineflag[hang-2]!=0){
                you_xie_flag = 1;                                                              //用计数器会更好？
                cout<<"出三叉：：右线下方向左倾斜"<<endl;
            }
    }

    if(endline > 45 && white_hang > 1 && you_xie_flag == 1){
        sancha_flag = 3;
    }
}

void sancha_chu(){
    for(uint hang=Row-1;hang>endline;hang--)                //直接确定左线和右�?
    {
        leftline[hang]=30;                  //35
        rightline[hang] = 119;           
        leftlineflag[hang]=1;
        rightlineflag[hang]=1;
    }

    uint sancha_chu_black = 0;
    for(uint hang = 59; hang > 56; hang--){                  //找最下面5行、中�?110列的黑点
        for(uint lie = 15; lie<95 ;lie++){
            if(Pixle[hang][lie] == black)   sancha_chu_black++;
        }
    }
    cout<<"sancha_chu_black == "<<sancha_chu_black<<endl;
    if(sancha_chu_black < 10){       //图像下面几乎不存在黑点，就认为是出了三叉�?
        sancha_flag = 0;
    }
}