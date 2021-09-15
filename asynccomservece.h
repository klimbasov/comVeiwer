#ifndef ASYNCCOMSERVECE_H
#define ASYNCCOMSERVECE_H
#include <windows.h>
#include <QThread>
#include "comservice.h"

class AsyncCOMService: public COMService
{
private:
    //static constexpr LPCTSTR COMM1_NAME = L"\\\\.\\COM1";
    static constexpr LPCSTR COMM1_NAME = "COM1";
    //static constexpr LPCTSTR COMM2_NAME = L"\\\\.\\COM2";
    static constexpr LPCSTR COMM2_NAME = "\\\\.\\COM2";
    static constexpr ULONG BAUD_RATE[] = {110, 300, 600, 1200,
                                      2400, 4800, 9600, 14400,
                                      19200, 38400, 57600, 115200,
                                      128000, 256000};
    HANDLE COMPHandel;
    DCB COMPParams,
        oldCOMPParams;
    COMMCONFIG COMPConfig,
            oldCOMPConfig;
    DWORD oldCOMPConfigSize;
    COMMPROP COMPProp;
    BOOL recoverySettingsRequired;
    BOOL recoveryConfigurationRequired;
    HANDLE readEvent,
            writeEvent;
    OVERLAPPED asyncReadInfo,
                asyncWriteInfo;

    void setDefaultCOMPParams();
    void setOldCOMPParams();

    void setDefaultCOMPConfig();
    void setOldCOMPConfig();

public:
    AsyncCOMService();
    ~AsyncCOMService();

    USHORT sendToCOMP(CHAR*, USHORT) override;

    USHORT readCOMP(CHAR**, USHORT) override;

    BOOL isOutputGetted();

    void changeTransmissionSpeed(USHORT) override;
};

#endif // ASYNCCOMSERVECE_H
