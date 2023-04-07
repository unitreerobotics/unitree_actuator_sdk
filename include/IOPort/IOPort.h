#ifndef __IOPORT_H
#define __IOPORT_H

#include <stdint.h>
#include <unistd.h>
#include <vector>
#include "unitreeMotor/unitreeMotor.h"

enum class BlockYN{
    YES,
    NO
};

class IOPort{
public:
    IOPort(BlockYN blockYN, size_t recvLength, size_t timeOutUs){
        resetIO(blockYN, recvLength, timeOutUs);
    }
    virtual ~IOPort(){}
    virtual size_t send(uint8_t *sendMsg, size_t sendLength) = 0;
    virtual size_t recv(uint8_t *recvMsg, size_t recvLength) = 0;
    virtual size_t recv(uint8_t *recvMsg) = 0;
    virtual bool sendRecv(std::vector<MotorCmd> &sendVec, std::vector<MotorData> &recvVec) = 0;
    void resetIO(BlockYN blockYN, size_t recvLength, size_t timeOutUs);
protected:
    BlockYN _blockYN = BlockYN::NO;
    size_t _recvLength;
    timeval _timeout;
    timeval _timeoutSaved;
};

inline void IOPort::resetIO(BlockYN blockYN, size_t recvLength, size_t timeOutUs){
    _blockYN = blockYN;
    _recvLength = recvLength;
    _timeout.tv_sec  = timeOutUs / 1000000;
    _timeout.tv_usec = timeOutUs % 1000000;
    _timeoutSaved = _timeout;
}


#endif  // z1_lib_IOPORT_H