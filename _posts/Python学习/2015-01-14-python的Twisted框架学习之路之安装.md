---
layout : life
title: python twisted模块之安装部署
category : Python学习
duoshuo: true
date : 2015-01-14
---

<!-- more -->


## Twisted 相关资源

* [官方下载网址](https://twistedmatrix.com/trac/wiki/Downloads)

## Twisted 安装

```sh
	sudo apt-get install python-twisted
```

## 服务端测试例子

```python
import sys
from twisted.internet.protocol import ServerFactory
from twisted.protocols.basic import LineReceiver
from twisted.python import log
from twisted.internet import reactor

class CmdProtocol(LineReceiver):

    delimiter = '\n'

    def connectionMade(self):
        self.client_ip = self.transport.getPeer()[1]
        log.msg("Client connection from %s" % self.client_ip)
        if len(self.factory.clients) >= self.factory.clients_max:
            log.msg("Too many connections. bye !")
            self.client_ip = None
            self.transport.loseConnection()
        else:
            self.factory.clients.append(self.client_ip)

    def connectionLost(self, reason):
        log.msg('Lost client connection.  Reason: %s' % reason)
        if self.client_ip:
            self.factory.clients.remove(self.client_ip)

    def lineReceived(self, line):
        log.msg('Cmd received from %s : %s' % (self.client_ip, line))

class MyFactory(ServerFactory):

    protocol = CmdProtocol

    def __init__(self, clients_max=10):
        self.clients_max = clients_max
        self.clients = []


if __name__ =="__main__"
	log.startLogging(sys.stdout)
	reactor.listenTCP(9999, MyFactory(2))
	reactor.run()
```

![测试例子](/res/img/blog/python学习/2015-01-14.png)

