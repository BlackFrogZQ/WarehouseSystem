#pragma once
#include "ui/mainWindowDef.h"
#include "ui/basic.h"
#include "system/basic.h"
#include "system/tool/halconTool.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#define TypeLength 9

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

protected:
    QLineEdit *m_ycgType;
    QLineEdit *m_lzType;

    QPushButton *m_reset;
    QPushButton *m_startRun;
    QPushButton *m_crashStop;
};