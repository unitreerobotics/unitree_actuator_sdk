#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "serialPort/SerialPort.h"

#define BroadAllMotorID 0xBB
#define MotorPulsator 11

int main()
{
    char serial_name[100];

    SerialPort *serial;
    MotorCmd motor_s;
    motor_s.motorType = MotorType::A1; // set the motor type, A1 or B1

    printf("Please input the name of serial port.(e.g. Linux:/dev/ttyUSB0, Windows:\\\\.\\COM3)\n");
    scanf("%s", serial_name);
    printf("The serial port is %s\n", serial_name);

    memset(static_cast<void *>(&motor_s), 0, sizeof(motor_s));
    motor_s.id = BroadAllMotorID;
    motor_s.mode = 10;
    motor_s.modify_data(&motor_s);

    printf("The motor ID is: %X\n", motor_s.motor_send_data.head.motorID);

    //进入伺服模式
    if (motor_s.motorType == MotorType::A1)
        serial = new SerialPort(serial_name, 78, 4800000); // set the serial port name
    else
        serial = new SerialPort(serial_name, 78, 6000000); // set the serial port name

    serial->send((uint8_t *)&(motor_s.motor_send_data), 34);

    usleep(100000); // sleep 0.1s

    //进入拨轮模式（修改ID）
    motor_s.mode = MotorPulsator;
    motor_s.modify_data(&motor_s);
    serial->send((uint8_t *)&(motor_s.motor_send_data), 34);

    printf("Please turn the motor.\n");
    printf("One time: id=0; Two times: id=1, Three times: id=2\n");
    printf("ID can only be 0, 1, 2\n");
    printf("Once finished, press 'a'\n");

    // int c;
    while (getchar() != (int)'a')
    {
    }
    printf("Turn finished\n");

    //保存ID
    motor_s.mode = 0;
    motor_s.modify_data(&motor_s);
    serial->send((uint8_t *)&(motor_s.motor_send_data), 34);

    return 0;
}