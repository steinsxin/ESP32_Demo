# ESP32_Demo

ESP32 示例工程，适用于 **ESP-32**开发板

## 📁 项目结构说明

此工程基于 **ESP32** 开发板，演示了Clion下的环境配置和调试

## 🛠️ 开发环境要求

- 开发板：ESP-WROOM-32
- 开发环境：Clion 2025.1 / ESP-IDF-5.3.3 / GCC 工具链

环境配置

1.安装ESP-IDF

​	[ESP-IDF](https://dl.espressif.com/dl/esp-idf/ ) 下载链接 ，安装过程建议科学上网，安装路径不要带有中文

注意事项

如果出现 **ERROR: Failed to check the tool while installed**

一般为用户名存在中文

解决方法如下：

进入设置中的更改时区-> 选择语言和区域 -> 勾选：Beta:使用unicodeUTF-8 -> 重新安装成功

2.配置Clion

修改自动配置文件`idf_cmd_init.bat`

编辑，首行添加，根据本地路径进行修改

```
:: set IDF_PATH
set IDF_PATH=E:\Espressif\frameworks\esp-idf-v5.3.3
```

工具链配置如下：

![image-20250511150416622](C:\Users\小新\AppData\Roaming\Typora\typora-user-images\image-20250511150416622.png)

配置好环境后，选择”flash“，点击锤子🔨进行编译

![屏幕截图 2025-05-11 144608](C:\Users\小新\Desktop\屏幕截图 2025-05-11 144608.png)

这里会出现设置 ESPPORT 和 ESPBAUD的提示，可选设置，不设置也会自行选择 

## Clion monitor

想要选择monitor去监测板子，会出现

```
Note: python;-m;esp_idf_monitor will search for a serial port. To specify a port, set the ESPPORT environment variable.
Error: Monitor requires standard input to be attached to TTY. Try using a different terminal.
CMake Error at run_serial_tool.cmake:66 (message):
  python;-m;esp_idf_monitor failed.
```

替代解决办法如下：

方案一：

![image-20250511161234883](C:\Users\小新\AppData\Roaming\Typora\typora-user-images\image-20250511161234883.png)

方案二：

1.添加一个Python解释器

​	设置-> 构建、执行、部署 -> Python 解释器

2.添加一个Python脚本，如下：

![屏幕截图 2025-05-11 160654](C:\Users\小新\Desktop\屏幕截图 2025-05-11 160654.png)

注意：需要在修改选项中添加 "在输出控制台中模拟终端"，否则输出为乱码

