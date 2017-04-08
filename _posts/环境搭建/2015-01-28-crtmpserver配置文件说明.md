---
layout : life
title: crtmpserver配置文件说明
category : 环境搭建
duoshuo: true
date : 2015-01-28
---

<!-- more -->

## ***配置文件说明***

配置文件实际上是一个Lua脚本，它包含至少一个configuration的对象，从而为程序提供灵活的扩展和定制功能。除了configuration对象外，还可以有函数，Lua库等。

### ***主结构***

```lua
configuration=
{
  daemon=false,
  pathSeparator="/",
  logAppenders=
  {
    -- content removed for clarity
  },
  applications=
  {
    -- content removed for clarity
  }
}
```

|key|type|mandatory|description|
|:------:|:------:|:-------:|:-------:|
|daemon|boolean|yes|true  表示 服务以后台方式启动;false 表示 服务以控制台模式启动(以用于开发);|
|pathSeparator|string(1)|yes|用来分隔路径; 例如，在UNIX-like是 /, Windows是\;|
|logAppenders|object|yes|配置日志追加的容器|
|applications|object|yes|配置加载各种应用的容器|

***服务启动时，将按顺序执行下列操作:***

* 配置文件加载后，首先做的就是对配置文件进行校验，如果配置文件有错误，将会有错误提示并停止启动，可进行修改后再启动;
* 读取 daemon 值，判断服务是以后台方式启动还是以控制台方式启动;
* 读取日志追加器，用来配置日志记录并启动到运行状态，依据日志追加器，可以看到更多的日志信息;
* 最后的应用加载，只到这一步完成后，服务和应用才在线，并准备就绪;

### ***日志追加器***

这部分包含了一个日志追加器的列表。整个日志追加器的添加是在加载时配置，依据日志级别，追加器可以选择是否有日志消息输出到指定目的处；

```lua
logAppenders=
{
  {
    name="console appender",
    type="coloredConsole",
    level=6
  },
  {
    name="file appender",
    type="file",
    level=6,
    fileName="/tmp/crtmpserver.log"
  }
},
```

***logAppenders structure***

|key|type|mandatory|description|
|:------:|:------:|:-------:|:-------:|
|name|string|yes|追加器的名字.|
|type|string|yes|追加器的类型可以是控制台，带颜色控制台或文件;控制台和带颜色控制台都会将日志消息输出到控制台，不同之处在于带颜色控制台会依据日志级别进行颜色标记；文件类型则会将所有消息输出到指定的文件；|
|level|number|yes|日志的级别可见下表中的级别定义；\n只有小于或等于这个级别的日志消息会被记录，高于这个级别则都被丢弃；例如:级别为0时，只记录 FATAL 消息；级别为3时，只记录 FATAL, ERROR, WARNING, INFO 消息；|
|fileName|string|yes|如果追加器类型为文件，则在此处指定日志文件和路径|

***Log levels***

|name|value|
|:------:|:------:|
|0|FATAL|
|1|ERROR|
|2|WARNING|
|3|INFO|
|4|DEBUG|
|5|FINE|
|6|FINEST|

***PS***: 当使用后台模式时，所有的控制台追加消息将会被忽略。

### ***应用***

这部分用来配置各种应用，并设置这些应用的属性；每个应用的属性都对应了这个应用的指定功能；

```lua
applications=
{
  rootDirectory="applications",
  {
    -- settings for application 1
    -- content removed for clarity
  },
  {
    -- settings for application 2
    -- content removed for clarity
  },
  {
    -- settings for application 3
    -- content removed for clarity
  }
}
```

***applications structure***

|key|type|mandatory|description|
|:------:|:------:|:-------:|:-------:|
|rootDirectory|string|true|这个目录包含了应用的子目录；如果路径以 / 或 \ 开始， 则视其为绝对路径，否则视为启动服务时所在的相对路径；|


rootDirectory 之后，是应用的集合；每个应用都定义了一个有特定属性的对象；细节如下所示；

***应用定义***

这些目录用来定义应用.自从790版本后，添加了一新的功能：mediaStorage; 这个功能能使应用可以有多个mediaFolder，并且可以将.seek/.meta文件和媒体文件分开存储在不同的文件夹中；

