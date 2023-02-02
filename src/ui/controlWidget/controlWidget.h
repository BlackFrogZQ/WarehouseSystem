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

protected:
    QPushButton *m_reset;
    QPushButton *m_startRun;
    QPushButton *m_crashStop;
    QPushButton *m_systemPara;

    QLineEdit *m_ycgType;
    QLineEdit *m_lzType;
    QPushButton *m_assemblyType;

    QLabel *m_allCount;
    QLabel *m_okCount;
    QLabel *m_ngCount;
};
