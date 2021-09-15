#ifndef COMCOMUNICATIONAPPLICATION_H
#define COMCOMUNICATIONAPPLICATION_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>
#include "asynccomservece.h"
#include "comservice.h"
#include "exceptionimpl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class COMComunicationApplication; }
QT_END_NAMESPACE

class COMComunicationApplication : public QMainWindow
{
    Q_OBJECT

public:
    COMComunicationApplication(COMService& compServise, QWidget *parent = nullptr);
    ~COMComunicationApplication();
protected:
    bool eventFilter(QObject *, QEvent *) override;

public slots:
    void textReceived(QString);
    void showException(exceptionImpl&);
    void baudRateChanged(const int&);

private:
    QMessageBox messageBox;
    Ui::COMComunicationApplication *ui;
    COMService& compServise;
};
#endif // COMCOMUNICATIONAPPLICATION_H
