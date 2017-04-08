---
layout : life
title : nginx 配置https并自签名证书
category : 环境搭建
duoshuo : true
date : 2016-10-28
---


******
	作者: daodaoliang
    版本: V1.0.1
    邮箱： daodaoliang@yeah.net
	
参考链接： [这里][1] 和 [这里][2] 和 [官方文档][3]

<!-- more -->

### 1. 制作服务器证书

服务器CA私钥：

```
openssl genrsa -des3 -out ca.key 2048
```

制作解密后的CA私钥（一般无此必要）：

```
openssl rsa -in ca.key -out ca_decrypted.key
```

ca.crt CA根证书（公钥）：

```
openssl req -new -x509 -days 7305 -key ca.key -out ca.crt
```

### 2. 制作生成网站的证书并用CA签名认证

在这里，假设网站域名为 www.example.com

生成 www.example.com 证书私钥：

```
openssl genrsa -des3 -out www.example.com.pem 1024
```

制作解密后的 www.example.com 证书私钥：

```
openssl rsa -in www.example.com.pem -out www.example.com.key
```

生成签名请求：

```
openssl req -new -key www.example.com.pem -out www.example.com.csr
```

在common name中填入网站域名，如 www.example.com 即可生成改站点的证书，同时也可以使用泛域名如 *.example.com 来生成所有二级域名可用的网站证书。

用CA进行签名：

```
openssl ca -policy policy_anything -days 1460 -cert ca.crt -keyfile ca.key -in www.example.com.csr -out www.example.com.crt
```

其中，policy参数允许签名的CA和网站证书可以有不同的国家、地名等信息，days参数则是签名时限。

如果在执行签名命令时，出现“I am unable to access the ../../CA/newcerts directory”

修改/etc/pki/tls/openssl.cnf中“dir = ./CA”

然后：

```
mkdir -p CA/newcerts

touch CA/index.txt

touch CA/serial

echo "01" > CA/serial
```

再重新执行签名命令。

最后，把ca.crt的内容粘贴到 www.example.com.crt 后面。这个比较重要！因为不这样做，可能会有某些浏览器不支持。

好了，现在https需要到的网站私钥 www.example.com.key 和网站证书 www.example.com.crt 都准备完毕。接下来开始配置服务端。

### 3. 配置nginx

新开一个虚拟主机，并在server{}段中设置：

```
listen 443;

ssl on;

ssl_certificate /path/to/www.example.com.crt;

ssl_certificate_key /path/to/www.example.com.key;
```

其中的路径是刚刚生成的网站证书的路径。

然后使用一下命令检测配置和重新加载nginx：

```
检测配置：
nginx -t
重新加载：
nginx -s reload
```

### 4. 优化nginx配置

**https和http共存:**

```
server {
        listen 80;
        listen 443 ssl;
        server_name zou.lu;
        root /var/www/html;

        #ssl on;   //这行必须要注释掉
        ssl_certificate /usr/local/nginx/conf/zou_lu.crt;
        ssl_certificate_key /usr/local/nginx/conf/zoulukey.pem;
    }
```

**优化性能:**

在http{}中加入：

```
ssl_session_cache shared:SSL:10m;

ssl_session_timeout 10m;
```

据官方文档所述，cache中的1m可以存放4000个session。

在配置https的虚拟主机server{}中加入：

```
keepalive_timeout 70;
```

**PS:常见错误:**

有时候，会发现，在phpMyAdmin等程序登入后会错误地跳转http的问题。解决方法是定位至“location ~ .*\.(php|php5)?${}”在include fcgi.conf;或者在fastcgi_param配置后面加上：

```
fastcgi_param HTTPS on;

fastcgi_param HTTP_SCHEME https;
```

[1]:http://blog.creke.net/762.html
[2]:http://blog.csdn.net/eclothy/article/details/45932733
[3]:http://nginx.org/en/docs/http/configuring_https_servers.html

