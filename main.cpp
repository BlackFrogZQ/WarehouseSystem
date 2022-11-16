#include "src/ui/mainWindow.h"
#include <QApplication>

#pragma comment(lib, "user32.lib")
void initSystemService();
void closeSystemService();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initSystemService();
    CMainWindow w;
    w.show();
    int code = a.exec();
    closeSystemService();
    return code;
}