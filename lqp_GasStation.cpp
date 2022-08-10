/*
lqp_GasStation.c
code by lqp
date : 2022/08/09
*/
#include "lqp_GasStation.hpp"
using namespace std;
using namespace cv;
/****************************************/

// para
ZSH_GASSTATION zsh_GASSTATION_flag = GASSTATION_NULL; //施工区状态初始化
uint8_t ZSH_GASSTATION_SPEED = 170;
/****************************************/
// func extern
uint8_t GasStation_Init(void);
uint8_t GasStation_State_Judge(void);
uint8_t GasStation_Begin(void);
uint8_t GasStation_In(void);
uint8_t GasStation_Inside(void);
uint8_t GasStation_Out_1(void);
uint8_t GasStation_Out_2(void);
uint8_t GasStation_Reset(void);
uint8_t GasStation_NULL(void);
/****************************************/

uint8_t GasStation_Init(void)
{
}

void GasStation_State_Judge(void)
{
    switch (GasStation_flag)
    {
    case GASSTATION_BEGIN:
        GasStation_Begin();
        break;
    case GASSTATION_IN:
        GasStation_In();
        break;
    case GASSTATION_INSIDE:
        GasStation_Inside();
        break;
    case GASSTATION_OUT_1:
        GasStation_Out_1();
        break;
    case GASSTATION_OUT_2:
        GasStation_Out_2();
        break;
    case GASSTATION_RESET:
        GasStation_Reset();
        break;
    default:
        GasStation_NULL();
    }
}
