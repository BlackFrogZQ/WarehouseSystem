#pragma once
#include <QLabel>
#include <QVector>
#include "hal/communication/serialPort/typeDef.h"

class CAssembleStateLed:public QLabel
{
    Q_OBJECT
public:
    CAssembleStateLed(QWidget *parent=nullptr);
    ~CAssembleStateLed();

public slots:
    void assembleReduceStorage(const int &p_readType);
    void assembleAddStorage(const int &p_readType);

protected:
    void initLayout();
    void setAllLed();
protected:
    QLabel* m_states[cytMax];
};
