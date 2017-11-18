---
layout : post
title: Qt笔记之多线程串口
category : Qt学习
wangyiyun: true
date : 2017-06-14
---

******

    作者:鹅卵石
    时间:2017-6-14 19:31:00
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt笔记之多线程串口

整理下串口通信过程中使用多线程的方法

### 原理

#### MoveToThread方式

1. 创建一个object,继承自QObject,将串口的所有初始化操作全部写在一个槽函数中，包括打开；
2. 在主线程中创建object对象，创建一个QThread对象，然后将object移动到该线程中；
3. 槽函数关联。线程的start信号关联的obj的初始化槽函数上；
4. 对串口的所有操作，俊通过信号和槽来进行；
5. **退出**，程序退出时不要直接delete obj,因为那个对象不在子线程

### 实现代码

**SerialPortObj.h**

```C++
#ifndef SERIALPORTOBJ_H
#define SERIALPORTOBJ_H

#include <QObject>

#include "serialport_src/qextserialport.h"

class SerialPortObj : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortObj(const QString &portName,
                           const int portBaud,
                           QObject *parent = 0);
    ~SerialPortObj();

private:
    void init();

    BaudRateType getBaundRate(int number);

Q_SIGNALS:
    /**
     * @brief  :   用来将读取串口的数据通过信号发射出去
     * @param  :   数据接入方式，这里为1(串口)
     * @param  :   buffData 串口接收到的数据
     * @return :
     */
    void signalSendSerialPortData(const QByteArray &buff);

private Q_SLOTS:

    /**
      *@brief 初始化串口信息，外部线程start信号触发
    */
    void slotStartInit();

    /**
     * @brief  :   读取串口数据
     * @return :   void
     */
    void slotReadSerialPortData();


    /**
     * @brief  :   用来向串口写入数据
     * @param  :   buffData 待写入的数据
     * @return :   void
     */
    void slotWriteSerialPort(const QByteArray &buffData);

private:
    QextSerialPort  *m_pSerialPort; //串口指针
    QString m_serialName;           //串口名称
    int m_serialBaud;               //波特率
};

#endif // SERIALPORTOBJ_H

```

**SerialPortObj.cpp**
```C++
#include "serialportobj.h"

#include <QDebug>
#include <QThread>

SerialPortObj::SerialPortObj(const QString &portName,
                             const int portBaud,
                             QObject *parent)
    :QObject(parent),m_serialName(portName),m_serialBaud(portBaud)
{
}

SerialPortObj::~SerialPortObj()
{
    qDebug ()<<"destroy SerialPortObj";
    if (m_pSerialPort){
        if (m_pSerialPort->isOpen ())
            m_pSerialPort->close ();
        delete m_pSerialPort;
        m_pSerialPort = NULL;
    }
}

void SerialPortObj::init()
{
#ifdef  Q_OS_WIN
    m_pSerialPort = new QextSerialPort(m_serialName);
#else
    m_pSerialPort = new QextSerialPort("/dev/"+m_serialName);
#endif
    connect (m_pSerialPort,SIGNAL(readyRead()),
             this,SLOT(slotReadSerialPortData()));

    //清空缓冲区
    m_pSerialPort->flush ();
    //设置波特率
    m_pSerialPort->setBaudRate (getBaundRate (m_serialBaud));
    //设置数据位
    m_pSerialPort->setDataBits (DATA_8);
    //设置校验位
    m_pSerialPort->setParity (PAR_NONE);
    //设置停止位
    m_pSerialPort->setStopBits (STOP_1);
    //设置无硬件流控制
    m_pSerialPort->setFlowControl (FLOW_OFF);
    m_pSerialPort->setTimeout (10);

    if (m_pSerialPort->open (QIODevice::ReadWrite)){
        qDebug ()<<"open serial port ok!";
    }else{
        qDebug ()<<"open serial port failed!"<<m_pSerialPort->errorString ();
    }
}

BaudRateType SerialPortObj::getBaundRate(int number)
{
    BaudRateType baud;
    switch (number) {
    case 9600:
        baud = BAUD9600;
        break;
    case 38400:
        baud = BAUD38400;
        break;
    case 115200:
        baud = BAUD115200;
        break;
    default:
        baud = BAUD38400;
        break;
    }
    return baud;
}

void SerialPortObj::slotStartInit()
{
    qDebug ()<<"SerialPortObj thread id:"<<QThread::currentThreadId ();

    init();
}

void SerialPortObj::slotReadSerialPortData()
{
    if (m_pSerialPort->bytesAvailable () <= 0)
        return ;

    QByteArray tempBuf = m_pSerialPort->readAll ();

    emit signalSendSerialPortData (tempBuf);
}

void SerialPortObj::slotWriteSerialPort(const QByteArray &buffData)
{
    if (!buffData.isEmpty ())
        m_pSerialPort->write (buffData);
}

```

**widget.cpp**

```C++
    m_pserialObj = new SerialPortObj("COM1",38400);
    m_pThread = new QThread;
    m_pserialObj->moveToThread (m_pThread);
    connect (m_pThread,SIGNAL(started()),
             m_pserialObj,SLOT(slotStartInit()));
    connect(qApp,SIGNAL(aboutToQuit()),
            m_pserialObj,SLOT(deleteLater()));
    connect (m_pserialObj,SIGNAL(destroyed(QObject*)),
             m_pThread,SLOT(quit()));
    connect (m_pThread,SIGNAL(finished()),
             m_pThread,SLOT(deleteLater()));

    m_pThread->start ();
```

以上就完成了一个简单的多线程串口，使用moveToThread方式比较简单，在不改变现有代码逻辑情况下，实现多线程和容易


---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)