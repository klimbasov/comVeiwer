#ifndef COMSERVICE_H
#define COMSERVICE_H
#include <windows.h>

class COMService
{
public:

    virtual USHORT sendToCOMP(CHAR*, USHORT);

    virtual USHORT readCOMP(CHAR**, USHORT);

    virtual void changeTransmissionSpeed(USHORT);
};

#endif // COMSERVICE_H