```lua
{
  name="flvplayback",
  protocol="dynamiclinklibrary",
  description="FLV Playback Sample",
  default=false,
  validateHandshake=true,
  enableCheckBandwidth=true,
  -- this settings are now part of mediaStorage setting
  -- keyframeSeek=true,
  -- seekGranularity=1.5,
  -- clientSideBuffer=12,
  -- generateMetaFiles=true,
  -- renameBadFiles=true,
  aliases=
  {
    "simpleLive",
    "vod",
    "live",
    "WeeklyQuest",
    "SOSample",
    "oflaDemo",
    "chat",
  },
  acceptors = 
  {
    {
      -- acceptor 1
      -- content removed for clarity
    },
    {
      -- acceptor 2
      -- content removed for clarity
    },
    {
      -- acceptor n
      -- content removed for clarity
    },
  },
  -- new feature mediaStorage
  mediaStorage = {
     namedStorage1={
      description="Main storage",
      mediaFolder="/usr/main_storage/media", -- only this parameter IS MANDATORY
      metaFolder="/usr/main_storage/metadata", -- if you have static large file to stream it is good to know that for a file around 500MB 
                                               -- it's .seek file has around 16MB; so it would be preffer to designate metafolder into a system
                                               -- partition which has enough space... for no surprises... :)
      statsFolder="/usr/main_storage/statsFolder",
      enableStats=true,
      clientSideBuffer=16,
      keyframeSeek=false, -- should crtmpdserver DO SEEK ONLY IN key-frame (true/false)? 
                          -- very useful to know in situations like play/pause/resume (meaning pause/seek/play)
      seekGranularity=1,
      generateMetaFiles=false,
      renameBadFiles=false,
    },
    --[[{
      -- here is another example of storage; it does not start with name={...}
       description="Second storage of same application",
       mediaFolder="/usr/second_storage/media/flv",
       metaFolder="/usr/second_storage/metadata",
       statsFolder="/usr/second_storage/statsFolder",
    },]]--
  }, 
  externalStreams = 
  {
    {
      -- stream 1
      -- content removed for clarity
    },
    {
      -- stream 2
      -- content removed for clarity
    },
    {
      -- stream n
      -- content removed for clarity
    },
  },
  authentication=
  {
    -- content removed for clarity
  }
}
```

***application structure***

|key|type|mandatory|description|
|:------:|:------:|:-------:|:-------:|
|name|string|yes|应用的名称|
|protocol|string|yes|应用的类型值为 dynamiclinklibrary 意即 应用是一个共享库|
|description|string|no|应用的描述信息|
|default|string|no|这个标志指定了默认应用；默认应用负责分析连接请求并将连接分配到正确的应用|
|validateHandshake|boolean|no|通知服务器在进行下一步前要对客户端的握手进行验证；这是一个可选项，其默认值为真。如果这个值为真 且 握手失败，服务器就放弃这个连接;如果这个值为假，则不会进行强制的握手验证，所有的连接都会被接受；|
|keyframeSeek|boolean|no|这个属性指定了流生成器只在关键帧搜索，如果是直播流，则忽略这个值|
|seekGranularity|double|no|搜索的精细度，以秒为单位, 值域定义在 0.1 ~ 600; 例如：如果粒度定义为10秒，并期望定位到 t= 2:34; 则实际上是会定位到 t= 2:30. 60秒被认定为完整的电影长度，1秒为电影片断；|
|clientSideBuffer|double|no|每个连接在客户端的缓冲秒数，值定义在5 ~ 30 秒；|
|generateMetaFiles|boolean|no|在应用启动前生成 seek/meta文件|
|renameBadFiles|boolean|no|如果这上值为真且媒体文件是不能被解析的，则媒体文件被重命名为 *.bad，否则这样的文件将不做处理|
|aliases|object|no|应用的别名|
|acceptors|object|no|接受器保持这个服务并让服务器托管；应用可以有它自己的接受器，但这个是可选的；|
|externalStreams|object|no||
|authentication|object|no||
|mediaFolder|string|yes|当定义了 mediaStorage时，这个域用来指定媒体文件的物理位置；|
|metaFolder|string|no|指定用来存放 .seek/.meta文件的位置；|
|statsFolder|string|no|服状态文件的位置|

***acceptor structure***

|key|type|mandatory|description|
|:------:|:------:|:-------:|:-------:|
|ip|string|yes|服务所在的IP, 0.0.0.0表示所有接口和所有IP; |
|port|string|yes|服务监听的端口号|
|protocol|string|yes|对应 ip:port的服务的协议|
