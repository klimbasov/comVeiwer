#include <QApplication>
#include <QDebug>
#include "acynccomreader.h"
#include "comservice.h"

void acyncCOMReader::clearBuffer()
{
    for(int counter = 0; counter < BUFFER_SIZE; counter++)
    buffer[counter] = 0;
}

void acyncCOMReader::checkOutput()
{
    clearBuffer();
    try{
    compService.readCOMP(&buffer, BUFFER_SIZE);
    }catch(exceptionImpl& e){
        emit exceptionThrown(e);
        if(e.isCritical()){
            throw e;
        }
    }
    curRead = QString(buffer);
    if(curRead.size()>0)
        emit resultReady(curRead);

    timer.start();
}

acyncCOMReader::acyncCOMReader(COMService& compService): compService(compService)
{
    timer.setInterval(TIMER_INTERVAL);
    connect(&timer, &QTimer::timeout,
            this, &acyncCOMReader::checkOutput);
    buffer = new char[BUFFER_SIZE];
    try{
    compService.readCOMP(&buffer, BUFFER_SIZE);
    }catch(exceptionImpl& e){
        emit exceptionThrown(e);
        if(e.isCritical()){
            throw e;
        }
    }

    timer.start();
}

acyncCOMReader::~acyncCOMReader()
{
    delete [] buffer;
}
