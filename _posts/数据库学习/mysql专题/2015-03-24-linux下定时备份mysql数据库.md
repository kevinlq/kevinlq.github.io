---
layout: life
title: linux下mysql定时备份
category: 数据库学习
duoshuo: true
date: 2015-03-24
---


<!-- more -->


### 1. 在服务器上建立备份文件的存放文件夹

```
	sudo mkdir /usr/local/dbbackup
```

### 2. 编写备份脚本

```
	vi dbbackup.sh 
```

在里面编写如下内容

```
	mysqldump -uroot -p1W8a6N0g1Y3i3F1a1N99 cusflo | gzip > /usr/local/cusflo-dbback/cusflo`date +%Y-%m-%d_%H%M%S`.sql.gz

	cd  /usr/local/cusflo-dbback/

	rm -rf `find . -name '*.sql.gz' -mtime 30`

```

### 3.更改备份脚本权限


```
	sudo chmod +x dbbackup.sh 
```


### 4.修改crontab定时执行脚本


```
	crontab -e 
```


#### 若每天晚上23点30备份，添加如下代码:


```
	30 23 * * * /usr/local/dbbackscripts/dbbackup.sh
```





