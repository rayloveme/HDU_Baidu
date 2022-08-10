#include "uart_test.hpp"
#include <unistd.h>

extern unsigned char SPEED;
extern unsigned char ANGLE;
extern std::shared_ptr<Driver> driver;   //类初始化

int ser_Init(){                                    //定义串口初始化函数
    //USB转串口的设备名为/dev/ttyUSB0
    driver = std::make_shared<Driver>("/dev/ttyUSB0", BaudRate::BAUD_115200);
    if (driver == nullptr) {
    std::cout << "Create Driver Error ." << std::endl;
    return -1;
    }
    //串口初始化，打开串口设备及配置串口数据格式
    int ret = driver->open();
    if (ret != 0){
    std::cout << "Driver Open failed ." << std::endl;
    return -1;
    }
    return 0;
}

void send(){                                        //定义发送数据函数
    unsigned char *temp = new unsigned char[8];
    temp[0] = 0xA5;
    temp[1] = SPEED;//86H == 170
    temp[2] = ANGLE;//04H == 4-128 = -124
    temp[3] = 0xEE;
    for (int i = 0; i < 4; i++){
            driver->senddata(temp[i]);                 
    }
}