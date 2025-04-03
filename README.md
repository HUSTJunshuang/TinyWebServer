# TinyWebServer

Personal PBL(Project-Based-Learning) repo, the source project is [qinguoyi/TinyWebServer](https://github.com/qinguoyi/TinyWebServer), which is licensed under the Apache License, Version 2.0.

## 项目亮点（个人贡献）

### 项目构建

#### 引入Kconfig配置系统

Kconfig官方文档：[Kconfig Language — The Linux Kernel documentation](https://www.kernel.org/doc/html/latest/kbuild/kconfig-language.html)

本来想基于Linux kernel原生的Kconfig系统进行改造，但目前精力有限且项目配置并不复杂，退而求其次，采用接近原生Kconfig系统的kconfig-frontends，但kconfig-frontends功能较简单，无法自动生成autoconf.h等文件，额外引入kconfiglib来实现类似功能（Tips：kconfiglib本身也能实现图形化menuconfig）。

配置方式如下：

```shell
# 安装kconfig-frontends
sudo apt install kconfig-frontends
# 安装kconfiglib，需要python环境
pip install kconfiglib
```

TODO：暂不支持fixdep（虽然小项目页没必要）。
