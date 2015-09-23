#include "manager.h"
#include "ui_manager.h"

manager::manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);
    edge = new QProcess(this);
#ifdef __linux__
    edge->setProgram("pkexec");
    QProcess *id = new QProcess(this);
    id->setProgram("id");
    id->setArguments(QStringList("-u"));
    id->start(QIODevice::ReadOnly);
    id->waitForStarted();
    id->waitForReadyRead();
    uid=id->readLine();
    uid=uid.simplified();
    id->waitForFinished();
    id->setArguments(QStringList("-g"));
    id->start(QIODevice::ReadOnly);
    id->waitForStarted();
    id->waitForReadyRead();
    gid=id->readLine();
    gid=gid.simplified();
    id->waitForFinished();
#elif __WIN32__
    edge->setProgram("edge.exe");
#endif
    ui->edge_ip->setText(settings.edge_ip);
    ui->edge_mask->setText(settings.edge_mask);
    ui->community->setText(settings.community);
    ui->key->setText(settings.key);
    ui->dhcp->setCheckState(settings.dhcp);
    ui->supernode_ip->setText(settings.supernode_ip);
    ui->supernode_port->setText(settings.supernode_port);
    ui->dns->setCheckState(settings.dns);
    ui->snm_port->setText(settings.snm_port);
    ui->edge_port->setText(settings.edge_port);
    ui->mgmt_port->setText(settings.mgmt_port);
    ui->mac->setText(settings.mac);
    ui->mtu->setText(settings.mtu);
    ui->forward->setCheckState(settings.forward);
    ui->multicast->setCheckState(settings.multicast);
    ui->apply_on_start->setCheckState(settings.apply_on_start);

    connect(edge,&QProcess::readyReadStandardError,this,&manager::on_edge_log_error);
    connect(edge,&QProcess::readyReadStandardOutput,this,&manager::on_edge_log_std);
    if(settings.apply_on_start)
    {
        on_apply_clicked();
    }
}

manager::~manager()
{
    if(edge->state()==QProcess::Running)
    {
        edge->terminate();
        edge->waitForFinished();
        edge->kill();
    }
    delete ui;
}

void manager::on_apply_clicked()
{
    bool ok=true;
    if(!settings.valid_ip(ui->edge_ip->text()) && !ui->dhcp->checkState())
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid edge IP"));
        ok=false;
    }
    if(!settings.valid_mask(ui->edge_mask->text()) && !ui->dhcp->checkState())
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid edge mask"));
        ok=false;
    }
    if(!settings.valid_port(ui->supernode_port->text()))
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid supernode port"));
        ok=false;
    }
    if(!settings.valid_port(ui->snm_port->text()))
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid SNM port"));
        ok=false;
    }
    if(!settings.valid_port(ui->edge_port->text()))
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid edge port"));
        ok=false;
    }
    if(!settings.valid_port(ui->mgmt_port->text()))
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid management port"));
        ok=false;
    }
    if(!settings.valid_mac(ui->mac->text()))
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Not valid MAC"));
        ok=false;
    }
    if(ok)
    {
        settings.community=ui->community->text();
        settings.key=ui->key->text();
        settings.edge_ip=ui->edge_ip->text();
        settings.edge_mask=ui->edge_mask->text();
        settings.dhcp=ui->dhcp->checkState();
        settings.supernode_ip=ui->supernode_ip->text();
        settings.supernode_port=ui->supernode_port->text();
        settings.snm_port=ui->snm_port->text();
        settings.dns=ui->dns->checkState();
        settings.edge_port=ui->edge_port->text();
        settings.mgmt_port=ui->mgmt_port->text();
        settings.mac=ui->mac->text();
        settings.mtu=ui->mtu->text();
        settings.forward=ui->forward->checkState();
        settings.multicast=ui->multicast->checkState();
        settings.apply_on_start=ui->apply_on_start->checkState();
        QStringList args;
#ifdef __linux__
        args.append(QApplication::applicationDirPath()+"/edge");
        args.append("-u "+uid);
        args.append("-g "+gid);
#endif
#ifndef __WIN32__
        args.append("-f");
#endif
        args.append("-c "+settings.community);
        if(ui->key_radio->isChecked())
        {
            args.append("-k "+settings.key);
        }
        else if(ui->keyfile_radio->isChecked())
        {
            args.append("-K "+settings.keyfile);
        }
        if(settings.dhcp)
        {
            args.append("-a -r dhcp:0.0.0.0");
        }
        else
        {
            args.append("-a static:"+settings.edge_ip+" -s "+settings.edge_mask);
        }
        args.append("-l "+settings.supernode_ip+":"+settings.supernode_port);
        if(settings.dns)
        {
            args.append("-b");
        }
        args.append("-x "+settings.snm_port);
        args.append("-p "+settings.edge_port);
        args.append("-t "+settings.mgmt_port);
        args.append("-m "+settings.mac);
        args.append("-M "+settings.mtu);
        if((settings.forward) && (!settings.dhcp))
        {
            args.append("-r");
        }
        if(settings.multicast)
        {
            args.append("-E");
        }
        if(settings.save_params()!=0)
        {
            QMessageBox::warning(this,"cannot save settings","cannot save settings to file");
        }
        if(edge->state()==QProcess::Running)
        {
            edge->terminate();
            edge->waitForFinished();
            if(edge->state()==QProcess::Running)
            {
                QMessageBox::critical(this,"edge","cannot terminate edge");
            }
        }
        edge->setArguments(args);
        QMessageBox::information(this,"neco",edge->arguments().join(" "));
        edge->start(QIODevice::ReadOnly);
        if(!edge->waitForStarted())
        {
            QString tmp;
            tmp=edge->readAllStandardError();
            QMessageBox::critical(this,"cannot start edge",tmp);
            tmp=edge->readAllStandardOutput();
            QMessageBox::critical(this,"cannot start edge",tmp);
        }
    }


}

void manager::on_quit_clicked()
{
    if(edge->state()==QProcess::Running)
    {
        edge->terminate();
        if(edge->waitForFinished())
        {
            edge->kill();
        }
    }
    emit quit();
}

void manager::on_keyfile_choose_clicked()
{
    settings.keyfile = QFileDialog::getOpenFileName(this,tr("keyfile"));
}

void manager::on_key_radio_clicked()
{
    ui->keyfile_choose->setEnabled(false);
    ui->key->setEnabled(true);
}

void manager::on_keyfile_radio_clicked()
{
    ui->keyfile_choose->setEnabled(true);
    ui->key->setEnabled(false);
}

void manager::on_dhcp_stateChanged(int arg1)
{
    ui->edge_ip->setEnabled(!arg1);
    ui->edge_mask->setEnabled(!arg1);
}

void manager::on_generate_mac_clicked()
{
    ui->mac->setText(settings.generate_mac());
}

void manager::on_log_clicked()
{
    logwindow.show();
}

void manager::on_edge_log_error()
{
    logwindow.on_error_log(edge->readAllStandardError());
}

void manager::on_edge_log_std()
{
    logwindow.on_std_log(edge->readAllStandardOutput());
}
