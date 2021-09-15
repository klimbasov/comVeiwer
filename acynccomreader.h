#ifndef ACYNCCOMREADER_H
#define ACYNCCOMREADER_H
#include <QTimer>
#include <windows.h>
#include "asynccomservece.h"
#include "comservice.h"
#include "exceptionimpl.h"

class acyncCOMReader : public QObject
{
    Q_OBJECT
    constexpr static USHORT TIMER_INTERVAL = 100;
    constexpr static USHORT BUFFER_SIZE = 100;
private:
    QTimer timer;
    COMService& compService;
    char * buffer;
    bool outputGet;
    QString curRead;

    void clearBuffer();
public slots:
    void checkOutput();

    signals:
        void resultReady(QString);
        void exceptionThrown(exceptionImpl&);
public:
    acyncCOMReader(COMService&);
    ~acyncCOMReader();
};

#endif // ACYNCCOMREADER_H
