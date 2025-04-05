# TinyWebServer

Personal PBL(Project-Based-Learning) repo, the source project is [markparticle/WebServer](https://github.com/markparticle/WebServer), which is licensed under the Apache License, Version 2.0.

Kconfig、Makefile等部分的灵感主要来源于“一生一芯”学习经历，感谢一生一芯项目组提供的学习平台@[一生一芯](https://ysyx.oscc.cc/)。

## 项目亮点（个人贡献）

> 作者网络服务器相关知识还是本科时了解的，下文若有纰漏，欢迎大佬们批评指正。

### 项目构建

#### 目录结构

前端目录结构无统一标准，一般由团队协商约定。可以通过webpack进行打包目录结构的配置。

#### 引入Kconfig配置系统

Kconfig官方文档：[Kconfig Language — The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html)

本来想基于Linux kernel原生的Kconfig系统进行改造，但目前精力有限且项目配置并不复杂，退而求其次，采用接近原生Kconfig系统的kconfig-frontends，但kconfig-frontends功能较简单，无法自动生成autoconf.h等文件，额外引入kconfiglib来实现类似功能（Tips：kconfiglib本身也能实现图形化menuconfig）。

配置方式如下：

```bash
# 安装kconfig-frontends
sudo apt install kconfig-frontends
# 安装kconfiglib，需要python环境
pip install kconfiglib
```

> TODO：暂不支持fixdep（虽然小项目也没必要）。

#### 引入GoogleTest测试框架

> GoogleTest（也称为 gtest）是一个由 Google 开发的 C++ 测试框架，用于编写和运行单元测试。它提供了一组丰富的断言和测试功能，帮助开发者确保代码的正确性和稳定性。（作者太懒，AI生成，狗头保命）

[GoogleTest官方仓库](https://github.com/google/googletest.git)，小项目采用集成源码的方案，使用时如有报错可尝试自己重新生成静态库（保证使用的C++标准与整个项目的环境保持一致）。

Step1：获取源码，仅保留需要用到的两个文件夹

Step2：编译静态库

```bash
# 编译所有源文件并打包成静态库
g++ -std=c++17 -I./include -c src/gtest-all.cc -o gtest-all.o
ar crv libgtest.a gtest-all.o
# 编译并打包主程序入口（可选，本项目每个测试都自己写main函数，不链接该静态库）
g++ -std=c++17 -I./include -c src/gtest_main.cc -o gtest_main.o
ar crv libgtest_main.a gtest_main.o
# 编译gmock库，同上（可选）
# 移除除include目录以外的其他文件夹（可选）
```

### 代码优化

> TODOs：线程池与数据库分离

#### 前后端分离

Kconfig提供 `Static Resource Proxy`配置选项，如果配置了静态资源代理方法（如使用Nginx代理服务器），则后端服务器只需要处理动态请求，仅需与CGI进行交互（或其他动态请求处理方式，如FastCGI、WSGI等）。
