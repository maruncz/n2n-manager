#include "logger.h"
#include "ui_logger.h"

logger::logger(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logger)
{
    ui->setupUi(this);
    ui->log->clear();
}

logger::~logger()
{
    delete ui;
}

void logger::on_error_log(QString str)
{
    ui->log->append("<font color=red>"+str+"</font>");
}

void logger::on_std_log(QString str)
{
    ui->log->append("<font color=black>"+str+"</font>");
}

void logger::on_close_clicked()
{
    close();
}
