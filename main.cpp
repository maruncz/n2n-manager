#include <QApplication>
#include "systemtray.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    systemtray tray;
    a.connect(&tray,&systemtray::quit,&a,&QApplication::quit);
    a.connect(&tray.manager_window,&manager::quit,&a,&QApplication::quit);

    return a.exec();
}
