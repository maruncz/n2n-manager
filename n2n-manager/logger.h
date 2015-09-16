#ifndef LOGGER_H
#define LOGGER_H

#include <QWidget>
#include <QString>

namespace Ui {
class logger;
}

class logger : public QWidget
{
    Q_OBJECT

public:
    explicit logger(QWidget *parent = 0);
    ~logger();

    void on_error_log(QString str);
    void on_std_log(QString str);

private slots:
    void on_close_clicked();

private:
    Ui::logger *ui;
};

#endif // LOGGER_H
