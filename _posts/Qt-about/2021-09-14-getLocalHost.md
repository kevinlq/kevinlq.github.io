---
layout: post
title: Qt 获取本机IP地址信息
categories: [Qt学习]
description: 获取电脑局本机 IP 地址信息
keywords: Qt, Windows,IP地址
---

> 阅读本文大概需要 3 分钟



获取本机网卡过程中，可以会遇到电脑包含多种网卡场景

- 有线网卡
- 无线网卡
- 虚拟网卡，安装虚拟机后会有
- 安装 VNP 类软件，也会有对应的 IP 地址


具体实际使用时根据业务需要进行过滤即可

### 测试代码

```
    QString ipAddr;
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        qDebug() << address.protocol() << address.toString();
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            ipAddr = address.toString();
            break;
        }
    }

    qDebug() << "ipAddr:" << ipAddr << endl;

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QList<QNetworkAddressEntry> entry;
    foreach(QNetworkInterface inter, interfaces)
    {
        qDebug() << "humanReadableName:" << inter.humanReadableName()
                 << "name:" << inter.name() << " flags:" << inter.flags()
                 << "type:" << inter.type() << " hardwareAddress:" << inter.hardwareAddress() << endl;

        entry = inter.addressEntries();

        qDebug() << "#addressEntries:" << entry.size();
        int cnt = entry.size();
        for (int i = 0; i < cnt; ++i)
        {
            qDebug() << i << entry.at(i).ip().protocol() << entry.at(i).ip().toString();
        }
    }

    qDebug() << "getIP Address ==========" << endl;
    foreach(QNetworkInterface inter, interfaces)
    {
        bool bValid = inter.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning);

        entry = inter.addressEntries();

        if (bValid)
        {
            int cnt = entry.size();
            for (int i = 0; i < cnt; ++i)
            {
                if (entry.at(i).ip().protocol() == QAbstractSocket::IPv4Protocol)
                {
                    if (-1 != inter.name().indexOf("wireless"))
                    {
                        qDebug() << i << "wireless ip:" <<entry.at(i).ip().toString()
                                 <<" broadcast:" << entry.at(i).broadcast().toString()
                                 << " netmask:" << entry.at(i).netmask().toString();;
                    }
                    else if (-1 != inter.name().indexOf("ethernet"))
                    {
                        qDebug() << i << "ethernet ip:" <<entry.at(i).ip().toString()
                                 <<" broadcast:" << entry.at(i).broadcast().toString()
                                 << " netmask:" << entry.at(i).netmask().toString();
                    }
                }
            }
        }
    }
```

输出结果

