#include "synccomservise.h"
#include "exceptionimpl.h"

void SyncCOMServise::setDefaultCOMPParams()
{
    COMPParams = {0};
    timeouts = {0};

    if (!GetCommState(COMPHandel, &oldCOMPParams)) {
        recoverySettingsRequired = FALSE;
    }

    COMPParams.DCBlength=sizeof(COMPParams);
    COMPParams.BaudRate=CBR_115200;         //BOD
    COMPParams.ByteSize=8;                  //data transmission size in bits
    COMPParams.StopBits=ONESTOPBIT;         //using only one stop bit
    COMPParams.Parity=NOPARITY;             //using no prity bit

    timeouts.ReadIntervalTimeout = TIMEOUT;
    timeouts.ReadTotalTimeoutConstant = TIMEOUT;
    timeouts.ReadTotalTimeoutMultiplier = TIMEOUT;
    timeouts.WriteTotalTimeoutConstant = TIMEOUT;
    timeouts.WriteTotalTimeoutMultiplier = TIMEOUT;

    if(!SetCommState(COMPHandel, &COMPParams)){
        throw exceptionImpl("Setting COM port parameters failed", TRUE);
    }
    if(!SetCommTimeouts(COMPHandel, &timeouts)){
        throw exceptionImpl("Setting COM port timeouts failed", TRUE);
    }
}

void SyncCOMServise::setOldCOMPParams()
{
    if(!SetCommState(COMPHandel, &oldCOMPParams)){
        throw exceptionImpl("Recovery COM port parameters failed", TRUE);
    }
}

SyncCOMServise::SyncCOMServise()
{
    COMPHandel = CreateFileA(COMM1_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(COMPHandel == INVALID_HANDLE_VALUE){
        //COMPHandel = CreateFile(COMM2_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
        COMPHandel = CreateFileA(COMM2_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if(COMPHandel == INVALID_HANDLE_VALUE){
            throw exceptionImpl("COM1 & COM2 ports are either not available or dose not exist.", TRUE);
        }
    }
    recoverySettingsRequired = TRUE;
    setDefaultCOMPParams();


}

SyncCOMServise::~SyncCOMServise()
{
    if(recoverySettingsRequired){
        setOldCOMPParams();
    }
    CloseHandle(COMPHandel);
}

USHORT SyncCOMServise::sendToCOMP(CHAR *charArray, USHORT charArraySize)
{
    USHORT resultCharArraySize;
    if(charArray == NULL)
        throw exceptionImpl("Writing buffer was NULL.", TRUE);
    WriteFile(COMPHandel, charArray, charArraySize, (LPDWORD)&resultCharArraySize, NULL);
    if(GetLastError() != ERROR_IO_PENDING){
        throw exceptionImpl("Writing data faild.", FALSE);
    }
    return resultCharArraySize;
}

USHORT SyncCOMServise::readCOMP(CHAR **charArray, USHORT expectedCharArraySize)
{
    USHORT charArraySize;
    if(charArray == NULL)
        throw exceptionImpl("Reading buffer pointer was NULL.", TRUE);
     if(!ReadFile(COMPHandel,
                    *charArray,
                    expectedCharArraySize,
                    (LPDWORD)&charArraySize,
                    NULL)){ //    (LPDWORD)&res
        if(GetLastError()!= ERROR_IO_PENDING){
            throw exceptionImpl("Reading data faild.", FALSE);
        }

    }
    return charArraySize;
}

void SyncCOMServise::changeTransmissionSpeed(USHORT baudRateNum)
{
    if(baudRateNum < sizeof(BAUD_RATE)/sizeof(ULONG)){
        COMPParams.BaudRate = BAUD_RATE[baudRateNum];
        if(!SetCommState(COMPHandel, &COMPParams)){
            throw exceptionImpl("Setting COM port parameters failed", TRUE);
        }
        return;
    }
    throw exceptionImpl("Setting new baud rate parametr faild", FALSE);
}
