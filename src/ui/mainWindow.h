#pragma once
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
    bool slotGrabYcgImage();
    bool slotGrabLzImage();
    bool slotYcgImageDiscern(bool &p_direction);
    bool slotLzImageDiscern(bool &p_direction);
    QString getSaveImagePath(bool p_saveYcg,bool p_isSuccessful) const;

protected:
    QLabel *m_ycgImageLabel;
    QLabel *m_lzImageLabel;
    QTextBrowser *m_pOutMsg;
    ControlWidget *m_controlWidget;
    CPlcStateLed *m_plcStateLed;

    QImage m_ycgImage;
    QImage m_lzImage;
    QPainterPath m_ycgRoiPath;
    QPainterPath m_lzRoiPath;
};

CMainWindow *mainWindow();