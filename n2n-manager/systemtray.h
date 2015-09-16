#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include <QIcon>
#include <QSystemTrayIcon>
#include "manager.h"
#include <QMenu>
#include <QAction>

class systemtray : public QObject
{
    Q_OBJECT

private:

    QIcon icon;

    QMenu *menu;

    QAction *open;
    QAction *close;

public:
    explicit systemtray(QObject *parent = 0);

    QSystemTrayIcon *trayicon;

    manager manager_window;

signals:
    void quit();

public slots:

    void tray_activated(QSystemTrayIcon::ActivationReason reason);

    void on_open();

    void on_close();

};

#endif // SYSTEMTRAY_H
