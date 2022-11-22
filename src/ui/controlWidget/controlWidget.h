#pragma once
#include "ui/mainWindowDef.h"
#include "ui/basic.h"
#include "system/basic.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class ControlWidget : public QLabel
{
    Q_OBJECT

public:
    ControlWidget();
    virtual ~ControlWidget();

protected:
    void initLayout();

protected slots:
    void vmStateUpdate();
    void setRunTtpe(QByteArray p_readType);
    void setTwistPara(double p_twistPara);

protected:
    QPushButton *m_reset;
    QPushButton *m_startRun;
    QPushButton *m_crashStop;

    QLineEdit *m_ycgType;
    QLineEdit *m_lzType;
    QLabel *m_typeMate;
};

struct CControlPara
{
    friend class ControlWidget;

public:
    quint16 runType() const;
    quint16 twist() const;

protected:
    quint16 m_runType = 1;
    quint16 m_twist = 20;
};

CControlPara *controlPara();