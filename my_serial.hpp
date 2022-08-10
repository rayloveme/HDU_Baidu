#pragma once

#include "uart_test.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include "zsh_construction.h"

// #define IMAGE_HEIGHT 60                    //图像高度
#define SERIAL_DATA_SIZE 10 //数据长度 10

#define SERIAL_HEAD 0xFF //定义帧头帧尾
#define SERIAL_TAIL 0xDD

extern unsigned char SPEED;
extern unsigned char ANGLE;
extern std::shared_ptr<Driver> driver; //类初始化
extern uint start_key;
extern uint end_key;
extern int ringflag;

extern int sancha_flag;
extern int sancha_send_flag;
extern int shigongqu_flag;
extern int shigongqu_send_flag;

extern int start_end_statues;
extern unsigned char START_SPEED;

extern int is_cheku;

extern int midline[Row];

extern uint xieflag;

class My_serial
{
private:
    std::unique_ptr<std::thread> ser_thread;
    bool _log_en;

public:
    //定义构造函数
    My_serial(bool logEn = false) : _log_en(logEn){};
    //定义析构函数
    ~My_serial(){};

    int ser_Init()
    { //定义串口初始化函数

        // USB转串口的设备名为/dev/ttyUSB0
        driver = std::make_shared<Driver>("/dev/ttyUSB0", BaudRate::BAUD_460800);
        if (driver == nullptr)
        {
            std::cout << "Create Driver Error ." << std::endl;
            return -1;
        }
        //串口初始化，打开串口设备及配置串口数据格式
        int ret = driver->open();

        if (ret != 0)
        {
            std::cout << "Driver Open failed ." << std::endl;
            return -1;
        }

        return 0;
    }

    unsigned char MODE_RUNNING = 0x00;
    unsigned char MODE_DEBUG = 0x01;

    /***************标志位***************/
    unsigned char ELE_STRAIGHT = 0x00;   //即无元素
    unsigned char ELE_LEFT_CURVE = 0x01; //普通弯道，后续可再添加其他标志位表示不同半径的弯道
    unsigned char ELE_RIGHT_CURVE = 0x02;
    unsigned char ELE_CROSS = 0x03;      //十字路口
    unsigned char ELE_LEFT_RING = 0x04;  //左环
    unsigned char ELE_RIGHT_RING = 0x05; //右环
    unsigned char ELE_RAMP = 0x06;       //坡道
    unsigned char ELE_PARK = 0x07;       //车库

    /***************识别元素Ai***************/
    unsigned char AI_ELE_NONE = 0x20;
    unsigned char AI_ELE_BRANCH = 0x21;
    unsigned char AI_ELE_CONSTRUCTION = 0x22;
    //坡道当前采用传统视觉标志位表示

    /***************接管标志位***************/
    unsigned char TAKE_NO = 0x40;
    unsigned char TAKE_ELE = 0x41;
    unsigned char TAKE_ALL = 0x42;

    unsigned char SLOW_SPEED = 240; //接管后慢速
    unsigned char sancha_SLOW_SPEED = 190;

    unsigned char *data_buff = new unsigned char[SERIAL_DATA_SIZE];

    struct PROTOCOL_DEAL_t
    {
        unsigned char data_buff[SERIAL_DATA_SIZE];

        unsigned char head_mode;
        unsigned char turns_num;
        unsigned char flag_byte;
        unsigned char ai_ele;
        unsigned char take_bype;
        unsigned char take_speed;
        unsigned char mid_err; //计算出来的偏差
        unsigned char mid_size;
        // unsigned char mid_data[IMAGE_HEIGHT];
    };

    void send()
    { //定义发送数据函数
        unsigned char _cnt = 0;
        unsigned char sumcheck = 0;
        unsigned char addcheck = 0;

        data_buff[_cnt++] = SERIAL_HEAD;
        data_buff[_cnt++] = MODE_RUNNING; //帧头模式
        data_buff[_cnt++] = 0x00;         //圈数

        if (ringflag == 1 || ringflag == 3 || ringflag == 6)
            data_buff[_cnt++] = ELE_LEFT_RING; //圆环元素
        else if (ringflag == 2 || ringflag == 4 || ringflag == 7)
            data_buff[_cnt++] = ELE_RIGHT_RING;
        else
            data_buff[_cnt++] = ELE_STRAIGHT; //直道

        if (sancha_flag)
            data_buff[_cnt++] = AI_ELE_BRANCH; //三叉元素
        else if (zsh_construction_flag != CONSTRUCTION_NULL)
            data_buff[_cnt++] = AI_ELE_CONSTRUCTION; //施工区
        else
            data_buff[_cnt++] = AI_ELE_NONE; //无元素

        if (cheku_flag == 3 || start_end_statues == 0)
            data_buff[_cnt++] = TAKE_ALL;
        else if (sancha_flag != 0 || ringflag != 0 || zsh_construction_flag != CONSTRUCTION_NULL || is_cheku == 1)
            data_buff[_cnt++] = TAKE_ELE; //元素接管下位机
        else
            data_buff[_cnt++] = TAKE_NO; //不接管下位机

        if (zsh_construction_flag != CONSTRUCTION_NULL)
            data_buff[_cnt++] = ZSH_CONSTRUCTION_SPEED; //接管速度
        // else if(sancha_flag!=0) data_buff[_cnt++] = sancha_SLOW_SPEED;
        // else if(start_end_statues == 0) data_buff[_cnt++] = START_SPEED;
        else
            data_buff[_cnt++] = SPEED;

        data_buff[_cnt++] = ANGLE;

        for (int i = 0; i < SERIAL_DATA_SIZE - 2; i++)
        {
            sumcheck += data_buff[i];
            addcheck += sumcheck;
        }
        data_buff[_cnt++] = addcheck;
        data_buff[_cnt++] = SERIAL_TAIL;

        for (int i = 0; i < SERIAL_DATA_SIZE; i++)
        {
            driver->senddata(data_buff[i]);
        }
    }

    void recv()
    { //定义接受数据函数
        unsigned char recv[3];
        size_t timeout_ms = 1; //阻塞时间3ms。
        int ret_ = driver->recvdata(recv[0], timeout_ms);
        if (recv[0] == 0xFF)
        {
            for (int i = 1; i < 3; i++)
            {
                driver->recvdata(recv[i], timeout_ms);
            }
        }
        if (recv[1] == 0x00)
        {
            start_key = 1;
            end_key = 0;
        }
        if (recv[1] == 0x01)
        {
            start_key = 0;
            end_key = 1;
        }
    }

    void start()
    {
        ser_thread = std::make_unique<std::thread>([this]() { //开启串口收发线程
            // clock_t start, finish;
            while (1)
            {
                // start = clock();
                send();
                recv();
                // finish = clock();
                // cout <<endl<<"the time cost is:" << double(finish - start) / CLOCKS_PER_SEC<<endl;
                usleep(10000);
                // cout<< "s = "<<start_key<<endl;
                // cout<<"e = "<< end_key <<endl;
            }
        });
    }
};