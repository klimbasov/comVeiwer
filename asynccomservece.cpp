#include "asynccomservece.h"
#include "exceptionimpl.h"
#include <windows.h>
#include <QDebug>


AsyncCOMService::AsyncCOMService()
{
    //COMPHandel = CreateFile(COMM1_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    COMPHandel = CreateFileA(COMM1_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
    if(COMPHandel == INVALID_HANDLE_VALUE){
        //COMPHandel = CreateFile(COMM2_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
        COMPHandel = CreateFileA(COMM2_NAME, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
        if(COMPHandel == INVALID_HANDLE_VALUE){
            throw exceptionImpl("COM1 & COM2 ports are either not available or dose not exist.", TRUE);
        }
    }
    recoverySettingsRequired = TRUE;
    setDefaultCOMPParams();
    //setDefaultCOMPConfig();
    readEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    asyncReadInfo.hEvent = readEvent;
    writeEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
    asyncWriteInfo.hEvent = writeEvent;
}

AsyncCOMService::~AsyncCOMService()
{
    if(recoverySettingsRequired){
        setOldCOMPParams();
    }
    CloseHandle(COMPHandel);
}

USHORT AsyncCOMService::readCOMP(char ** charArray, USHORT expectedCharArraySize)
{
    ResetEvent(readEvent);
    if(charArray == NULL)
        throw exceptionImpl("Reading buffer pointer was NULL.", TRUE);
     if(!ReadFile(COMPHandel,
                    *charArray,
                    expectedCharArraySize,
                    NULL,
                    &asyncReadInfo)){ //    (LPDWORD)&res
        if(GetLastError()!= ERROR_IO_PENDING){
            throw exceptionImpl("Reading data faild.", FALSE);
        }

    }
    return 0;

}

USHORT AsyncCOMService::sendToCOMP(char * charArray, USHORT charArraySize)
{
    USHORT resultCharArraySize;
    if(charArray == NULL)
        throw exceptionImpl("Writing buffer was NULL.", TRUE);
    WriteFile(COMPHandel, charArray, charArraySize, (LPDWORD)&resultCharArraySize, &asyncWriteInfo);
    if(GetLastError() != ERROR_IO_PENDING){
        throw exceptionImpl("Writing data faild.", FALSE);
    }
    return resultCharArraySize;
}

void AsyncCOMService::setDefaultCOMPParams()
{
    this->COMPParams = {0};

    if (!GetCommState(COMPHandel, &oldCOMPParams)) {
        recoverySettingsRequired = FALSE;
    }

    COMPParams.DCBlength=sizeof(COMPParams);

    COMPParams.BaudRate=CBR_115200;         //BOD
    COMPParams.ByteSize=8;                  //data transmission size in bits
    COMPParams.StopBits=ONESTOPBIT;         //using only one stop bit
    COMPParams.Parity=NOPARITY;             //using no prity bit

    if(!SetCommState(COMPHandel, &COMPParams)){
        throw exceptionImpl("Setting COM port parameters failed", TRUE);
    }

}

void AsyncCOMService::setOldCOMPParams()
{
    if(!SetCommState(COMPHandel, &oldCOMPParams)){
        throw exceptionImpl("Recovery COM port parameters failed", TRUE);
    }
}

//void COMPServece::setDefaultCOMPConfig()
//{
//    if(!GetCommConfig(COMPHandel, &oldCOMPConfig, &oldCOMPConfigSize)){
//        recoveryConfigurationRequired  = FALSE;
//    }
//    this->COMPParams = {0};

//    COMPParams.DCBlength=sizeof(COMPParams);

//    COMPParams.BaudRate=CBR_115200;         //BOD
//    COMPParams.ByteSize=8;                  //data transmission size in bits
//    COMPParams.StopBits=ONESTOPBIT;         //using only one stop bit
//    COMPParams.Parity=NOPARITY;
//    oldCOMPConfig.dcb = COMPParams;
//    COMMPROP tempProp = *((COMMPROP*)oldCOMPConfig.dwProviderSubType);

//    if(!SetCommConfig(COMPHandel ,&oldCOMPConfig, sizeof(COMMCONFIG))){
//        throw exceptionImpl("Setting COM port configuration failed", TRUE);
//    }
//}

//void COMPServece::setOldCOMPConfig()
//{
//    if(recoveryConfigurationRequired){
//        if(!SetCommConfig(COMPHandel ,&oldCOMPConfig, oldCOMPConfig.dwSize)){
//            throw exceptionImpl("Setting COM port configuration failed", TRUE);
//        }
//    }
//}

void AsyncCOMService::changeTransmissionSpeed(USHORT baudRateNum)
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
