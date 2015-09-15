#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QFileDialog>
#include <cstdlib>
#include "params.h"


namespace Ui {
class manager;
}

class manager : public QWidget
{
    Q_OBJECT

public:
    explicit manager(QWidget *parent = 0);
    ~manager();

private slots:
    void on_apply_clicked();

    void on_quit_clicked();

    void on_keyfile_choose_clicked();

    void on_key_radio_clicked();

    void on_keyfile_radio_clicked();

    void on_dhcp_stateChanged(int arg1);

    void on_generate_mac_clicked();

signals:

    void quit();

private:
    Ui::manager *ui;

    QProcess *edge;

    params settings;
};

#endif // MANAGER_H
