#pragma once
#include "basic.h"
#include "mainWindowDef.h"
#include "system/basic.h"
#include "controlWidget/controlWidget.h"
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
    void drawRectangle2Region();
    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void slotUpdateYcgImage(const QImage &p_image);
    void slotUpdateLzImage(const QImage &p_image);
    void slotVideoImage(double p_YcgExposureTime, double p_LzExposureTime);
    void slotGrabImage();
    bool slotImageDiscern(const bool &p_useWindow, const bool &p_processYcg, bool &p_direction);

protected:
    QLabel *m_pImageLabel;
    QLabel *m_ycgImageLabel;
    QLabel *m_lzImageLabel;
    QTextBrowser *m_pOutMsg;
    HalconCpp::HTuple m_drawWindowHandle;
    ControlWidget *m_controlWidget;
    bool m_showYcg;
    QImage m_ycgImage;
    QImage m_lzImage;
};

CMainWindow *mainWindow();