/*
zsh_construction.c
code by zsh
date : 2022/07/13
*/
#include "zsh_construction.h"
using namespace std;
using namespace cv;
/****************************************/
// para
ZSH_CONSTRUCTION zsh_construction_flag = CONSTRUCTION_NULL; //施工区状态初始化
uint8_t ZSH_CONSTRUCTION_SPEED = 170;
/****************************************/
// func extern
uint8_t zsh_construction_null(void);
uint8_t zsh_construction_begin(void);
uint8_t zsh_construction_in(void);
uint8_t zsh_construction_inside(void);
uint8_t zsh_construction_out(void);
uint8_t zsh_construction_reset(void);
/****************************************/

uint8_t zsh_construction_init(void)
{
}

uint8_t zsh_construction_deal(void)
{
    // cout << "zsh_construction_flag:" << zsh_construction_flag << endl;
    switch (zsh_construction_flag)
    {
    case CONSTRUCTION_BEGIN:
        zsh_construction_begin();
        break;
    case CONSTRUCTION_IN:
        zsh_construction_in();
        break;
    case CONSTRUCTION_INSIDE:
        zsh_construction_inside();
        break;
    case CONSTRUCTION_OUT:
        zsh_construction_out();
        break;
    case CONSTRUCTION_RESET:
        zsh_construction_reset();
        break;
    default:
        zsh_construction_null();
    }
}

uint8_t zsh_construction_null(void)
{
}

uint8_t zsh_construction_begin(void)
{
    if (1)
    {
        zsh_construction_flag = CONSTRUCTION_IN; //跳转到IN
    }
}

uint8_t zsh_construction_in(void)
{
    zsh_get_red_mask_pixle(red_mask);
    zsh_find_right_bottom_pixle();
    rightline[zshConstructionBeginaAxis_Y] = zshConstructionBeginaAxis_X;
    rightline[59] = 119;
    connect(59, zshConstructionBeginaAxis_Y, rightline);
    for (uint hang = 59; hang > zshConstructionBeginaAxis_Y; hang--)
    {
        leftlineflag[hang] = 0;
        rightlineflag[hang] = 1;
    }
    cout << "进入施工区" << endl;
    if (zshConstructionBeginaAxis_Y > 35 && zshConstructionBeginaAxis_X > 70)
    {
        zsh_construction_flag = CONSTRUCTION_INSIDE;
    }
}

uint8_t zsh_construction_inside(void)
{
    int _temp_1 = 0;
    int _temp_2 = 0;
    for (int j = 0; j < 6; j++)
    {
        for (int i = 30; i < 120; i++) // 90
        {
            if (Pixle[21 + j][i] == black)
            {
                _temp_1++;
            }
        }
        //
    }
    //  for (int i = 30; i < 75; i++)//90
    // {
    //     if (Pixle[25][i] == black)
    //     {
    //         _temp_1++;
    //     }
    // }
    cout << "_temp_1_:" << _temp_1 << endl;
    if (_temp_1 > 350)
    { // 26
        zsh_construction_flag = CONSTRUCTION_OUT;
    }
}

uint8_t zsh_construction_out(void)
{
    construction_out_cnt++;
    if (construction_out_cnt == 1)
    {
        for (uint hang = Row - 1; hang > 0; hang--) //直接确定左线和右线
        {
            leftline[hang] = 110;
            rightline[hang] = 119;
            leftlineflag[hang] = 1;
            rightlineflag[hang] = 1;
        }
    }

    else if (construction_out_cnt == 2)
    {
        usleep(380 * 1000);
    }
    else if (construction_out_cnt == 3)
    {
        for (uint hang = Row - 1; hang > 0; hang--) //直接确定左线和右线
        {
            leftline[hang] = 0;
            rightline[hang] = 119;
            leftlineflag[hang] = 1;
            rightlineflag[hang] = 1;
        }
    }

    if (construction_out_cnt == 4)
    {
        usleep(200 * 1000);
        zsh_construction_flag = CONSTRUCTION_NULL;
        construction_out_cnt = 0;
    }
}

uint8_t zsh_construction_reset(void)
{
}