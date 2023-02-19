#include "src/ui/mainWindow.h"
#include <QIcon>
#include <QtSingleApplication>
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
    instance.setWindowIcon(QIcon(":res/storage.png"));
    initSystemService();
    CMainWindow w;
    w.setWindowTitle("StorageSystem");
    // w.show();
    w.showFullScreen();
    w.setWindowState(Qt::WindowMaximized);
    instance.setActivationWindow(&w);
    int code = instance.exec();
    closeSystemService();
    return code;
}

