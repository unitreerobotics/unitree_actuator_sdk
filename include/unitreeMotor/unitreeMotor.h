#ifndef __UNITREEMOTOR_H
#define __UNITREEMOTOR_H

#include "unitreeMotor/include/motor_msg.h"  // 电机通信协议
#include <stdint.h>
#include <iostream>

enum class MotorType{
    GO_M8010_6
};

struct MotorCmd{
	// 定义 发送格式化数据
    public:
        MotorType motorType = MotorType::GO_M8010_6;
	    int hex_len = 17;                   
        unsigned short id;              //电机ID，0xBB代表全部电机
        unsigned short mode;            //0:空闲, 5:开环转动, 10:闭环FOC控制
        float T;                        //期望关节的输出力矩（电机本身的力矩）（Nm）
        float W;                        //期望关节速度（电机本身的速度）(rad/s)
        float Pos;                      //期望关节位置（rad）
        float K_P;                      //关节刚度系数
        float K_W;                      //关节速度系数
        void modify_data(MotorCmd* motor_s); 
        uint8_t* get_motor_send_data();

    private:
        ControlData_t  motor_send_data;  //电机控制数据结构体，详见motor_msg.h
};

struct MotorData{
    // 定义 接收数据
    public:
        MotorType motorType = MotorType::GO_M8010_6;
        int hex_len = 16;                    //接收的16进制命令数组长度, 78
        bool correct = false;                   //接收数据是否完整（true完整，false不完整）
        unsigned char motor_id;         //电机ID
        unsigned char mode;             //0:空闲, 5:开环转动, 10:闭环FOC控制
        int Temp;                       //温度
        int MError;           //错误码
        float T;                        // 当前实际电机输出力矩
        float W;                        // 当前实际电机速度（高速）
        float Pos;                      // 当前电机位置
        int footForce;
        bool extract_data(MotorData* motor_r);
        uint8_t* get_motor_recv_data();

    private:
        MotorData_t motor_recv_data;     //电机接收数据结构体，详见motor_msg.h

};


#endif  // UNITREEMOTOR_H