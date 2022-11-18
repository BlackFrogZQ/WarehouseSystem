﻿#pragma once
#include "basic.h"
#include "mainWindowDef.h"
#include "system/basic.h"
#include "controlWidget/controlWidget.h"
#include "stateLedWidget/plcStateLed.h"
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

public slots:
    void slotVideoImage(double p_YcgExposureTime, double p_LzExposureTime);
    void slotUpdateYcgImage(const QImage &p_image);
    void slotUpdateLzImage(const QImage &p_image);
    void slotGrabYcgImage();
    void slotGrabLzImage();
    bool slotYcgImageDiscern(bool &p_direction);
    bool slotLzImageDiscern(bool &p_direction);

protected:
    QLabel *m_ycgImageLabel;
    QLabel *m_lzImageLabel;

    QTextBrowser *m_pOutMsg;
    ControlWidget *m_controlWidget;
    CPlcStateLed *m_plcStateLed;

    QImage m_ycgImage;
    QImage m_lzImage;
};

CMainWindow *mainWindow();