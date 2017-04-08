---
layout : life
title: mysql中create_database和create_schema的区别
category : 数据库学习
duoshuo: true
date : 2015-07-27
---

<!-- more -->

>
  在使用MS SQL的时候，一般创建数据库我们都习惯于使用CREATE DATABASE 来完成，而使用CREATE SCHEMA来创建架构，但是在MySQL中，官方的中文文档在 CREATE DATABASE 语法一节中写了一句：也可以使用CREATE SCHEMA。那么CREATE SCHEMA和CREATE DATABASE在MySQL难道是一样的吗？

  MySQL 5.0官方的英文文档中有这么一句：

```sql
CREATE DATABASE creates a database with the given name.
To use this statement, you need the CREATE privilege for the database.
CREATE SCHEMA is a synonym for CREATE DATABASE as of MySQL 5.0.2.
```

   这个说法译成中文应该是：CREATE　DATABASE根据给定的名称创建数据库，要用这个语法，你需要有数据库的CREATE权限，CREATE SCHEMA从MySQL5.0.2起，可作为CREATE DATABASE的一个代名词。

   按照这个说法，CREATE SCHEMA是和CREATE DATABASE是一样的，为了验证这个说法，参阅了后继的官方文档，在MySQL5.5的英文文档中，官方如是说：

```sql
CREATE DATABASE creates a database with the given name.
To use this statement, you need the CREATE privilege for the database.
CREATE SCHEMA is a synonym for CREATE DATABASE.
```

   中文：CREATE DATABASE根据给定的名称创建数据库，你需要拥有数据库的CREATE权限来使用这个语句。CREATE SCHEMA是CREATE DATABASE的一个代名词。

  由此可见，在MySQL的语法操作中（MySQL5.0.2之后），可以使用CREATE DATABASE和CREATE SCHEMA来创建数据库，两者在功能上是一致的。在使用MySQL官方的MySQL管理工具MySQL Workbench 5.2.47创建数据库时，使用的是CREATE SCHEMA来创建数据库的。而这和MS SQL中的SCHEMA有很大差别。
