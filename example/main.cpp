#include "serialPort/SerialPort.h"
#include <csignal>

int main(){
    // set the serial port name
    SerialPort serial("/dev/ttyUSB0");

    // send message struct
    MOTOR_send motor_run, motor_stop;
    // receive message struct
    MOTOR_recv motor_r;

    // set the id of motor
    motor_run.id = 0;
    // set the motor type, A1 or B1
    motor_run.motorType = MotorType::A1;
    motor_run.mode = 5;
    motor_run.T = 0.0;
    motor_run.W = 0.0;
    motor_run.Pos = 0.0;
    motor_run.K_P = 0.0;
    motor_run.K_W = 0.0;

    motor_stop.id = motor_run.id;
    motor_stop.motorType = motor_run.motorType;
    motor_stop.mode = 0;

    motor_r.motorType = motor_run.motorType;

    // encode data into motor commands
    modify_data(&motor_run);
    modify_data(&motor_stop);

    // turn for 3 second
    for(int i(0); i<3; ++i){
        serial.sendRecv(&motor_run, &motor_r);
        // decode data from motor states
        extract_data(&motor_r);
        std::cout << "Pos: " << motor_r.Pos << std::endl;
        usleep(1000000);
    }

    // stop the motor
    while(!serial.sendRecv(&motor_stop, &motor_r)){
        usleep(100000);
    }

    return 0;
}
