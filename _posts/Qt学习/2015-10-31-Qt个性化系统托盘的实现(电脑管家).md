---
layout : post
title: Qt实现个性化系统托盘
category : Qt学习
wangyiyun: true
date : 2015-10-31
---

******

	作者:鹅卵石
    时间:2015年10月31日14:10:09
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt实现个性化系统托盘

----------
　　今天刚好有点时间，就想着模仿下电脑管家的系统化托盘，看着挺好看的。下边是电脑管家自带的系统托盘：

　　![电脑管家系统托盘](/res/img/blog/Qt学习/trayicon1.png)

　　可以看出主要是由三部分组成的，分为上部，中间和底部，上边是
2个label，加2个图标label；中间是6个按钮，文字显示在底部，只能是QToolButton了，QPushButton办不到；底部也是2个图标label加2个开关按钮(开关按钮需要自己实现状态切换)，最底部用3个QPushButton就可以了。

　　
好了，有了上面的分析就可以开始进行设计了。


1. **新建一个工程**，名字就叫ComputerManager吧，类名和其他不变，Widget吧。本次将系统托盘的实现独立出来，封装成一个类。
2. **新建一个C++类，**作为系统托盘的具体实现，叫ComputerManager,继承自QSystemTrayIcon，源码中有详细介绍。

    `private:`

	`void initTopAction();                   //初始化顶部菜单`

	`void initMidAction();                   //初始化中间菜单`

	`void initBottomAction();                //初始化底部菜单`

    ` void initAction();                      //初始化Action`

    `void addActions();                      //将Action添加到Qmenu上`

    `void initConnect();                     //初始化信号和槽的连接`

3. **初始化顶部菜单和Action**

```
void ComputerManager::initTopAction()
{
    //顶层菜单项初始化
    m_topWidget = new QWidget();
    m_topWidgetAction = new QWidgetAction(m_trayMenu);
    m_topLabelIcon = new QLabel();
    m_topLabelIcon->setPixmap(QPixmap(":/image/topicon.png"));
    m_topLabelIcon->setFixedSize(30,30);

    m_topLabel = new QLabel(tr("Comuter Manager now protected you!"));
    m_topLabel->setObjectName(QString("WhiteLabel"));
    m_topLabel->setFont(QFont("Times", 11));
    QHBoxLayout* m_topLayout = new QHBoxLayout();
    m_topLayout->addWidget(m_topLabelIcon);
    m_topLayout->addWidget(m_topLabel,1,Qt::AlignHCenter | Qt::AlignVCenter);
    m_topLayout->setContentsMargins(0,0,0,0);
    m_topWidget->setLayout(m_topLayout);
    m_topWidget->setMinimumHeight(70);
    m_topWidget->installEventFilter(this);
    m_topWidgetAction->setDefaultWidget(m_topWidget);


    m_topQQWidget = new QWidget();
    m_topQQWidgetAction = new QWidgetAction(m_trayMenu);
    m_QQInfoBtn = new QLabel(tr("qq infor safety\n")+
                             tr("reinforce more secure"));
    m_QQInfoBtn->setCursor(Qt::PointingHandCursor);
    m_labelIcon = new QLabel();
    m_labelIcon->setPixmap(QPixmap(":/image/icon.png"));
    m_labelIcon->setFixedSize(50,50);

    QHBoxLayout* m_topQQLayout = new QHBoxLayout();
    m_topQQLayout->addWidget(m_labelIcon);
    m_topQQLayout->addWidget(m_QQInfoBtn);
    m_topQQLayout->setContentsMargins(0,0,0,0);
    m_topQQWidget->setLayout(m_topQQLayout);
    m_topQQWidget->setMinimumHeight(70);
    m_topQQWidget->installEventFilter(this);
    m_topQQWidgetAction->setDefaultWidget(m_topQQWidget);


    m_topTbnWidget = new QWidget();
    m_topTbnWidgetAction = new QWidgetAction(m_trayMenu);

    m_tbnOpenSteward = new QToolButton();
    m_tbnOpenSteward->setIcon(QIcon(":/image/1.png"));
    m_tbnOpenSteward->setIconSize(QSize(50,50));
    m_tbnOpenSteward->setAutoRaise(true);
    m_tbnOpenSteward->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_tbnHome = new QToolButton();
    m_tbnHome->setIcon(QIcon(":/image/2.png"));
    m_tbnHome->setIconSize(QSize(50,50));
    m_tbnHome->setAutoRaise(true);
    m_tbnHome->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_tbnAntiVirus = new QToolButton();
    m_tbnAntiVirus->setIcon(QIcon(":/image/3.png"));
    m_tbnAntiVirus->setIconSize(QSize(50,50));
    m_tbnAntiVirus->setAutoRaise(true);
    m_tbnAntiVirus->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    m_tbnCleanRubbish = new QToolButton();
    m_tbnCleanRubbish->setIcon(QIcon(":/image/4.png"));
    m_tbnCleanRubbish->setIconSize(QSize(50,50));
    m_tbnCleanRubbish->setAutoRaise(true);
    m_tbnCleanRubbish->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_tbnComputerSpeed = new QToolButton();
    m_tbnComputerSpeed->setIcon(QIcon(":/image/5.png"));
    m_tbnComputerSpeed->setIconSize(QSize(50,50));
    m_tbnComputerSpeed->setAutoRaise(true);
    m_tbnComputerSpeed->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_tbnComputerClinic = new QToolButton();
    m_tbnComputerClinic->setIcon(QIcon(":/image/6.png"));
    m_tbnComputerClinic->setIconSize(QSize(50,50));
    m_tbnComputerClinic->setAutoRaise(true);
    m_tbnComputerClinic->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QHBoxLayout *layout1 = new QHBoxLayout();
    layout1->addWidget(m_tbnOpenSteward);
    layout1->addWidget(m_tbnHome);
    layout1->setContentsMargins(0,0,0,0);
    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(m_tbnAntiVirus);
    layout2->addWidget(m_tbnCleanRubbish);
    layout2->setContentsMargins(0,0,0,0);
    QHBoxLayout *layout3 = new QHBoxLayout();
    layout3->addWidget(m_tbnComputerSpeed);
    layout3->addWidget(m_tbnComputerClinic);
    layout3->setContentsMargins(0,0,0,0);
    QVBoxLayout *mainLout = new QVBoxLayout();
    mainLout->addLayout(layout1);
    mainLout->addLayout(layout2);
    mainLout->addLayout(layout3);
    mainLout->setContentsMargins(0,0,0,0);
    m_topTbnWidget->setLayout(mainLout);
    m_topTbnWidgetAction->setDefaultWidget(m_topTbnWidget);
}
```

