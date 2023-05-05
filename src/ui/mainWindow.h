#pragma once
#include "basic.h"
#include "mainWindowDef.h"
#include "system/basic.h"
#include "stateLedWidget/ycgStateLed.h"
#include "stateLedWidget/assembleStateLed.h"
using namespace std;

class QLabel;
class QTextBrowser;

class CMainWindow : public QDialog
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();
    void initLayout();
    void printMsg(QString p_msg);

protected:
    QTextBrowser *m_pOutMsg;
    CYcgStateLed *m_ycgStateLed;
    CAssembleStateLed *m_assembleStateLed;
};

CMainWindow *mainWindow();