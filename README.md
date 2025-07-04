# 编码方式

ASCII字符是只有符合和字母  GBK是国标

Unicode只是一个编码表，表示对应方法。具体编码方式有`UTF-8 UTF-16le  UTF-16be`

## 指定编码格式

编译的时候，可以指定程序以什么方式保存`-finput-charset=UTF-8`,如果没有指定，默认使用UTF-8

编译出来的可执行程序，也可以指定以什么字符编码，`-fexec-charset=UTF-8`,如果没有指定，默认使用UTF-8

 

# `adb`命令

```shell
adb devices
adb push 1.txt /root # 把Ubuntu的文件放到开发板的/root目录
adb pull /root/1.txt 2.tx # 把开发板的/root/1.txt下载并改名为2.txt
adb shell # 登录开发板
```



# 常用命令

实验过程中LCD过几分钟就黑屏，这是正常的。
可以关闭黑屏的功能，执行以下命令即可：

```shell
echo -e "\033[9;0]" > /dev/tty0
```



框架 驱动层 中间层 app

全局链表链接多个设备，如何添加(链表‘=’赋值的含义)，如何选中

extern用法,声明其他文件的函数

管理多个输入设备



# GDB调试

![image-20250618163516927](https://markdownpic-1329280348.cos.ap-nanjing.myqcloud.com/image-20250618163516927.png)

需要在板子上启动`ulimit -c unlimited`命令来开启打印core，然后会在当前目录打印出来core

然后在ubuntu输入`adb pull /root/core . `把core拷贝到当前目录

输入`arm-buildroot-linux-gnueabihf-gdb ./test core`命令可以分析core文件

