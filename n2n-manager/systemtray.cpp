#include "systemtray.h"

systemtray::systemtray(QObject *parent) : QObject(parent)
{
    icon.addFile(":/pic/icon.png");

    trayicon = new QSystemTrayIcon(icon,new QObject(0));

    open = new QAction("open",this);
    close = new QAction("close",this);

    connect(trayicon,&QSystemTrayIcon::activated,this,&systemtray::tray_activated);
    connect(open,&QAction::triggered,this,&systemtray::on_open);
    connect(close,&QAction::triggered,this,&systemtray::on_close);

    menu = new QMenu();
    menu->addAction(open);
    menu->addAction(close);


    trayicon->setContextMenu(menu);
    trayicon->show();
    manager_window.show();
}

void systemtray::tray_activated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
        {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
        {
            manager_window.show();
        }
        break;
        case QSystemTrayIcon::Context:
        {

        }
        break;
    }
}

void systemtray::on_open()
{
    manager_window.show();
}

void systemtray::on_close()
{
    emit quit();
}

