#include "comcomunicationapplication.h"
#include "ui_comcomunicationapplication.h"

COMComunicationApplication::COMComunicationApplication(COMService& compServise, QWidget *parent): compServise(compServise), QMainWindow(parent)
    , ui(new Ui::COMComunicationApplication)
{
    ui->setupUi(this);
    ui->inputBlock->installEventFilter(this);
    this->setFixedSize(this->size());
    this->statusBar()->setSizeGripEnabled(false);
}

COMComunicationApplication::~COMComunicationApplication()
{
    delete ui;
}

bool COMComunicationApplication::eventFilter(QObject *obj, QEvent *event)
{
        if (obj == ui->inputBlock && event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(Qt::Key_Return == keyEvent->key() ||Qt::Key_Enter == keyEvent->key() )
            {
                QString inputString = ui->inputBlock->toPlainText();
                try{
                compServise.sendToCOMP(inputString.toLocal8Bit().data(), inputString.size());
                }catch(exceptionImpl e){
                    messageBox.setText(e.what());
                    messageBox.exec();
                    if(e.isCritical()){
                        throw e;
                    }
                }

                ui->inputBlock->clear();
                return true;
            }else{
                return false;
            }
        }else{
            return QMainWindow::eventFilter(obj, event);
        }
}

void COMComunicationApplication::textReceived(QString textToShow)
{
    ui->outputBlock->setText(textToShow);
}

void COMComunicationApplication::showException(exceptionImpl & e)
{
    messageBox.setText(e.what());
    messageBox.exec();
    if(e.isCritical()){
        throw e;
    }
}

void COMComunicationApplication::baudRateChanged(const int & baudRateNum)
{
    compServise.changeTransmissionSpeed(baudRateNum);
}
