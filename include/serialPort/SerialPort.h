#ifndef SERIALPORT_H
#define SERIALPORT_H

/* 
High frequency serial communication,
Not that common, but useful for motor communication.
*/
#include <termios.h>
// #include <sys/epoll.h>
#include <sys/select.h>

#include <string>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>    //serial驱动相关
#include <unistd.h>
#include <iostream>

#include "serialPort/include/errorClass.h"
#include "unitreeMotor/unitreeMotor.h"
#include "IOPort/IOPort.h"

enum class bytesize_t{
    fivebits,
    sixbits,
    sevenbits,
    eightbits
};

enum class parity_t{
    parity_none,
    parity_odd,
    parity_even,
    parity_mark,
    parity_space
};

enum class stopbits_t{
    stopbits_one,
    stopbits_two,
    stopbits_one_point_five
};

enum class flowcontrol_t{
    flowcontrol_none,
    flowcontrol_software,
    flowcontrol_hardware
};



class SerialPort : public IOPort{
public:
    SerialPort(const std::string &portName,
            size_t recvLength = 78,
            uint32_t baudrate = 4800000,
            size_t timeOutUs = 20000,
            BlockYN blockYN = BlockYN::NO,
            bytesize_t bytesize = bytesize_t::eightbits,
            parity_t parity = parity_t::parity_none,
            stopbits_t stopbits = stopbits_t::stopbits_one,
            flowcontrol_t flowcontrol = flowcontrol_t::flowcontrol_none);
    virtual ~SerialPort();
    void resetSerial(size_t recvLength = 78,
                     uint32_t baudrate = 4800000,
                     size_t timeOutUs = 20000,
                     BlockYN blockYN = BlockYN::NO,
                     bytesize_t bytesize = bytesize_t::eightbits,
                     parity_t parity = parity_t::parity_none,
                     stopbits_t stopbits = stopbits_t::stopbits_one,
                     flowcontrol_t flowcontrol = flowcontrol_t::flowcontrol_none);
    size_t send(uint8_t *sendMsg, size_t sendLength);
    size_t recv(uint8_t *recvMsg);
    bool sendRecv(uint8_t *sendMsg, uint8_t *recvMsg, size_t sendLength);
    bool sendRecv(MOTOR_send* sendMsg, MOTOR_recv* recvMsg);
    bool sendRecv(std::vector<MOTOR_send> &sendVec, std::vector<MOTOR_recv> &recvVec);

private:
    void _open();
    void _set();
    void _close();
    size_t _nonBlockRecv(uint8_t *recvMsg, size_t readLen); // only visual in pubSDK
    std::string _portName;
    uint32_t _baudrate;
    bytesize_t _bytesize;
    parity_t _parity;
    stopbits_t _stopbits;
    flowcontrol_t _flowcontrol;
    bool _xonxoff;
    bool _rtscts;

    int _fd;
    fd_set _rSet;

};


#endif  // SERIALPORT_H