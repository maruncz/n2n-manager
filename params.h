#ifndef PARAMS_H
#define PARAMS_H

#define DEFAULT_PARAMS "community:\n\
key:\n\
keyfile:\n\
edge_ip:0.0.0.0\n\
edge_mask:255.255.255.0\n\
dhcp:no\n\
supernode:0.0.0.0:7000\n\
dns:no\n\
snm:7001\n\
port:7002\n\
mgmt:7003\n\
mac:00:00:00:00:00:00\n\
mtu:1400\n\
forward:no\n\
multicast:no\n\
"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QHostAddress>
#include <cmath>

class params : public QObject
{
    Q_OBJECT

private:

    QFile *file;

    QTextStream stream;


public:
    explicit params(QObject *parent = 0);

    QString community,key,keyfile,edge_ip,edge_mask,supernode_ip,supernode_port,snm_port,edge_port,mgmt_port,mac,mtu;
    Qt::CheckState dhcp,dns,forward,multicast;


    bool valid_ip(QString ip);

    bool valid_mask(QString mask);

    bool valid_port(QString port);

    bool valid_mac(QString mac);

    QString generate_mac();

    int save_params();

signals:

public slots:
};

#endif // PARAMS_H
