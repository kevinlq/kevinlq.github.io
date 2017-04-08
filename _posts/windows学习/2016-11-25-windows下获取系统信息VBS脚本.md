---
layout : life
title : windows下获取系统信息VBS脚本
category : windows学习
duoshuo : true
date : 2016-11-25
---


******

	作者: daodaoliang
	版本: V 0.0.1
	日期: 2016年11月25日

<!-- more -->

*******


```vb
'********************************************************************
'*                                                                  *
'*                           Begin of File                          *
'*                                                                  *
'*******************************************************************


On Error Resume Next

Const forwriting=2
Const forreading=1
Dim oFSO,oF,eventlog


Text = "此脚本运行不会对您的系统造成任何损害或恶意行为" &VbCrLf&vbCrlf& "通过连接WMI提供的公共接口枚举系统相关信息"&VbCrLf&VbCrLf& "按确定键运行30秒左右会生成html格式报告"
Title_Text = "bkReport  Version 2.2"
MsgBox Text,vbExclamation+vbSystemModal,Title_Text

dtmStart = Now()

strComputer = "."
Set oShell = WScript.CreateObject("WScript.Shell")
hostname=oShell.ExpandEnvironmentStrings("%computername%")
MainReport=hostname&"_Report.html"
EventReport=hostname&"_EventLog.html"
datec=Now()
Set oFSO = CreateObject("Scripting.FileSystemObject")
Set oF = oFSO.CreateTextFile(MainReport)
Set eventlog=oFSO.CreateTextFile(EventReport)

strComputer = "."
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")

Set colComputers = objWMIService.ExecQuery _
    ("Select * from Win32_ComputerSystem")

For Each objComputer in colComputers
   Select  Case objComputer.DomainRole
    Case "0" machine= "独立工作站"
    Case "1" machine= "成员工作站"
    Case "2" machine= "独立服务器"
    Case "3" machine= "成员服务器"
    Case "4" machine= "备份服务器"
    Case "5" machine= "主域控制器"
   End Select
Next
oF.WriteLine "<html>"
oF.WriteLine "<head><title>MainReport Build by bkReport.vbs</title></head>"
oF.Writeline "<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>"
oF.WriteLine "<h2><font color=MidnightBlue><center>"&hostname&"@"&machine&"   快照信息 </center></font></h2>"
oF.WriteLine "<p align=right><font size=2>date: " & Now()&"</font></p>"
oF.Writeline "<hr width=80% color=#ff8000>"
oF.Writeline "<p>&nbsp;</p>"
oF.WriteLine "<OL><LI><a href=#here1>系统摘要</a>"
oF.WriteLine "<LI><a href=#here2>BIOS信息</a>"
oF.WriteLine "<LI><a href=#here3>环境变量</a>"
oF.WriteLine "<LI><a href=#here4>系统文件内容</a>"
oF.WriteLine "<LI><a href=#here5>网络状态</a>"
oF.WriteLine "<LI><a href=#here6>磁盘和共享</a>"
oF.WriteLine "<LI><a href=#here7>进程</a>"
oF.WriteLine "<LI><a href=#here8>进程ID对应的启动服务</a>"
oF.WriteLine "<LI><a href=#here9>服务</a>"
oF.WriteLine "<LI><a href=#here10>补丁</a>"
oF.WriteLine "<LI><a href=#here11>软件</a>"
oF.WriteLine "<LI><a href=#here12>帐号</a>"
oF.WriteLine "<LI><a href=#here13>AT创建的计划任务</a>"
oF.WriteLine "<LI><a href=#here14>重要文件属性</a>"
oF.WriteLine "<LI><a href=#here15>自启动项</a>"
oF.WriteLine "<LI><a href=#here16>注册表</a>"
oF.WriteLine "<LI><a href=#here17>系统日志</a></LI></OL>"
oF.Writeline "<p>&nbsp;</p><p>&nbsp;</p>"
'---------------------------------
'系统摘要
' --------------------------------

Set objWMIService = GetObject("winmgmts:\\" & strComputer)
Set colOperatingSystems = objWMIService.InstancesOf("Win32_OperatingSystem")
oF.WriteLine "<h3><font color=MidnightBlue><a name=here1>"&"[  1. 系统摘要  ]"&"</a></font></h3>"
oF.WriteLine "<table BORDER=1 style=font-size:9pt cellspacing=1 align=CENTER>"
For Each objOperatingSystem In colOperatingSystems
ostype=objOperatingSystem.Name
LastbootUpTime=objOperatingSystem.LastBootUpTime
'     oF.WriteLine "<tr><td><b>" &"Name:            "&"</b></td><td>"& objOperatingSystem.Name &"</td></tr>"
 '    oF.WriteLine  "<tr><td><b>" & "Caption:         " &"</b></td><td>" &objOperatingSystem.Caption &"</td></tr>"    
  '   oF.WriteLine  "<tr><td><b>" & "LastBootUpTime:  "&"</b></td><td>"&objOperatingSystem.LastBootUpTime &"</td></tr>"
   '  oF.WriteLine  "<tr><td><b>" & "LocalDateTime:   " &"</b></td><td>"&objOperatingSystem.LocalDateTime &"</td></tr>"
    ' oF.WriteLine  "<tr><td><b>"& "Version:         " &"</b></td><td>"&objOperatingSystem.Version    &"</td></tr>"
     'oF.WriteLine  "<tr><td><b>"& "Windows Directory: "&"</b></td><td>"&objOperatingSystem.WindowsDirectory&"</td></tr>"
Next

Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
Set colItems = objWMIService.ExecQuery("Select * from Win32_MemoryArray")

For Each objItem in colItems
memsize =CLng(objItem.EndingAddress /1024)
'    oF.WriteLine "memory size: " & objItem.EndingAddress  /1024 &" MB"
Next

Set objWMI = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
Set col = objWMI.ExecQuery("Select * from Win32_Processor")

For Each obj in col


    cpuinfo=obj.Name
     Next

oF.WriteLine "<table BORDER=1 style=font-size:9pt> <tr><th CLASS=pt bgColor=#808080>OS</th><th bgColor=#808080>HostName</th><th bgColor=#808080>Memory</th><th bgColor=#808080>CPU</th><th bgColor=#808080>LastBootUpTime</th></tr> "
oF.WriteLine "<tr><td>"&ostype&"</td><td>"&hostname&"</td><td>"&memsize&"MB</td><td>"&cpuinfo&"</td><td>"& LastBootUpTime &"</td></tr>"


 strComputer = "."
Set objWMIService = GetObject("winmgmts:\\"& strComputer & "\root\cimv2")
Set colAdapters = objWMIService.ExecQuery _
  ("SELECT * FROM Win32_NetworkAdapterConfiguration WHERE IPEnabled = True")

 oF.WriteLine "<tr><th bgColor=#808080>Adapter</th><th bgColor=#808080>MACaddr</th><th bgColor=#808080>IPaddr</th><th bgColor=#808080>Subnet</th><th bgColor=#808080>gateway</th></tr> "

For Each objAdapter in colAdapters
  Adapter= objAdapter.Description
  MACaddr= objAdapter.MACAddress

   If Not IsNull(objAdapter.IPAddress) Then
      For i = 0 To UBound(objAdapter.IPAddress)
        IPaddr=objAdapter.IPAddress(i)

      Next
   End If
   If Not IsNull(objAdapter.IPSubnet) Then
      For i = 0 To UBound(objAdapter.IPSubnet)
        Subnet=objAdapter.IPSubnet(i)
      Next
   End If
   If Not IsNull(objAdapter.DefaultIPGateway) Then
      For i = 0 To UBound(objAdapter.DefaultIPGateway)
         gateway = objAdapter.DefaultIPGateway(i)
      Next
   End If


oF.WriteLine "<tr><td>"&Adapter&"</td><td>"&MACaddr&"</td><td>"&IPaddr&"</td><td>"&Subnet&"</td><td>"&gateway&"</td></tr>"

 Next
 oF.WriteLine "</table>"  


 '--------------------------------------------------------------------
 'BIOS信息
 '--------------------------------------------------------------------
 oF.WriteLine "<h3><font color=MidnightBlue><a name=here2>"&"[  2. BIOS信息  ]"&"</a></font ></h3>"
 Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colBIOS = objWMIService.ExecQuery _
    ("Select * from Win32_BIOS")
     oF.WriteLine "<TABLE BORDER=1 WIDTH=800 style=font-size:9pt cellspacing=1><TR><TD>"
For Each objBIOS in colBIOS
    oF.WriteLine  "<p><font size=2>"&"Build Number: " &objBIOS.BuildNumber&"</font></p>"
   oF.WriteLine  "<p><font size=2>"&"Current Language: " &objBIOS.CurrentLanguage&"</font></p>"
    oF.WriteLine  "<p><font size=2>"&"Installable Languages: " &objBIOS.InstallableLanguages&"</font></p>"
   oF.WriteLine  "<p><font size=2>"& "Manufacturer: " &objBIOS.Manufacturer&"</font></p>"
   oF.WriteLine  "<p><font size=2>"& "Name: "&objBIOS.Name&"</font></p>"
   oF.WriteLine  "<p><font size=2>"& "Primary BIOS: "& objBIOS.PrimaryBIOS&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "Release Date: " &objBIOS.ReleaseDate&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "Serial Number: " & objBIOS.SerialNumber&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "SMBIOS Version: " &objBIOS.SMBIOSBIOSVersion&"</font></p>"

   oF.WriteLine  "<p><font size=2>"&"SMBIOS Minor Version: " &objBIOS.SMBIOSMinorVersion&"</font></p>"
   oF.WriteLine  "<p><font size=2>"& "SMBIOS Present: " &objBIOS.SMBIOSPresent&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "Status: " &objBIOS.Status&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "Version: " &objBIOS.Version&"</font></p>"
    oF.WriteLine  "<p><font size=2>"& "BIOS Characteristics: "&"</font></p>"

Next
 oF.WriteLine  "</TD></TR></TABLE>"



 '-----------------------------------------------------------------
 '环境变量
 '-----------------------------------------------------------------
 oF.WriteLine "<h3><font color=MidnightBlue><a name=here3>"&"[  3. 环境变量  ]"&"</a></font </h3>"
 Set wshshell = CreateObject("WScript.Shell")
oF.WriteLine "<TABLE BORDER=1 WIDTH=800 style=font-size:9pt cellspacing=1><TR><TD>"
oF.writeline "<b><font size=3>SYSTEM variables</font></b>"
For Each EnvirSYSTEM In wshshell.Environment("SYSTEM")
 oF.writeline "<p><font size=2>" &EnvirSYSTEM &"</font></p>"

Next
  oF.writeline "<b><font size=3>PROCESS variables</font></b>"
For Each EnvirPROCESS In wshshell.Environment("PROCESS")
 oF.writeline "<p><font size=2>"&EnvirPROCESS &"</font></p>"
Next
  oF.writeline "<b><font size=3>USER variables</font></b>"
For Each EnvirUSER In wshshell.Environment("USER")
 oF.writeline "<p><font size=2>"&EnvirUSER &"</font></p>"
Next
 oF.writeline "<b><font size=3>VOLATILE variables</font></b>"
For Each EnvirVOLATILE In wshshell.Environment("VOLATILE")
 oF.writeline "<p><font size=2>"&EnvirVOLATILE &"</font></p>"
Next

set wshshell=Nothing
 oF.WriteLine  "</TD></TR></TABLE>"
 '----------------------------------
 '系统文件
 '----------------------------------
 oF.WriteLine "<h3><font color=MidnightBlue><a name=here4>"&"[  4. 系统文件内容  ]"&"</a></font></h3>"

 Set wshshell=wscript.CreateObject("WScript.shell")
autoexecBAT=wshshell.expandEnvironmentStrings("%systemdrive%")&"\autoexec.bat"
configSYS=wshshell.expandEnvironmentStrings("%systemdrive%")&"\config.sys"
bootINI=wshshell.expandEnvironmentStrings("%systemdrive%")&"\boot.ini"
winINI=wshshell.expandEnvironmentStrings("%systemroot%")&"\win.ini"
systemINI=wshshell.expandEnvironmentStrings("%systemroot%")&"\system.ini"
autoexecNT=wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\autoexec.nt"
configNT=wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\config.nt"
HOSTS=wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\drivers\etc\hosts"
Function readtxt (txtfile)
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.OpenTextFile(txtfile, 1)
 oF.WriteLine "<TABLE BORDER=1 WIDTH=800 cellspacing=1><TR><TD><b><font size=3>" &txtfile &"</font></b>"
Do Until objFile.AtEndOfStream
    strCharacters = objFile.Readline
     oF.WriteLine "<p><font size=2>" &strCharacters &"</font></p>"
Loop
oF.WriteLine  "</TD></TR></TABLE>"
End Function
readtxt (autoexecBAT)
readtxt (configSYS)
readtxt (bootINI)
readtxt (winINI)
readtxt (systemINI)
readtxt (autoexecNT)
readtxt (configNT)
readtxt (HOSTS)


 '---------------------------------
'网络状态
' --------------------------------
oF.WriteLine "<h3>"&"<font color=MidnightBlue><a name=here5>"&"[  5. 网络状态  ]"&"</a></font >"&"</h3>"

Dim f1
Set ws=WScript.CreateObject ("wscript.shell")
ws.run "%comspec% /c echo ######################### ipconfig /all #########################  > ttmp",0,True
ws.run "%comspec% /c ipconfig /all  >> ttmp",0,True
ws.run "%comspec% /c echo ######################### netstat -r ######################### >> ttmp",0,True
ws.run "%comspec% /c netstat -r >>ttmp",0,True
ws.run "%comspec% /c echo ######################### arp -a ######################### >> ttmp",0,True
ws.run "%comspec% /c arp -a >>ttmp",0,True
ws.run "%comspec% /c echo ######################### netstat -an ######################### >> ttmp",0,True
ws.run "%comspec% /c netstat -an >>ttmp",0,True
ws.run "%comspec% /c echo ######################### nbtstat -r ######################### >> ttmp",0,True
ws.run "%comspec% /c nbtstat -r >>ttmp",0,True
ws.run "%comspec% /c echo ######################### nbtstat -n  ######################### >> ttmp",0,True
ws.run "%comspec% /c nbtstat -n >>ttmp",0,True
ws.run "%comspec% /c echo ######################### nbtstat -S ######################### >> ttmp",0,True
ws.run "%comspec% /c nbtstat -S >>ttmp",0,True
ws.run "%comspec% /c echo ######################### netstat -es ######################### >> ttmp",0,True
ws.run "%comspec% /c netstat -es >>ttmp",0,True

Set f1=oFSO.OpenTextFile ("ttmp",forreading,True)
oF.WriteLine "<TABLE BORDER=1 WIDTH=800 cellspacing=1><TR><TD>"
Do Until  f1.AtEndOfStream
a=f1.ReadLine
oF.WriteLine "<p><font size=2>" &a&"</font></p>"
Loop
oF.WriteLine  "</TD></TR></TABLE>"
f1.Close
ofso.DeleteFile "ttmp",True

'---------------------------------
'检查磁盘和共享
' --------------------------------
oF.WriteLine "<h3><font color=MidnightBlue><a name=here6>"&"[ 6. 磁盘和共享  ]"&"</a></font ></h3>"
Sub Enudisk
oF.writeline "<table BORDER=1 style=font-size:9pt cellspacing=1>"
On Error Resume Next
'Enumerate Disk Drive Properties 用了除法转成MB单位，并用clng函数取整
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set colDrives = objFSO.Drives
of.writeline "<tr><th bgColor=#808080>Drive letter: </th><th bgColor=#808080>Volume name: </th><th bgColor=#808080>File system: </th><th bgColor=#808080>Total size: </th><th bgColor=#808080>Free space: </th></tr>"
For Each objDrive in colDrives
    of.writeline "<tr><td>" & objDrive.DriveLetter &"</td>"&_
    "<td>" & objDrive.VolumeName&"</td>"&_
   "<td>" & objDrive.FileSystem&"</td>"&_
    "<td>" &  clng(objDrive.TotalSize /1024 /1024 ) &"MB"&"</td>"&_
   "<td>" &  CLng(objDrive.FreeSpace /1024 /1024 ) &"MB"&"</td></tr>"

Next
If Err <>0 Then

Err.Clear
End If

End Sub

Enudisk


strComputer = "."
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
Set colItems = objWMIService.ExecQuery("Select * from Win32_Share",,48)

    of.writeline "<tr><th bgColor=#808080>Path: </th><th bgColor=#808080>Name: </th><th bgColor=#808080>Caption:</th><th bgColor=#808080>Status:</th><th bgColor=#808080>MaximumAllowed:</th></tr>"

For Each objItem in colItems
    of.writeline "<tr><td>"& objItem.Path&"</td>"&  _
     "<td>" & objItem.Name&"</td>"& _
    "<td>"  & objItem.Caption&"</td>"& _
    "<td>" & objItem.Status&"</td>"&_
     "<td>" &objItem.MaximumAllowed &"</td></tr>"

Next

oF.WriteLine "</table>"     

'---------------------------------
'检测进程  
' --------------------------------

oF.WriteLine "<h3><font color=MidnightBlue><a name=here7>"&"[  7. 进程  ]"&"</a></font ></h3>"
Set objWMI = _
   GetObject("winmgmts:{impersonationLevel=impersonate}//./root/cimv2")
Set colProcessList = objWMI.ExecQuery("SELECT * FROM Win32_Process")
Set objFSO = CreateObject("Scripting.FileSystemObject")

oF.WriteLine "<table BORDER=1  WIDTH=%100 style=font-size:9pt cellspacing=1>"
oF.writeline "<tr><th bgColor=#808080>UserDomain</th><th bgColor=#808080>Ownership</th><th bgColor=#808080>CreationDate</th><th bgColor=#808080>Process ID:</th><th bgColor=#808080>Process Name:</th>"&"<th bgColor=#808080>Executable Path:</th><th bgColor=#808080>Size:</th><th bgColor=#808080>File created:</th><th bgColor=#808080>File last modified:</th><th bgColor=#808080>File last accessed:</th></tr>"
For Each colprocess In colProcessList

colProperties=colProcess.GetOwner(strNameOfUser,strUserDomain)
Set objFile = objFSO.GetFile (colProcess.ExecutablePath)
   oF.WriteLine "<tr><td>" & strUserDomain  & "</td><td>"&strNameOfUser&"</td><td>"& colProcess.CreationDate&"</td><td>"&colProcess.Processid &"</td><td>" &colprocess.name&"</td><td>"&colProcess.ExecutablePath & "</td>"
oF.WriteLine  "<td>" & objFile.Size &"</td>"
oF.WriteLine "<td>" &  objFile.DateCreated &"</td>"
oF.WriteLine  "<td>" & objFile.DateLastModified &"</td>"
oF.WriteLine  "<td>" & objFile.DateLastAccessed &"</td></tr>"
Next

oF.WriteLine "</table>"
'---------------------------------
'检测进程id对应的服务  
' --------------------------------
oF.WriteLine "<h3><font color=MidnightBlue><a name=here8>"&"[  8. 进程ID对应的启动服务  ]"&"</a></font ></h3>"
  set objIdDictionary = CreateObject("Scripting.Dictionary")
strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colServices = objWMIService.ExecQuery _
    ("Select * from Win32_Service Where State <> 'Stopped'")
For Each objService in colServices
    If objIdDictionary.Exists(objService.ProcessID) Then
    Else
        objIdDictionary.Add objService.ProcessID, objService.ProcessID
    End If
Next
colProcessIDs = objIdDictionary.Items
oF.WriteLine "<TABLE BORDER=1 style=font-size:9pt cellspacing=1>"
For i = 0 to objIdDictionary.Count - 1
    Set colServices = objWMIService.ExecQuery _
        ("Select * from Win32_Service Where ProcessID = '" & _
            colProcessIDs(i) & "'")
   oF.WriteLine "<TR><TH bgColor=#808080>Process ID: </TH><td>" &colProcessIDs(i)&"<td/>"
    For Each objService in colServices
          oF.WriteLine "<tr><td COLSPAN=2>" & objService.DisplayName &"</td></tr>"
    Next
Next
 oF.WriteLine  "</TABLE>"
'---------------------------------
'检测服务  
' --------------------------------
oF.WriteLine "<h3>"&"<font color=MidnightBlue><a name=here9>"&"[   9. 服务  ]"&"</a></font >"&"</h3>"
strComputer = "."  


Set objWMIService = GetObject("winmgmts:" & _
    "{impersonationLevel=Impersonate}!\\" & strComputer & "\root\cimv2")
Set colStartServices = objWMIService.ExecQuery _
  ("SELECT DisplayName,State FROM Win32_Service WHERE State = 'Running'")

oF.WriteLine "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"
oF.WriteLine "<tr><th bgColor=#808080>Running server list:</th></tr>"
of.writeline "<tr><td>"
For Each objService in colStartServices
    of.writeline  "<p>" & objService.DisplayName & "</p>"  
Next
oF.WriteLine "</td></tr></table>"                  

Set objWMIService = GetObject("winmgmts:\\" & strComputer)
Set colServices = objWMIService.InstancesOf("Win32_Service")

oF.WriteLine "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"

oF.WriteLine "<tr><th bgColor=#808080>Name:</th><th bgColor=#808080>Display Name:</th><th bgColor=#808080>Path Name:</th><th bgColor=#808080>Start Mode:</th><th bgColor=#808080>State:</th></tr>"
For Each objService In colServices
    oF.WriteLine "<tr>"&"<td>" &objService.Name    &"</td>"&_    
                "<td>" & objService.DisplayName&"</td>"& _
                "<td>" & objService.PathName     &"</td>"& _
                "<td>" & objService.StartMode    &"</td>"& _
                 "<td>" &objService.State        &"</td>"

Next                                           
oF.WriteLine "</tr></table>"                                




'---------------------------------
'检测补丁
' --------------------------------
oF.WriteLine "<h3>"&"<font color=MidnightBlue><a name=here10>"&"[  10. 补丁  ]"&"</a></font >"&"</h3>"

strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colOperatingSystems = objWMIService.ExecQuery _
    ("Select * from Win32_OperatingSystem")
For Each objOperatingSystem in colOperatingSystems
    PACKVER = objOperatingSystem.ServicePackMajorVersion  _
        & "." & objOperatingSystem.ServicePackMinorVersion
        Next

Set objWMIService = GetObject("winmgmts:" _
& "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colQuickFixes = objWMIService.ExecQuery _
("Select * from Win32_QuickFixEngineering")

oF.WriteLine "<table BORDER=1 style=font-size:9pt cellspacing=1>"
oF.WriteLine "<tr><th bgColor=#808080>PackVersion: </th><td>"&PACKVER&"</td></tr>"
'   oF.WriteLine "<td>Computer: </td>"
   oF.WriteLine "<tr><th bgColor=#808080>Description:</th>"
   oF.WriteLine "<th bgColor=#808080>Hotfix ID:</th>"
 '   oF.WriteLine "<td>Installation Date:</td>"
 '  oF.WriteLine "<td>Installed By:</td>"
   oF.WriteLine "</tr>"
For Each objQuickFix in colQuickFixes
   oF.WriteLine "<tr>"
'    oF.WriteLine "<td>" & objQuickFix.CSName & "</td>"
   oF.WriteLine "<td>" & objQuickFix.Description & "</td>"
   oF.WriteLine "<td>" & objQuickFix.HotFixID & "</td>"
'   oF.WriteLine "<td>" & objQuickFix.InstallDate & "</td>"
'    oF.WriteLine "<td>" & objQuickFix.InstalledBy & "</td>"
   oF.WriteLine "</tr>"
Next
oF.WriteLine "</table>"

'---------------------------------
'检测软件
' --------------------------------
'get installed software
oF.WriteLine "<h3>"&"<font color=MidnightBlue><a name=here11>"&"[  11. 软件  ]"&"</a></font >"&"</h3>"
oF.WriteLine "<table BORDER=1 style=font-size:9pt cellspacing=1>"
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
oF.WriteLine "<tr><th bgColor=#808080>Caption:</th><th bgColor=#808080>version:</th></tr>"
Set colApps = objWMIService.ExecQuery("Select * from Win32_Product")
For Each objApp in colApps
oF.WriteLine "<tr><td>"&objApp.Caption &"</td><td>"& objApp.Version &"</td></tr>"
Next

Dim oRegistry, sBaseKey, iRC, sKey, arSubKeys, sValue
Const HKLM = &H80000002 'HKEY_LOCAL_MACHINE
Set oRegistry = GetObject("winmgmts:\\" & strComputer  & _
"/root/default:StdRegProv")

sBaseKey = "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\"

iRC = oRegistry.EnumKey(HKLM, sBaseKey, arSubKeys)
For Each sKey In arSubKeys
  iRC = oRegistry.GetStringValue(HKLM, sBaseKey & sKey, _
  "DisplayName", sValue)

  version= oRegistry.GetStringValue(HKLM, sBaseKey & sKey, _
  "DisplayVersion", sVer)


  If iRC <> 0 Then
    oRegistry.GetStringValue HKLM, sBaseKey & sKey, _
    "QuietDisplayName", sValue
  End If


  If sValue <> "" Then
    	of.writeline"<tr><td>" & sValue & "</td><td>" & sver & "</td></tr>"

  ElseIf Err <> 0 Then
	of.writeline"<tr><td>"& "Installed App Name Not Available" & "</td></tr>"
  	err.clear
	err.Number=0
  End If
Next




Const ADMINISTRATIVE_TOOLS = &H2f&
Set objShell = CreateObject("Shell.Application")
Set objFolder = objShell.Namespace(ADMINISTRATIVE_TOOLS)
Set objTools = objFolder.Items
oF.WriteLine "<tr><th bgColor=#808080>Admin tools:</th></tr>"
For i = 0 to objTools.Count - 1
   oF.WriteLine "<tr><td>"& objTools.Item(i)&"</td></tr>"
Next
oF.WriteLine "</table>"



'---------------------------------
'检测帐号
' --------------------------------

oF.WriteLine "<h3><font color=MidnightBlue><a name=here12>"&"[  12. 帐号  ]"&"</a></font ></h3>"

strComputer = "."
Set objWMIService = GetObject("winmgmts:\\" & strComputer & "\root\cimv2")
Set colItems = objWMIService.ExecQuery("Select * from Win32_UserAccount",,48)
oF.WriteLine "<table width=100% BORDER=1 style=font-size:9pt  cellspacing=1>"
    oF.writeline "<tr><th bgColor=#808080>" &"Name: "& "</th>"_
    &"<th bgColor=#808080>"&"Description: "& "</th>"_
    &"<th bgColor=#808080>"&"Lockout: "& "</th>"_
     &"<th bgColor=#808080>"&"PasswordChangeable: "& "</th>"_
     &"<th bgColor=#808080>"&"PasswordExpires: "& "</t>"_
     &"<th bgColor=#808080>"&"SID: " & "</th>"_
     &"<th bgColor=#808080>"& "Status: "& "</th>"_
     &"<th bgColor=#808080>"& "administrators: "& "</th>"
For Each objItem in colItems
 strUser = objItem.Name

Set objGroup = GetObject("WinNT://" & strComputer & "/Administrators")

   oF.writeline  "<tr><td>" & objItem.Name &"</td><td>" & _
    objItem.Description &"</td><td>" & _
    objItem.Lockout &"</td><td>" & _
    objItem.PasswordChangeable &"</td><td>" & _
    objItem.PasswordExpires &"</td><td>" & _
    objItem.SID &"</td><td>" & _
    objItem.Status&"</td>"

    For Each objUser in objGroup.Members
    If objUser.Name = strUser Then
        oF.writeline "<td>True</td>"

    End If
   Next
Next

On Error Resume Next
Set objNetwork = CreateObject("Wscript.Network")
strComputer = objNetwork.ComputerName

strPassword = ""

Set colAccounts = GetObject("WinNT://" & strComputer)
colAccounts.Filter = Array("user")

For Each objUser In colAccounts
    objUser.ChangePassword strPassword, strPassword
    If Err = 0 or Err = -2147023569 Then
        oF.writeline  "<p><font size=2>" &objUser.Name &  " password is null !.</font></p>"
    End If
    Err.Clear
Next
oF.WriteLine "</tr>"
oF.WriteLine "</table>"

'---------------------------------
'检查计划任务
' --------------------------------
oF.WriteLine "<h3><font color=MidnightBlue><a name=here13>"&"[  13. AT创建的计划任务  ]"&"</a></font ></h3>"
oF.writeline "<table width=100% BORDER=1 style=font-size:9pt cellspacing=1>"
'oF.writeline "<tr><th bgColor=#808080>Caption:</th><th bgColor=#808080>Command:</th><th bgColor=#808080>Days Of Month:</th><th bgColor=#808080>Days Of Week:</th><th bgColor=#808080>Description: </th><th bgColor=#808080>Elapsed Time:</th><><></th><><><>
strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colScheduledJobs = objWMIService.ExecQuery _
    ("Select * from Win32_ScheduledJob")


For Each objJob In colScheduledJobs
    oF.writeline "<tr><th bgColor=#808080>Caption:</th><td> " & objJob.Caption &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Command:</th><td> " & objJob.Command &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Days Of Month: </th><td>" & objJob.DaysOfMonth &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Days Of Week: </th><td>" & objJob.DaysOfWeek &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Description: </th><td>" & objJob.Description &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Elapsed Time: </th><td>" & objJob.ElapsedTime &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Install Date: </th><td>" & objJob.InstallDate &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Interact with Desktop: </th><td>" & objJob.InteractWithDesktop &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Job ID: </th><td>" & objJob.JobID &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Job Status: </th><td>" & objJob.JobStatus &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Name: </th><td>" & objJob.Name &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Notify: </th><td>" & objJob.Notify &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Owner: </th><td>" & objJob.Owner &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Priority: </th><td>" & objJob.Priority &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Run Repeatedly: </th><td>" & objJob.RunRepeatedly &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Start Time: </th><td>" & objJob.StartTime &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Status: </th><td>" & objJob.Status &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Time Submitted: </th><td>" & objJob.TimeSubmitted &"</td></tr>"
    oF.writeline "<tr><th bgColor=#808080>Until Time: </th><td>" & objJob.UntilTime &"</td></tr>"
Next
oF.writeline "</table>"


'----------------------------------
'获取文件信息函数
'----------------------------------

Function getfileinfo (targetfiles)
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.GetFile (targetfiles)


oF.WriteLine  "<tr><td>" & objFile.Path &"</td>"
oF.WriteLine  "<td>" & objFile.Size &"</td>"
oF.WriteLine  "<td>" & objFile.Type &"</td>"
oF.WriteLine "<td>" & objFile.DateCreated &"</td>"
oF.WriteLine  "<td>" & objFile.DateLastModified &"</td>"
oF.WriteLine  "<td>" & objFile.DateLastAccessed &"</td></tr>"

End Function
oF.WriteLine "<h3><font color=MidnightBlue><a name=here14>"&"[  14. 重要文件属性  ]"&"</a></font ></h3>"
oF.writeline "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"
oF.WriteLine "<tr><th bgColor=#808080>Path:</th><th bgColor=#808080>Size:</th><th bgColor=#808080>Type:</th><th bgColor=#808080>Date created:</th><th bgColor=#808080>Date last modified:</th><th bgColor=#808080>Date last accessed:</th></tr>"

getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\cmd.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\services.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\xcopy.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\arp.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\posix.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\cacls.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\debug.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\telnet.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\ftp.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\tftp.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\tracert.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\edlin.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\rsh.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\ipconfig.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\regedt32.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\finger.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\at.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\netstat.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\wscript.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\cscript.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\ping.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\atsvc.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\rcp.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\regedit.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\nslookup.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\runonce.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\net.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\route.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\copy.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\user.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\csrss.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\rexec.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\nbtstat.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\notepad.exe"
getfileinfo wshshell.expandEnvironmentStrings("%systemroot%")&"\system32\edit.com"

oF.writeline "</table>"


'get autorun
oF.WriteLine "<h3><font color=MidnightBlue><a name=here15>"&"[  15. 自启动项  ]"&"</a></font></h3>"
oF.writeline "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")
Set colStartupCommands = objWMIService.ExecQuery _
    ("Select * from Win32_StartupCommand")
    oF.writeline "<tr><th bgColor=#808080>User:</th><th bgColor=#808080>Location:</th><th bgColor=#808080>Name:</th><th bgColor=#808080>Command:</th>"
For Each objStartupCommand in colStartupCommands
    oF.writeline "<tr><td>"&objStartupCommand.User&"</td><td>"&objStartupCommand.location&"</td><td>"&objStartupCommand.name &"</td><td>"& objStartupCommand.command&"</td></tr>"
Next
oF.WriteLine "</table>"
'---------------------------------
'检查注册表自启动项目
' --------------------------------
oF.WriteLine "<h3><font color=MidnightBlue><a name=here16>"&"[  16. 注册表  ]"&"</a></font></h3>"

Const HKEY_CLASSES_ROOT = &H80000000
Const HKEY_CURRENT_USER = &H80000001
Const HKEY_LOCAL_MACHINE = &H80000002
Const HKEY_USERS=&H80000003
Const HKEY_CURRENT_CONFIG=&H80000005
Const REG_SZ = 1
Const REG_EXPAND_SZ = 2
Const REG_BINARY = 3
Const REG_DWORD = 4
Const REG_MULTI_SZ = 7

strComputer = "."

Set oReg=GetObject("winmgmts:{impersonationLevel=impersonate}!\\" &_
strComputer & "\root\default:StdRegProv")
On Error Resume Next
oF.writeline "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"
oF.writeline "<tr><th bgColor=#808080>读取此项内容</th><th bgColor=#808080>内容</th></tr>"

'enum subkeys\all entryNames
oReg.GetdwordValue HKEY_LOCAL_MACHINE,"SYSTEM\CurrentControlSet\Services\lanmanserver\parameters","Autoshareserver",strValue
If IsNull(strValue) Then

Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\lanmanserver\parameters\Autoshareserver" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"  
End If

oReg.GetstringValue HKEY_LOCAL_MACHINE,"Software\Microsoft\Windows NT\CurrentVersion\Winlogon","Shell",strValue
If IsNull(strValue) Then

Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Windows\shell" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"
End If

oreg.GetstringValue HKEY_LOCAL_MACHINE,"Software\Microsoft\Windows NT\CurrentVersion\Winlogon","Userinit",strValue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Windows\Userinit" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"
End If

oReg.GetstringValue HKEY_LOCAL_MACHINE,"Software\Microsoft\Windows NT\CurrentVersion\Windows","run",strValue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Windows\run" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"
End If

oReg.GetstringValue HKEY_CURRENT_USER,"Software\Microsoft\Windows NT\CurrentVersion\Windows","run",strValue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\Windows\run" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"
End If

oReg.GetstringValue HKEY_CURRENT_USER,"Software\Microsoft\Windows NT\CurrentVersion\Windows","load",strValue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_CURRENT_USER\Software\Microsoft\Windows NT\CurrentVersion\Windows\load" &"</td>"
oF.WriteLine "<td>"&strValue &"</td></tr>"
End If

oreg.getdwordvalue HKEY_LOCAL_MACHINE,"SYSTEM\CurrentControlSet\Services\EventLog\Application","Maxsize",dwordvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\Maxsize" &"</td>"
oF.WriteLine "<td>"&dwordValue &"</td></tr>"
End If

oreg.getdwordvalue HKEY_LOCAL_MACHINE,"SYSTEM\CurrentControlSet\Services\EventLog\security","Maxsize",dwordvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\security\Maxsize" &"</td>"
oF.WriteLine "<td>"&dwordValue &"</td></tr>"
End If

oreg.getdwordvalue HKEY_LOCAL_MACHINE,"SYSTEM\CurrentControlSet\Services\EventLog\system","Maxsize",dwordvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\system\Maxsize" &"</td>"
oF.WriteLine "<td>"&dwordValue &"</td></tr>"
End If

oreg.getdwordvalue HKEY_LOCAL_MACHINE,"SYSTEM\CurrentControlSet\Control\Lsa","restrictanonymous",dwordvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Lsa\RestrictAnonymous" &"</td>"
oF.WriteLine "<td>"&dwordValue &"</td></tr>"
End If

oreg.getdwordvalue HKEY_LOCAL_MACHINE,"System\CurrentControlSet\Services\NetBT\Parameters","SMBDeviceEnabled",dwordvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NetBT\Parameters\SMBDeviceEnabled" &"</td>"
oF.WriteLine "<td>"&dwordValue &"</td></tr>"
End If

oReg.GetStringValue HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows","Appinit_Dlls",stringvalue
If IsNull(strValue) Then
Else
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\Appinit_Dlls" &"</td>"
oF.WriteLine "<td>"&stringvalue &"</td></tr>"
End If

oF.writeline "<tr><th bgColor=#808080>枚举此键内容</th><th bgColor=#808080>项目名</th><th bgColor=#808080>数据类型</th></tr>"
'get entryNames values
RegEnum HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
RegEnum HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnceEx"
RegEnum HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce"
RegEnum HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows\CurrentVersion\RunServices"
RegEnum HKEY_LOCAL_MACHINE,"SOFTWARE\Microsoft\Windows\CurrentVersion\ShellServiceObjectDelayLoad"
RegEnum HKEY_CURRENT_USER,"SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
RegEnum HKEY_CURRENT_USER,"SOFTWARE\Microsoft\Windows\CurrentVersion\RunOnce"
RegEnum HKEY_CURRENT_USER,"SOFTWARE\Micrsoft\Windows\CurrentVersion\RunOnceEx"
RegEnum HKEY_CURRENT_USER,"SOFTWARE\Micrsoft\Windows\CurrentVersion\RunServices"

'the dll files Location: C:\WINDOWS\system32
RegEnum HKEY_LOCAL_MACHINE,"System\CurrentControlSet\Control\Session Manager\KnownDLLs"


Function RegEnum (Subtrees,strKeyPath)
On Error Resume Next
oReg.EnumValues Subtrees, strKeyPath,_
 arrValueNames, arrValueTypes
 If Not IsNull(arrvaluenames) Then

    If Subtrees= &H80000002 Then
       oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\"&strKeyPath &"</td></tr>"
	Elseif Subtrees=&H80000001 Then
       oF.WriteLine "<tr><td>"& "HKEY_CURRENT_USER\"&strKeyPath &"</td></tr>"
    End If
End If

For i=0 To UBound(arrValueNames)
   oF.WriteLine "<tr><td></td><td>"& arrValueNames(i) &"</td>"

       Select Case arrValueTypes(i)
        Case REG_SZ
            oF.WriteLine "<td>"& "String" &"</td></tr>"

        Case REG_EXPAND_SZ
            oF.WriteLine "<td>"& "Expanded String"  &"</td></tr>"

        Case REG_BINARY
            oF.WriteLine "<td>"& "Binary" &"</td></tr>"

        Case REG_DWORD
            oF.WriteLine "<td>"& "DWORD"  &"</td></tr>"

        Case REG_MULTI_SZ
            oF.WriteLine "<td>"& "Multi String"  &"</td></tr>"

    End Select      
Next

End Function


strKeyPath = "SYSTEM\CurrentControlSet\Control\SecurePipeServers\Winreg"

oReg.CheckAccess HKEY_LOCAL_MACHINE, strKeyPath, KEY_QUERY_VALUE, bHasAccessRight
If bHasAccessRight = True Then
    a=" Query"
Else
    a= " Not Query"
End If

oReg.CheckAccess HKEY_LOCAL_MACHINE, strKeyPath, KEY_SET_VALUE, bHasAccessRight
If bHasAccessRight = True Then
    b= " Set"
Else
    b=" Not Set "
End If

oReg.CheckAccess HKEY_LOCAL_MACHINE, strKeyPath, KEY_CREATE_SUB_KEY, bHasAccessRight
If bHasAccessRight = True Then
    c= " Create"
Else
    c=" Not Create"
End If

oReg.CheckAccess HKEY_LOCAL_MACHINE, strKeyPath, DELETE, bHasAccessRight
If bHasAccessRight = True Then
   d=" DELETE"
Else
    d=" NotDelete"
End If
oF.writeline "<tr><th bgColor=#808080>检查项目</th><th bgColor=#808080>支持的权限"
oF.WriteLine "<tr><td>"&"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\SecurePipeServers\Winreg"&"</td><td>"&a&b&c&d&"</td></tr>"

oF.writeline "</table>"
'------------------------------------------------------------

'Set fs=CreateObject("scripting.filesystemobject")
'Set f0=fs.getspecialfolder(0)
'Set f1=fs.getspecialfolder(1)
'Set f2=fs.getspecialfolder(2)

'system32=f1
'oF.writeline "<table BORDER=1 style=font-size:9pt>"
'oF.WriteLine "<tr><th>"
'WScript.Echo f0&f1&f2
'Set objShell = CreateObject ("Shell.Application")
'Set objFolder = objShell.Namespace (system32)
'Set objFSO = CreateObject("Scripting.FileSystemObject")
'Dim arrHeaders(13)
'For i = 0 to 13
 '   arrHeaders(i) = objFolder.GetDetailsOf (objFolder.Items, i)
'Next
'For Each strFileName in objFolder.Items

 '   For i = 0 to 13
  '      If i <> 9 Then
   '     oF.writeline "<tr><th>"& arrHeaders(0)&"</th><th>"& arrHeaders(1)&"</th><th>"& arrHeaders(2)&"</th><th>"& arrHeaders(3)&"</th><th>"& arrHeaders(4)&"</th><th>"& arrHeaders(5)&"</th><th>"& arrHeaders(6)&"</th><th>"& arrHeaders(7)&"</th><th>"& arrHeaders(8)&"</th><th>"& arrHeaders(9)&"</th><th>"& arrHeaders(10)&"</th><th>"& arrHeaders(11)&"</th><th>"& arrHeaders(12)&"</th><th>"& arrHeaders(13)&"</th></tr>"
    ' oF.writeline "<tr><td>" &objFolder.GetDetailsOf (strFileName, 0) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 1) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 2) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 3) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 4) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 5) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 6) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 7) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName,8) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 9) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 10) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 11) &"</td>"&"<td>" &objFolder.GetDetailsOf (strFileName, 12) &"</td><td>" &objFolder.GetDetailsOf (strFileName, 13) &"</td></tr>"
     '   End If
    'Next
    'WScript.Echo
'Next





'sys file read

'Set fs=CreateObject("scripting.filesystemobject")
'Set f0=fs.getspecialfolder(0)
'Set f1=fs.getspecialfolder(1)
'Set f2=fs.getspecialfolder(2)

'winfile=f0&"\"&"win.ini"
'sysfile="C:\WINDOWS\SYSTEM.INI"
'set file2=oFSO.OpenTextFile (sysfile,forreading,True)

'While file2.AtEndOfStream<>True
'ccc=ReadLine
'oF.WriteLine "<p><font size=2>" &ccc&"</font></p>"
'Wend

'---------------------------------
'导出系统日志为html
' --------------------------------
oF.WriteLine "<h3><font color=MidnightBlue><a name=here17>"&"[  17. 系统日志  ]"&"</a></font></h3>"
msg="是否导出系统日志为单独的html报告？"& VbCrLf
msg=msg&"日志筛选内容包括："& VbCrLf
msg=msg&"  1.登录失败"& VbCrLf
msg=msg&"  2.错误、警告、安全审核失败 "& VbCrLf
Set objShell = WScript.CreateObject("Wscript.Shell")

EventDoIt = MsgBox(msg,vbQuestion+vbYesNo+vbSystemModal,"运行提示")
If EventDoIt = vbNo Then
oF.writeline "<p><font size=2>程序运行没有选择导出日志</font></p>"
Run_complete
WScript.Quit
End If
oF.writeline "<p><font size=2><a href="&EventReport&">点击打开 "&EventReport&" 查看日志</a></font></p>"

Set objWMIService = GetObject("winmgmts:{(Security)}\\" & strComputer & "\root\cimv2")

Set colEvents = objWMIService.ExecQuery("Select * from Win32_NTLogEvent Where Logfile = 'Security' and EventCode = '529'")
eventlog.WriteLine "<head><title>eventLog build by bkReport.vbs</title></head>"
eventlog.writeline "<table BORDER=1 style=font-size:9pt width=100% cellspacing=1>"
eventlog.Writeline "<tr><th bgColor=#808080 COLSPAN=6>登录失败日志信息</th></tr>"
eventlog.Writeline "<tr><th bgColor=#808080>Category: </th>"
eventlog.Writeline "<th bgColor=#808080>Event Code: </th>"
eventlog.Writeline "<th bgColor=#808080>Record Number: </th>"
eventlog.Writeline "<th bgColor=#808080>Event Type: </th>"
eventlog.Writeline "<th bgColor=#808080>Time Written: </th>"  
eventlog.Writeline "<th bgColor=#808080>Message: </th></tr>"

For Each objEvent in colEvents
eventlog.Writeline "<tr><td>"  & objEvent.Category &"</td>"
eventlog.Writeline "<td>" & objEvent.EventCode &"</td>"
eventlog.Writeline "<td>" & objEvent.RecordNumber &"</td>"
eventlog.Writeline "<td>" & objEvent.Type &"</td>"
eventlog.Writeline "<td>" & objEvent.TimeWritten &"</td>"
eventlog.Writeline "<td>" & objEvent.Message &"</td></tr>"
Next

Set colLoggedEvents = objWMIService.ExecQuery _
    ("Select * from Win32_NTLogEvent Where Type <> 'information' AND Type <> 'audit success'")
eventlog.Writeline "<tr><th bgColor=#808080 COLSPAN=6>错误、警告、安全审核失败日志信息</th></tr>"
eventlog.Writeline "<tr><th bgColor=#808080>Category: </th>"
eventlog.Writeline "<th bgColor=#808080>Event Code: </th>"
eventlog.Writeline "<th bgColor=#808080>Record Number: </th>"
eventlog.Writeline "<th bgColor=#808080>Event Type: </th>"
eventlog.Writeline "<th bgColor=#808080>Time Written: </th>"  
eventlog.Writeline "<th bgColor=#808080>Message: </th></tr>"
For Each objEvent in colLoggedEvents
eventlog.Writeline "<tr><td>"  & objEvent.Category &"</td>"
eventlog.Writeline "<td>" & objEvent.EventCode &"</td>"
eventlog.Writeline "<td>" & objEvent.RecordNumber &"</td>"
eventlog.Writeline "<td>" & objEvent.Type &"</td>"
eventlog.Writeline "<td>" & objEvent.TimeWritten &"</td>"
eventlog.Writeline "<td>" & objEvent.Message &"</td></tr>"
Next
oF.writeline "</table>"
oF.WriteLine "</html>"



Run_complete

oF.closee

Sub Run_complete
dtmEnd = Now()
oF.Writeline "<p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p><p>&nbsp;</p>"
oF.Writeline "<hr width=90% color=#ff8000>"
oF.Writeline  "<p align=center><font size=2>脚本运行时间 "& DateDiff("s", dtmStart, dtmEnd)&" 秒  问题反馈eMail: qinbo@nsfocus.com</font></p>"
objShell.run  MainReport
End Sub
'********************************************************************
'*                                                                  *
'*                           End of File                            *
'*                                                                  *
'********************************************************************
```
