#ifndef UNITREEMOTOR_H
#define UNITREEMOTOR_H

#include "unitreeMotor/include/motor_msg.h"  // 电机通信协议
#include "CRC/crc32.h"      // CRC32校验算法
#include <stdint.h>
#include <iostream>

enum class MotorType{
    A1,      // 4.8M baudrate, K_W x1024
    B1          // 6.0M baudrate, K_W x512
};

struct MOTOR_send{
	// 定义 发送格式化数据
    MasterComdDataV3  motor_send_data;  //电机控制数据结构体，详见motor_msg.h
    MotorType motorType = MotorType::A1;
	int hex_len = 34;                    //发送的16进制命令数组长度, 34
    // long long send_time;            //发送该命令的时间, 微秒(us)
    // 待发送的各项数据
    unsigned short id;              //电机ID，0xBB代表全部电机
    unsigned short mode;            //0:空闲, 5:开环转动, 10:闭环FOC控制
    //实际给FOC的指令力矩为：
    //K_P*delta_Pos + K_W*delta_W + T
    float T;                        //期望关节的输出力矩（电机本身的力矩）（Nm）
    float W;                        //期望关节速度（电机本身的速度）(rad/s)
    float Pos;                      //期望关节位置（rad）
    float K_P;                      //关节刚度系数
    float K_W;                      //关节速度系数
    COMData32 Res;                  // 通讯 保留字节  用于实现别的一些通讯内容
};

struct MOTOR_recv{
    // 定义 接收数据
    ServoComdDataV3 motor_recv_data;     //电机接收数据结构体，详见motor_msg.h
    MotorType motorType = MotorType::A1;
    int hex_len = 78;                    //接收的16进制命令数组长度, 78
    // long long resv_time;            //接收该命令的时间, 微秒(us)
    bool correct = false;                   //接收数据是否完整（true完整，false不完整）
    //解读得出的电机数据
    unsigned char motor_id;         //电机ID
    unsigned char mode;             //0:空闲, 5:开环转动, 10:闭环FOC控制
    int Temp;                       //温度
    unsigned char MError;           //错误码

    float T;                        // 当前实际电机输出力矩
    float W;                        // 当前实际电机速度（高速）
    float LW;                       // 当前实际电机速度（低速）
    int Acc;                      // 电机转子加速度
    float Pos;                      // 当前电机位置（主控0点修正，电机关节还是以编码器0点为准）

    float gyro[3];                  // 电机驱动板6轴传感器数据
    float acc[3];
};

inline void modify_data(MOTOR_send* motor_s){
    motor_s->hex_len = 34;
    motor_s->motor_send_data.head.start[0] = 0xFE;
    motor_s->motor_send_data.head.start[1] = 0xEE;
    motor_s->motor_send_data.head.motorID = motor_s->id;
    motor_s->motor_send_data.head.reserved = 0x0;
    motor_s->motor_send_data.Mdata.mode = motor_s->mode;
    motor_s->motor_send_data.Mdata.ModifyBit = 0xFF;
    motor_s->motor_send_data.Mdata.ReadBit = 0x0;
    motor_s->motor_send_data.Mdata.reserved = 0x0;
    motor_s->motor_send_data.Mdata.Modify.L = 0;
    motor_s->motor_send_data.Mdata.T = motor_s->T*256;
    motor_s->motor_send_data.Mdata.W = motor_s->W*128;
    motor_s->motor_send_data.Mdata.Pos = (int)((motor_s->Pos/6.2832)*16384.0);
    motor_s->motor_send_data.Mdata.K_P = motor_s->K_P*2048;
    
    if(motor_s->motorType == MotorType::A1){
        motor_s->motor_send_data.Mdata.K_W = motor_s->K_W*1024;
    }
    else if(motor_s->motorType == MotorType::B1){
        motor_s->motor_send_data.Mdata.K_W = motor_s->K_W*512;       
    }
    
    motor_s->motor_send_data.Mdata.LowHzMotorCmdIndex = 0;
    motor_s->motor_send_data.Mdata.LowHzMotorCmdByte = 0;
    motor_s->motor_send_data.Mdata.Res[0] = motor_s->Res;
    motor_s->motor_send_data.CRCdata.u32 = crc32_core((uint32_t*)(&(motor_s->motor_send_data)), 7);
}

inline bool extract_data(MOTOR_recv* motor_r){
    if(motor_r->motor_recv_data.CRCdata.u32 !=
        crc32_core((uint32_t*)(&(motor_r->motor_recv_data)), 18)){
        std::cout << "[WARNING] Receive data CRC error" << std::endl;
        motor_r->correct = false;
        return motor_r->correct;
    }else{
        motor_r->motor_id = motor_r->motor_recv_data.head.motorID;
        motor_r->mode = motor_r->motor_recv_data.Mdata.mode;
        motor_r->Temp = motor_r->motor_recv_data.Mdata.Temp;
        motor_r->MError = motor_r->motor_recv_data.Mdata.MError;
        motor_r->T = ((float)motor_r->motor_recv_data.Mdata.T) / 256;
        motor_r->W = ((float)motor_r->motor_recv_data.Mdata.W) / 128;
        motor_r->LW = motor_r->motor_recv_data.Mdata.LW;

        motor_r->Acc = (int)motor_r->motor_recv_data.Mdata.Acc;
        motor_r->Pos = 6.2832*((float)motor_r->motor_recv_data.Mdata.Pos) / 16384;
        
        motor_r->gyro[0] = ((float)motor_r->motor_recv_data.Mdata.gyro[0]) * 0.00107993176;
        motor_r->gyro[1] = ((float)motor_r->motor_recv_data.Mdata.gyro[1]) * 0.00107993176;
        motor_r->gyro[2] = ((float)motor_r->motor_recv_data.Mdata.gyro[2]) * 0.00107993176;
        
        motor_r->acc[0] = ((float)motor_r->motor_recv_data.Mdata.acc[0]) * 0.0023911132;
        motor_r->acc[1] = ((float)motor_r->motor_recv_data.Mdata.acc[1]) * 0.0023911132;
        motor_r->acc[2] = ((float)motor_r->motor_recv_data.Mdata.acc[2]) * 0.0023911132;

        motor_r->correct = true;
        return motor_r->correct;
    }
}

#endif  // UNITREEMOTOR_H