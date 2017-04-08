---
layout : life
title: 远程开机包封装
category : Python学习
duoshuo: true
date : 2015-05-19
---

* 使用方法
 * 同一局域网内的机器，只需指定目标主机的MAC地址，就令远程主机开机。
 * 非同一局域网的情况下，除需指定目标主机的MAC地址，还需指定目标主机的地址。

<!-- more -->

* 示例：
 * 同一局域网内的使用示例：./wake_on_lan.py -m 12-34-56-78-90-AB
 * 非同一局域网的使用示例：./wake_on_lan.py -m 12-34-56-78-90-AB -a 8.8.8.8

```python
#coding=utf-8
from optparse import OptionParser
import socket
import struct

def wake(addr, mac):
    mac_data = []
    for i in range(0, 12, 2):
        mac_data.append(int(mac[i:i+2], 16))
    packet = struct.pack("!BBBBBB", 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    packet_mac = struct.pack("!BBBBBB", *mac_data)
    for i in range(0, 16):
        packet += packet_mac
    #print "len: ", len(packet), "data: ", packet
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    try:
        s.sendto(packet, addr)
        print "唤醒数据包发送完成", addr, mac
    finally:
        s.close()

def main():
    usage = "%prog [options]"
    parser = OptionParser(usage = usage)
    parser.add_option("-a", "--addr", dest="addr", help="Boardcast address", metavar="255.255.255.255")
    parser.add_option("-p", "--port", dest="port", help="Port", metavar="7")
    parser.add_option("-m", "--mac", dest="mac", help="MAC address", metavar="FF-FF-FF-FF-FF-FF")
    (options, args) = parser.parse_args()
    if not options.mac:
        parser.print_help()
        return
    addr = "255.255.255.255"
    port = 7
    mac = options.mac.replace("-", "")
    if options.addr:
        addr = options.addr
    if len(mac) != 12:
        print "无效的MAC地址: %s" % options.mac
        return
    if options.port:
        port = int(options.port)
    wake((addr, port), mac)

if __name__ == "__main__":
    main()

```
