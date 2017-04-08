---
layout : life
title : 日常linux命令之iftop
category : linux学习
duoshuo : true
date : 2016-05-23
---


******

	作者: daodaoliang
    时间: 2016年5月23日
    版本: v0.0.1
    邮箱: daodaoliang@yeah.net

<!-- more -->

日常用的网络流量查看工具为 `iftop`， 但是他仅仅只能简单的查看网络的流量情况，若是想要查看某个链接的流量情况就比较困难了，因此再次推荐`iftop` 这个工具

### 0x01 安装 iftop

[我是官方网站](http://www.ex-parrot.com/pdw/iftop/)

**编译安装iftop示例:**

```sh
    # centos
    yum install flex byacc  libpcap ncurses ncurses-devel libpcap-devel

    # debian
    apt-get install flex byacc  libpcap0.8 libncurses5
```

```sh
    wget http://www.ex-parrot.com/pdw/iftop/download/iftop-0.17.tar.gz

    tar zxvf iftop-0.17.tar.gz

    cd iftop-0.17

    ./configure

    make && make install
```

**一键安装iftop 示例:**

```sh
    # 安装EPEL源

    # CentOS/RHEL 5 ：
    rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-5.noarch.rpm

    # CentOS/RHEL 6 ：
    rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-6.noarch.rpm

    # CentOS/RHEL 7 ：
    rpm -Uvh https://dl.fedoraproject.org/pub/epel/epel-release-latest-7.noarch.rpm
```

```sh
    # centos
    yum install iftop

    # debian
    apt-get install iftop
```

### 0x02  iftop 界面说明

直接执行

```sh
    iftop -i eth1
```

![iftop](/res/img/blog/linux学习/iftop.png)

**界面说明:**

```sh
    # 顶部: 带宽显示， 可以通过按键 `L` 进行切换流量的刻度;

    # 中间部分: 外部链接列表，列出所有和本机有网络交互的链接信息, 其中右边部分的数字分别为 2s、10s、40s平均流量，=>代表发送数据，<= 代表接收数据;

    # 底部: TX-发送流量 、RX-接收流量、TOTAL-总流量、Cumm-运行iftop到目前时间的总流量、peak-流量峰值、rates-分别表示过去 2s 10s 40s 的平均流量
```

### 0x03 iftop 常用参数

**-i:**  设定监测的网卡，如：# iftop -i eth1

**-B:**  以bytes为单位显示流量(默认是bits)，如：# iftop -B

**-n:** 使host信息默认直接都显示IP，如：# iftop -n

**-N:** 使端口信息默认直接都显示端口号，如: # iftop -N

**-F:** 显示特定网段的进出流量，如# iftop -F 10.10.1.0/24或# iftop -F 10.10.1.0/255.255.255.0

**-h:** （display this message），帮助，显示参数信息

**-p:** 使用这个参数后，中间的列表显示的本地主机信息，出现了本机以外的IP信息;

**-b:** 使流量图形条默认就显示;

**-f:** 这个暂时还不太会用，过滤计算包用的;

**-P:** 使host信息及端口信息默认就都显示;

**-m:** 设置界面最上边的刻度的最大值，刻度分五个大段显示，例：# iftop -m 100M;

### 0x04 iftop画面上一些操作命令

```text
    按h切换是否显示帮助;

    按n切换显示本机的IP或主机名;

    按s切换是否显示本机的host信息;

    按d切换是否显示远端目标主机的host信息;

    按t切换显示格式为2行/1行/只显示发送流量/只显示接收流量;

    按N切换显示端口号或端口服务名称;

    按S切换是否显示本机的端口信息;

    按D切换是否显示远端目标主机的端口信息;

    按p切换是否显示端口信息;

    按P切换暂停/继续显示;

    按b切换是否显示平均流量图形条;

    按B切换计算2秒或10秒或40秒内的平均流量;

    按T切换是否显示每个连接的总流量;

    按l打开屏幕过滤功能，输入要过滤的字符，比如ip,按回车后，屏幕就只显示这个IP相关的流量信息;

    按L切换显示画面上边的刻度;刻度不同，流量图形条会有变化;

    按j或按k可以向上或向下滚动屏幕显示的连接记录;

    按1或2或3可以根据右侧显示的三列流量数据进行排序;

    按<根据左边的本机名或IP排序;

    按>根据远端目标主机的主机名或IP排序;

    按o切换是否固定只显示当前的连接;

    按f可以编辑过滤代码，这是翻译过来的说法，我还没用过这个！

    按!可以使用shell命令，这个没用过！没搞明白啥命令在这好用呢！

    按q退出监控。
```




