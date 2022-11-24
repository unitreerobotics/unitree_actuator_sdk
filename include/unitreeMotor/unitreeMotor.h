#ifndef __UNITREEMOTOR_H
#define __UNITREEMOTOR_H

#include "unitreeMotor/include/motor_msg.h"  // 电机通信协议
#include <stdint.h>
#include <iostream>

enum class MotorType{
    GO_M8010_6,   
};

struct MotorCmd{
	// 定义 发送格式化数据
    public:
        MotorType motorType = MotorType::GO_M8010_6;
	    int hex_len = 17;                   
        unsigned short id;              // 电机ID 0~14 15:广播ID 此时电机无返回
        unsigned short mode;            // 电机模式 0:刹车 1:FOC闭环 2:电机标定(发送后等待5sec,期间禁止给电机发送消息)
        float T;                        // 期望关节的输出力矩(电机转子转矩 N.m) 范围: ±127.99
        float W;                        // 期望关节速度(电机转子转速 rad/s)         ±804.00
        float Pos;                      // 期望关节位置(电机转子位置 rad)           ±411774
        float K_P;                      // 关节刚度系数                           0~25.599
        float K_W;                      // 关节速度系数                           0~25.599
        void modify_data(MotorCmd* motor_s); 
        uint8_t* get_motor_send_data();

    private:
        ControlData_t  motor_send_data;  //电机控制数据结构体，详见motor_msg.h
};

struct MotorData{
    // 定义 接收数据     
    public:
        MotorType motorType = MotorType::GO_M8010_6;
        int hex_len = 16;               // 接收的命令长度: 16Byte
        bool correct = false;           // 接收数据是否完整(true完整，false不完整或断联)
        unsigned char motor_id;         // 电机ID 0~14 15:广播ID 此时电机无返回
        unsigned char mode;             // 电机模式 0:刹车 1:FOC闭环 2:电机标定
        int Temp;                       // 温度 -50~127 ℃
        int MError;                     // 错误标志 0.正常 1.过热 2.过流 3.过压 4.编码器故障
        float T;                        // 关节的输出力矩(电机转子转矩 N.m) 范围: ±127.99
        float W;                        // 关节速度(电机转子转速 rad/s)         ±804.00
        float Pos;                      // 关节位置(电机转子位置 rad)           ±411774
        int footForce;                  // 足端气压传感器接口 ADC原始值
        bool extract_data(MotorData* motor_r);
        uint8_t* get_motor_recv_data();

    private:
        MotorData_t motor_recv_data;     //电机接收数据结构体，详见motor_msg.h

};


#endif  // UNITREEMOTOR_H