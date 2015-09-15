#include "params.h"

params::params(QObject *parent) : QObject(parent)
{
    file=new QFile("n2n.conf");
    stream.setDevice(file);
    if(!file->exists())
    {
        file->open(QIODevice::WriteOnly | QIODevice::Truncate);
        stream << DEFAULT_PARAMS ;
        file->close();
    }
    QStringList line;
    file->open(QIODevice::ReadOnly);
    line=stream.readLine().split(":",QString::KeepEmptyParts);
    while(!stream.atEnd())
    {
        if(line.at(0)=="community")
        {
            community=line.at(1);
        }
        if(line.at(0)=="key")
        {
            key=line.at(1);
        }
        if(line.at(0)=="keyfile")
        {
            keyfile=line.at(1);
        }
        if(line.at(0)=="edge_ip")
        {
            edge_ip=line.at(1);
        }
        if(line.at(0)=="edge_mask")
        {
            edge_mask=line.at(1);
        }
        if(line.at(0)=="dhcp")
        {
            (line.at(1).toLower()=="yes")?(dhcp=Qt::Checked):(dhcp=Qt::Unchecked);
        }
        if(line.at(0)=="supernode")
        {
            supernode_ip=line.at(1);
            supernode_port=line.at(2);
        }
        if(line.at(0)=="dns")
        {
            (line.at(1).toLower()=="yes")?(dns=Qt::Checked):(dns=Qt::Unchecked);
        }
        if(line.at(0)=="snm")
        {
            snm_port=line.at(1);
        }
        if(line.at(0)=="port")
        {
            edge_port=line.at(1);
        }
        if(line.at(0)=="mgmt")
        {
            mgmt_port=line.at(1);
        }
        if(line.at(0)=="mac")
        {
            mac.clear();
            for(int i=1;i<6;i++)
            {
                mac.append(line.at(i));
                if(i<5)
                {
                    mac.append(":");
                }
            }
        }
        if(line.at(0)=="mtu")
        {
            mtu=line.at(1);
        }
        if(line.at(0)=="forward")
        {
            (line.at(1).toLower()=="yes")?(forward=Qt::Checked):(forward=Qt::Unchecked);
        }
        if(line.at(0)=="multicast")
        {
            (line.at(1).toLower()=="yes")?(multicast=Qt::Checked):(multicast=Qt::Unchecked);
        }
        line=stream.readLine().split(":",QString::KeepEmptyParts);
    }
    if(!valid_ip(edge_ip))
    {
        edge_ip="0.0.0.0";
    }
    if(!valid_mask(edge_mask))
    {
        edge_mask="255.255.255.0";
    }
    if(!valid_ip(supernode_ip))
    {
        supernode_ip="0.0.0.0";
    }
    if(!valid_port(supernode_port))
    {
        supernode_port="7000";
    }
    if(!valid_port(snm_port))
    {
        snm_port="7001";
    }
    if(!valid_port(edge_port))
    {
        edge_port="7002";
    }
    if(!valid_port(mgmt_port))
    {
        mgmt_port="7003";
    }
    if(!valid_mac(mac))
    {
        mac=generate_mac();
    }
    file->close();
    save_params();
}

QString params::generate_mac()
{
    int i;
    QString mac;
    mac.clear();
    QString tmp;
    for(i=0;i<6;i++)
    {
        tmp=QString::number(rand()%256,16);
        if(tmp.size()<2)
        {
            mac.append("0");
        }
        mac.append(tmp);
        if(i<5)
        {
            mac.append(":");
        }
    }
    return mac;
}

int params::save_params()
{
    if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return -1;
    }
    QString tmp;
    stream << "community:"+community+"\n";
    stream << "key:"+key+"\n";
    stream << "keyfile:"+keyfile+"\n";
    stream << "edge_ip:"+edge_ip+"\n";
    stream << "edge_mask:"+edge_mask+"\n";
    if(dhcp==Qt::Checked)
    {
        tmp="yes";
    }
    else
    {
        tmp="no";
    }
    stream << "dhcp:"+tmp+"\n";
    stream << "supernode:"+supernode_ip+":"+supernode_port+"\n";
    if(dns==Qt::Checked)
    {
        tmp="yes";
    }
    else
    {
        tmp="no";
    }
    stream << "dns:"+tmp+"\n";
    stream << "snm:"+snm_port+"\n";
    stream << "port:"+edge_port+"\n";
    stream << "mgmt:"+mgmt_port+"\n";
    stream << "mac:"+mac+"\n";
    stream << "mtu:"+mtu+"\n";
    if(forward==Qt::Checked)
    {
        tmp="yes";
    }
    else
    {
        tmp="no";
    }
    stream << "forward:"+tmp+"\n";
    if(multicast==Qt::Checked)
    {
        tmp="yes";
    }
    else
    {
        tmp="no";
    }
    stream << "multicast:"+tmp+"\n";
    file->close();
    return 0;
}

bool params::valid_ip(QString ip)
{
    QHostAddress ipa;
    ipa=QHostAddress(ip);
    if(ipa.isNull())
    {
        return false;
    }
    return true;
}

bool params::valid_mask(QString mask)
{
    QHostAddress msk;
    QStringList tmp;
    int i;
    double d,e;
    msk=QHostAddress(mask);
    if(msk.isNull())
    {
        return false;
    }
    tmp=mask.split(".",QString::KeepEmptyParts);
    if(tmp.size()<4)
    {
        return false;
    }
    for(i=0;i<4;i++)
    {
        d=tmp.at(i).toDouble()+1;
        e=log(d)/log(2);
        if((int)e%1!=0)
        {
            return false;
        }
    }

    return true;
}

bool params::valid_port(QString port)
{
    bool ok;
    int i;
    i=port.toInt(&ok,10);
    if((i>65535) || (i<1))
    {
        return false;
    }
    return ok;
}

bool params::valid_mac(QString mac)
{
    bool ok;
    int i,j;
    QStringList tmp;
    tmp=mac.split(":",QString::KeepEmptyParts);
    if(tmp.size()<6)
    {
        return false;
    }
    for(i=0;i<tmp.size();i++)
    {
        j=tmp.at(i).toInt(&ok,16);
        if(!ok)
        {
            return false;
        }
        if((j>255)||(j<0))
        {
            return false;
        }
    }
    return true;
}
