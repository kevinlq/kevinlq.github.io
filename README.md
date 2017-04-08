## 简介

[![Join the chat at https://gitter.im/daodaoliang/daodaoliang.github.com](https://badges.gitter.im/daodaoliang/daodaoliang.github.com.svg)](https://gitter.im/daodaoliang/daodaoliang.github.com?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

这是一款Jekyll博客主题，基础模板采用的是该**Source: [GitHub](https://github.com/pexcn/Jekyll-Light)**下的模板(感谢作者分享),里面我也做了一些个人喜好的修改，若是你喜欢该模板，请在下载后清除我的博文和我的配置信息，谢谢。PS：Enjoy it :）

## 使用

### 站点配置
在`_config.yml`配置你的站点信息，详细配置如下：

	blog:
		name:                  # 博客名称
		description:           # 博客描述
		title:                 # 网页标题
		url:                   # 博客地址
		duoshuo:               # 多说ID
		tongji:                # 百度统计ID
		qiniu:                 # 七牛云地址
	author:
		name:                  # 作者名称
		email:                 # 邮箱地址
		weibo:                 # 微博地址
		github:                # GitHub地址
		logo:                  # logo地址
把以上这些选项的信息全改成你自己的，就配置完成了。

### 多说评论框
_posts文章默认开启评论框，而简版页面默认关闭。  
_posts文章可以在开头设置`duoshuo: false`来关闭。  
简版页面可以在开头设置`duoshuo: true`来开启。

### MathJax数学公式
需要在页面开头添加`math: true`来开启

在需要用到公式的地方用`\\ \\`或`$$ $$`括起来

例如：

	行内公式：
	\\E=mc^2\\

	行间公式：
	$$E=mc^2$$

### 创建文章/页面
定位到博客目录，可以运行以下命令

* 创建文章：`rake post title="Post Name"` 
* 创建简版页面：`rake life title="Page Name"`
* 创建页面： `rake page title="Page name"`

### 页面的使用
修改的都是`markdown文件`

* 普通页面  
layout项改为`blog`
* 简版页面  
layout项改为`life`
* 文章  
_posts文件夹下的markdown文件的layout项改为`post`，使用简版页面就改成`life`

### 生成静态博客
把你的博客推送到GitHub或者其它支持Jekyll的代码托管网站就可以了。  
具体可以到[Jekyll官网](http://jekyllrb.com/)或[GitHub Pages](https://pages.github.com/)查看详细教程。


## 许可证

[**GNU GPL v3**](http://www.gnu.org/licenses/gpl-3.0.html)
