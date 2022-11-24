#include "serialPort/SerialPort.h"
#include <unistd.h>

int main() {

  SerialPort  serial("/dev/ttyUSB0");
  MotorCmd    cmd;
  MotorData   data;

  while(true) {
    cmd.motorType = MotorType::GO_M8010_6;
    cmd.id    = 0;
    cmd.mode  = 1;
    cmd.K_P   = 0.0;
    cmd.K_W   = 0.05;
    cmd.Pos   = 0.0;
    cmd.W     = 6.28*6.33;
    cmd.T     = 0.0;
    serial.sendRecv(&cmd,&data);

    std::cout <<  std::endl;
    std::cout <<  "motor.Pos: "    << data.Pos    <<  std::endl;
    std::cout <<  "motor.Temp: "   << data.Temp   <<  std::endl;
    std::cout <<  "motor.W: "      << data.W      <<  std::endl;
    std::cout <<  "motor.T: "      << data.T      <<  std::endl;
    std::cout <<  "motor.MError: " << data.MError <<  std::endl;
    std::cout <<  std::endl;

    usleep(200);

  }

}
