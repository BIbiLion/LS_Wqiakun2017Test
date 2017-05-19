首先需要安装qtxlslwriter 这个库，下载地址：

https://github.com/leolin0518/QtXlsxWriter

Note: Perl is needed in this step.

打开qt的命令行工具，进入相应的.pro目录，执行一下命令进行安装：

    1.qmake(生成makefile文件)

    2.mingw32-make(编译源码)

    3.mingw32-make install(编译安装)

至此，若不出错，则顺利安装成功！


文档：http://qtxlsx.debao.me/


参考文档：

Qt操作Excel文件 QtXlsxWriter的配置使用说明

Qt下Excel报表生成的又一利器----QtXlsxWriter

http://www.cnblogs.com/lvdongjie/p/4402294.html

C++读写EXCEL文件方式比较

在Qt中用QAxObject来操作Excel



