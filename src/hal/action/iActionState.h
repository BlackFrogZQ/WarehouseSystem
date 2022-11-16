#pragma once
#include <QObject>
class IActionState:public QObject
{
    Q_OBJECT
public:
    virtual ~IActionState();
    virtual void run() = 0;
    virtual void runing();
protected:
    void changeState(IActionState* p_state);
};