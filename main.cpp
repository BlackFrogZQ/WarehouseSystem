#include "src/ui/mainWindow.h"
#include <QIcon>
#include <QtSingleApplication>
#include <QMessageBox>

#pragma comment(lib, "user32.lib")
void initSystemService();
void closeSystemService();

int main(int argc, char *argv[])
{
    QtSingleApplication instance(argc, argv);
    if (instance.sendMessage("Wake up!"))
    {
        return 0;
    }
    instance.setWindowIcon(QIcon(":res/assemble.png"));
    if(QMessageBox::information(NULL, cnStr("请检查"), cnStr("气泵是否打开？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        initSystemService();
        CMainWindow w;
        w.setWindowTitle("AutoAssemble");
        // w.show();
        w.showFullScreen();
        instance.setActivationWindow(&w);
        int code = instance.exec();
        closeSystemService();
        return code;
    }
}