---
layout : life
title: Python在windows下的服务程序
category : Python学习
duoshuo: true
date : 2015-06-09
---


Python程序作为Windows服务启动，需要安装pywin32包。下载路径：

[我是下载路径](http://sourceforge.net/projects/pywin32/files/pywin32/)

<!-- more -->


```python
import win32serviceutil 
import win32service 
import win32event 

class PythonService(win32serviceutil.ServiceFramework): 
    """
    Usage: 'PythonService.py [options] install|update|remove|start [...]|stop|restart [...]|debug [...]'
    Options for 'install' and 'update' commands only:
     --username domain\username : The Username the service is to run under
     --password password : The password for the username
     --startup [manual|auto|disabled|delayed] : How the service starts, default = manual
     --interactive : Allow the service to interact with the desktop.
     --perfmonini file: .ini file to use for registering performance monitor data
     --perfmondll file: .dll file to use when querying the service for
       performance data, default = perfmondata.dll
    Options for 'start' and 'stop' commands only:
     --wait seconds: Wait for the service to actually start or stop.
                     If you specify --wait with the 'stop' option, the service
                     and all dependent services will be stopped, each waiting
                     the specified period.
    """
    #服务名
    _svc_name_ = "PythonService"
    #服务显示名称
    _svc_display_name_ = "Python Service Demo"
    #服务描述
    _svc_description_ = "Python service demo."

    def __init__(self, args): 
        win32serviceutil.ServiceFramework.__init__(self, args) 
        self.hWaitStop = win32event.CreateEvent(None, 0, 0, None)
        self.logger = self._getLogger()
        self.isAlive = True
        
    def _getLogger(self):
        import logging
        import os
        import inspect
        
        logger = logging.getLogger('[PythonService]')
        
        this_file = inspect.getfile(inspect.currentframe())
        dirpath = os.path.abspath(os.path.dirname(this_file))
        handler = logging.FileHandler(os.path.join(dirpath, "service.log"))
        
        formatter = logging.Formatter('%(asctime)s %(name)-12s %(levelname)-8s %(message)s')
        handler.setFormatter(formatter)
        
        logger.addHandler(handler)
        logger.setLevel(logging.INFO)
        
        return logger

    def SvcDoRun(self):
        import time
        self.logger.error("svc do run....") 
        while self.isAlive:
            self.logger.error("I am alive.")
            time.sleep(1)
        # 等待服务被停止 
        #win32event.WaitForSingleObject(self.hWaitStop, win32event.INFINITE) 
            
    def SvcStop(self): 
        # 先告诉SCM停止这个过程 
        self.logger.error("svc do stop....")
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING) 
        # 设置事件 
        win32event.SetEvent(self.hWaitStop) 
        self.isAlive = False

if __name__=='__main__': 
    win32serviceutil.HandleCommandLine(PythonService)

```

* 安装服务

```python
	python PythonService.py install
```

* 让服务自动启动

```python
	python PythonService.py --startup auto install 
```

* 启动服务

```python
	python PythonService.py start
```

* 重启服务

```python
	python PythonService.py restart
```

* 停止服务

```python
	python PythonService.py stop
```

* 删除/卸载服务

```python
	python PythonService.py remove
```
