#include <unistd.h>
#include "serialPort/SerialPort.h"
#include "unitreeMotor/unitreeMotor.h"
#include <math.h>
#define PI 3.1415926

int main() {

  SerialPort  serial("/dev/ttyUSB0");
  MotorCmd    cmd;
  MotorData   data;

  float output_kp = 25;
  float output_kd = 0.6;
  float rotor_kp = 0;
  float rotor_kd = 0;
  float gear_ratio = queryGearRatio(MotorType::A1);
  float sin_counter = 0.0;

  rotor_kp = (output_kp / (gear_ratio * gear_ratio)) / 26.07;
  rotor_kd = (output_kd / (gear_ratio * gear_ratio)) * 100.0;

  cmd.motorType = MotorType::A1;
  data.motorType = MotorType::A1;
  cmd.mode  = queryMotorMode(MotorType::A1,MotorMode::FOC);
  cmd.id    = 0;
  cmd.kp    = 0.0;
  cmd.kd    = 0.0;
  cmd.q     = 0.0;
  cmd.dq    = 0.0;
  cmd.tau   = 0.0;
  serial.sendRecv(&cmd,&data);
  float output_angle_c;
  output_angle_c = (data.q / gear_ratio) * (180/PI);
  while(true) 
  {
    sin_counter+=0.0001;
    float output_angle_d;
    output_angle_d = output_angle_c + 90 * sin(2*PI*sin_counter);
    float rotor_angle_d = (output_angle_d * (PI/180)) * gear_ratio;
    cmd.motorType = MotorType::A1;
    data.motorType = MotorType::A1;
    cmd.mode  = queryMotorMode(MotorType::A1,MotorMode::FOC);
    cmd.id    = 0;
    cmd.kp    = rotor_kp;
    cmd.kd    = rotor_kd;
    cmd.q     = rotor_angle_d;
    cmd.dq    = 0.0;
    cmd.tau   = 0.0;
    serial.sendRecv(&cmd,&data);


    std::cout <<  std::endl;
    std::cout <<  "motor.q: "    << data.q / gear_ratio    <<  std::endl;
    std::cout <<  "motor.temp: "   << data.temp   <<  std::endl;
    std::cout <<  "motor.W: "      << data.dq / gear_ratio     <<  std::endl;
    std::cout <<  "motor.merror: " << data.merror <<  std::endl;
    std::cout <<  "rotor_kp: " << rotor_kp <<  std::endl;
    std::cout <<  "rotor_kd: " << rotor_kd <<  std::endl;
    std::cout <<  std::endl;

    usleep(200);
  }

}