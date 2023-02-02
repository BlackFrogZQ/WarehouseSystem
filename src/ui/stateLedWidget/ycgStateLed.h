#pragma once
#include <QLabel>
#include <QVector>
#include "hal/communication/serialPort/typeDef.h"

class CYcgStateLed:public QLabel
{
    Q_OBJECT
public:
    CYcgStateLed(QWidget *parent=nullptr);
    ~CYcgStateLed();

public slots:
    void ycgReduceStorage(const int &p_readType);
    void ycgAddStorage(const int &p_readType);

protected:
    void initLayout();
    void setAllLed();
protected:
    QLabel* m_states[cytMax];
};
