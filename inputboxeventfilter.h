#ifndef INPUTBOXEVENTFILTER_H
#define INPUTBOXEVENTFILTER_H
#include <QMainWindow>

class InputBoxEventFilter
{
public:
    InputBoxEventFilter();
    bool eventFilter(QObject *, QEvent *);



};

#endif // INPUTBOXEVENTFILTER_H
