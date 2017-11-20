---
layout : post
title: Qt笔记之QSignalMapper的使用
category : Qt学习
wangyiyun: true
date : 2017-05-21
---

******

    作者:鹅卵石
    时间:2017年5月21日9:11:56
    版本:V 0.0.0
    邮箱:kevinlq@yeah.net

<!-- more -->

## Qt笔记之QSignalMapper的使用
QSignalMapper作为一个信号翻译器或者说是信号转换器，可以方便的将多个用一类型的信号转化成我们单一的数值型或者文本型的信号

### 使用
这里使用了垂直布局将创建的按钮进行了布局
```
    m_pTbnLayout = new QVBoxLayout(m_pWidgetBack);

    m_pSignalMap = new QSignalMapper(this);

    m_listTbnText <<"tbn1"<<"tbn2"<<"tbn3";

    foreach (QString text, m_listTbnText) {
        m_pTbn = new QToolButton(m_pWidgetBack);
        m_pTbn->setText (text);
        m_pTbn->setMinimumSize (100,30);
        m_pTbnLayout->addWidget (m_pTbn);
        m_pSignalMap->setMapping (m_pTbn,text);
        connect (m_pTbn,SIGNAL(clicked(bool)),m_pSignalMap,SLOT(map()));
    }
    connect (m_pSignalMap,SIGNAL(mapped(QString)),this,SIGNAL(signalOnTbnClicked(QString)));

    m_pTbnLayout->addStretch ();
    m_pTbnLayout->setContentsMargins (2,2,2,2);

```

### 运行效果

![](/res/img/blog/Qt学习/521.png)

---

如果您对本文有任何问题，可以在下方留言，或者Email我 

微信公众号：devstone，扫描二维码直接关注

![](/res/img/blog/qrcode_for_devstone.jpg)