#ifndef I2CMODULE_H
#define I2CMODULE_H

#include <stdint.h>

class i2cModule
{
private:
    const char *devFileName = "/dev/i2c-1";
    uint8_t i2cAddr;
    int fd;

public:
    i2cModule();
    ~i2cModule();

    int sendDev(uint8_t *buffer, int length);

    int openDev(uint8_t i2cAddr_);
    int closeDev();
};

#endif // I2CMODULE_H
