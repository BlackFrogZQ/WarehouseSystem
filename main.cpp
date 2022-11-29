#include "src/ui/mainWindow.h"
#include <QApplication>
#include <QMessageBox>

#pragma comment(lib, "user32.lib")
void initSystemService();
void closeSystemService();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":res/assemble.png"));
    if(QMessageBox::information(NULL, cnStr("请检查"), cnStr("气泵是否打开？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        initSystemService();
        CMainWindow w;
        // w.show();
        w.showFullScreen();
        int code = a.exec();
        closeSystemService();
        return code;
    }
}