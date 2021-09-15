#include "comcomunicationapplication.h"
#include "asynccomservece.h"
#include "exceptionimpl.h"
#include "acynccomreader.h"
#include "synccomservise.h"
#include "comservice.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    COMService* compService = new AsyncCOMService();
    acyncCOMReader comReader(*compService);
    COMComunicationApplication w(*compService);
    QObject::connect(&comReader, &acyncCOMReader::resultReady,
                     &w, &COMComunicationApplication::textReceived);
    QObject::connect(&comReader, &acyncCOMReader::exceptionThrown,
                     &w, &COMComunicationApplication::showException);
    QObject::connect(&a, SIGNAL(QApplication::aboutToQuit()), &comReader,
                     SLOT(acyncCOMReader::finishExecution()));
    try{
    w.show();
    }catch(exceptionImpl e){
        a.quit();
        delete compService;
        return a.exec();
    }
    delete compService;
    return a.exec();
}