```
QAbstractSocket::IPv6Protocol "fe80::bcad:65e8:d6c6:2123%ethernet_32777"
QAbstractSocket::IPv4Protocol "2.0.0.1"
ipAddr: "2.0.0.1"

humanReadableName: "以太网 4" name: "ethernet_32777"  flags: QFlags<QNetworkInterface::InterfaceFlag>(IsUp|IsRunning|CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "00:FF:35:35:41:5B"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::bcad:65e8:d6c6:2123%ethernet_32777"
1 QAbstractSocket::IPv4Protocol "2.0.0.1"
humanReadableName: "以太网 5" name: "ethernet_32771"  flags: QFlags<QNetworkInterface::InterfaceFlag>(CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "08:00:58:00:00:05"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::5d68:608:4d99:501e%ethernet_32771"
1 QAbstractSocket::IPv4Protocol "169.254.80.30"
humanReadableName: "以太网" name: "ethernet_32768"  flags: QFlags<QNetworkInterface::InterfaceFlag>(IsUp|IsRunning|CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "F4:8E:38:A9:EC:D8"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::49c4:80c5:e8f:c341%ethernet_32768"
1 QAbstractSocket::IPv4Protocol "192.168.1.29"
humanReadableName: "VMware Network Adapter VMnet1" name: "ethernet_32775"  flags: QFlags<QNetworkInterface::InterfaceFlag>(IsUp|IsRunning|CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "00:50:56:C0:00:01"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::f515:9d16:7b33:4ee3%ethernet_32775"
1 QAbstractSocket::IPv4Protocol "192.168.229.1"
humanReadableName: "VMware Network Adapter VMnet8" name: "ethernet_32776"  flags: QFlags<QNetworkInterface::InterfaceFlag>(IsUp|IsRunning|CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "00:50:56:C0:00:08"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::d545:673d:482f:ea1d%ethernet_32776"
1 QAbstractSocket::IPv4Protocol "192.168.230.1"
humanReadableName: "以太网 2" name: "ethernet_32770"  flags: QFlags<QNetworkInterface::InterfaceFlag>(CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "00:FF:24:2B:DD:2B"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::f9f2:5176:b928:4b8f%ethernet_32770"
1 QAbstractSocket::IPv4Protocol "169.254.75.143"
humanReadableName: "本地连接* 2" name: "ethernet_32772"  flags: QFlags<QNetworkInterface::InterfaceFlag>(CanBroadcast|CanMulticast) type: QNetworkInterface::Ethernet  hardwareAddress: "00:FF:F5:C6:FA:42"

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "fe80::95b9:22c6:3c5f:8f3%ethernet_32772"
1 QAbstractSocket::IPv4Protocol "169.254.8.243"
humanReadableName: "Loopback Pseudo-Interface 1" name: "loopback_0"  flags: QFlags<QNetworkInterface::InterfaceFlag>(IsUp|IsRunning|CanBroadcast|IsLoopBack|CanMulticast) type: QNetworkInterface::Loopback  hardwareAddress: ""

#addressEntries: 2
0 QAbstractSocket::IPv6Protocol "::1"
1 QAbstractSocket::IPv4Protocol "127.0.0.1"
getIP Address ==========

1 ethernet ip: "2.0.0.1"  broadcast: "2.0.0.255"  netmask: "255.255.255.0"
1 ethernet ip: "192.168.1.29"  broadcast: "192.168.1.255"  netmask: "255.255.255.0"
1 ethernet ip: "192.168.229.1"  broadcast: "192.168.229.255"  netmask: "255.255.255.0"
1 ethernet ip: "192.168.230.1"  broadcast: "192.168.230.255"  netmask: "255.255.255.0"
get_ip ==========

#humanReadableName  "以太网 4"
#humanReadableName  "以太网 5"
#humanReadableName  "以太网"
#humanReadableName  "VMware Network Adapter VMnet1"
virtual net continue: "VMware Network Adapter VMnet1"
#humanReadableName  "VMware Network Adapter VMnet8"
virtual net continue: "VMware Network Adapter VMnet8"
#humanReadableName  "以太网 2"
#humanReadableName  "本地连接* 2"
#humanReadableName  "Loopback Pseudo-Interface 1"
virtual net continue: "Loopback Pseudo-Interface 1"
"192.168.1.29"  :  1
"2.0.0.1"  :  1
```


## 详细代码

```
/*
nType 0:ipv4, 1:ipv6
*/
QString GetLocalHostAddress(int nType)
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    QList<QNetworkAddressEntry> entry;
    foreach(QNetworkInterface inter, interfaces)
    {
        // 过滤无效网卡信息
        if (inter.humanReadableName().contains(QLatin1String("VMware"))
                || inter.humanReadableName().contains(QLatin1String("Loopback"))
                || inter.humanReadableName().contains(QLatin1String("VirtualBox")))
        {
            continue;
        }

        if (inter.flags() & (QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
        {
            entry = inter.addressEntries();

            for (int i = 0; i < entry.size(); i++)
            {
                if (entry.at(i).ip().protocol() == nType)
                {
                    // 这里因为条件多，暂时只过滤 192开头的
                    // 无线可以这样过滤 -1 != inter.name().indexOf("wireless")
                    // -1 != inter.name().indexOf("ethernet")
                    QString strIP = entry.at(i).ip().toString();
                    if (strIP.startsWith(QLatin1String("192.")))
                    {
                        return strIP;
                    }
                }
            }
        }
    }

    return "0.0.0.0";
}
```


******

    作者:鹅卵石
    时间: 2021年9月14日
    版本:V 0.0.1
    邮箱:kevinlq@163.com
	版权:本博客若无特别声明，均属于作者原创文章，欢迎大家转载分享。但是，
	希望您注明来源，并留下原文地址，这是对作者最大的尊重，也是对知识的尊重。

<!-- more -->


---

**如果您对本文有任何问题，可以在下方留言，或者Email我.**

## 更多内容敬请关注公众号：devstone

<center>
<img src="/res/img/blog/qrcode_for_devstone.jpg" width="50%" height="50%" />
</center>

---