4 **初始化中间菜单和Action**


```
void ComputerManager::initMidAction()
{
    m_midWidget = new QWidget();
    m_midWidgetAction = new QWidgetAction(m_trayMenu);

    m_speedLabel = new QLabel(tr("Auto backup"));
    m_SpeedBtn = new mySwitchButton();
    m_SpeedBtn->setFixedSize(60,25);
    m_SpeedBtn->setObjectName(QString("TrayButtonAuto"));
    m_SpeedBtn->SetCheck(true);

    QHBoxLayout *m_midUpLayout = new QHBoxLayout();
    m_midUpLayout->addWidget(m_speedLabel);
    m_midUpLayout->addWidget(m_SpeedBtn);


    m_troubleLabel = new QLabel(tr("Auto Start"));
    m_troubleLabel->setToolTip(tr("computer now "));
    m_TroubleBtn = new mySwitchButton();
    m_TroubleBtn->setFixedSize(60,25);
    m_TroubleBtn->setObjectName(QString("TrayButtonAuto"));
    m_TroubleBtn->SetCheck(true);

    QHBoxLayout *m_midBottomLayout = new QHBoxLayout();
    m_midBottomLayout->addWidget(m_troubleLabel);
    m_midBottomLayout->addWidget(m_TroubleBtn);


    QVBoxLayout *m_midMainLayout = new QVBoxLayout();
    m_midMainLayout->addLayout(m_midUpLayout);
    m_midMainLayout->addLayout(m_midBottomLayout);

    m_midWidget->setLayout(m_midMainLayout);
    m_midWidgetAction->setDefaultWidget(m_midWidget);
}

```

5 **初始化底部菜单和Action**

```
void ComputerManager::initBottomAction()
{
    m_bottomWidget = new QWidget();
    m_bottomWidgetAction = new QWidgetAction(m_trayMenu);

    m_ReportBtn = new QPushButton(QIcon(":/image/mainMenu/update.png"), tr("Update"));
    m_ReportBtn->setObjectName(QString("TrayButton"));
    m_ReportBtn->setFixedSize(60, 25);

    m_InformBtn = new QPushButton(QIcon(":/image/mainMenu/about.png"), tr("About"));
    m_InformBtn->setObjectName(QString("TrayButton"));
    m_InformBtn->setFixedSize(60, 25);

    m_exitBtn = new QPushButton(QIcon(":/image/mainMenu/quit.png"), tr("Exit"));
    m_exitBtn->setObjectName(QString("TrayButton"));
    m_exitBtn->setFixedSize(60, 25);

    QHBoxLayout* m_bottomLayout = new QHBoxLayout();
    m_bottomLayout->addWidget(m_ReportBtn, 0, Qt::AlignCenter);
    m_bottomLayout->addWidget(m_InformBtn, 0, Qt::AlignCenter);
    m_bottomLayout->addWidget(m_exitBtn, 0, Qt::AlignCenter);

    m_bottomLayout->setSpacing(5);
    m_bottomLayout->setContentsMargins(5,5,5,5);

    m_bottomWidget->setLayout(m_bottomLayout);
    m_bottomWidgetAction->setDefaultWidget(m_bottomWidget);
}
```

6 **将所创建的Action添加到QMenu中**

```
void ComputerManager::initAction()
{
    this->setToolTip(tr("Computer Manager"));
    this->setIcon(QIcon(":/image/system_tray.png"));
    m_trayMenu = new QMenu();

    this->setContextMenu(m_trayMenu);
}
```

7 **外部调用**

```
    m_systemTray = new ComputerManager(this);

    m_systemTray->show();
   
```

###Qt利用语言家实现多语言翻译
软件开发中可以使用Qt强大的语言家进行翻译，保证不会出现大家所谓的乱码问题。


![Qt自带的翻译工具](/res/img/blog/Qt学习/trayicon2.png)




完成后的样子如下图所示：
![自己实现的效果](/res/img/blog/Qt学习/trayicon3.png)

**在来一张**

![这里写图片描述](/res/img/blog/Qt学习/trayicon4.png)

源码下载地址：

> http://download.csdn.net/detail/u013704336/9229231

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)