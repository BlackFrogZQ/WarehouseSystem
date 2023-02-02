#include "src/ui/mainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#pragma comment(lib, "user32.lib")
void initSystemService();
void closeSystemService();

#define AUTO_RUN_KEY	"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
void setMyAppAutoRun(bool isStart)
{
    QString application_name = QApplication::applicationName();
    QSettings *settings = new QSettings(AUTO_RUN_KEY, QSettings::NativeFormat);
    if(isStart)
    {
        QString application_path = QApplication::applicationFilePath();
        settings->setValue(application_name, application_path.replace("/", "\\"));
    }
    else
    {
        settings->remove(application_name);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setMyAppAutoRun(true);
    a.setWindowIcon(QIcon(":res/storage.png"));
    initSystemService();
    CMainWindow w;
    w.setWindowTitle("StorageSystem");
    // w.show();
    w.showFullScreen();
    int code = a.exec();
    closeSystemService();
    return code;
}

