# FiveInARow 五子棋程序

## 简介

本软件为基于Visual C++、Python等语言，采用了**MFC**、ffmpeg、Flask等技术的**多人在线**/离线五子棋对战实例。

## 已实现的功能

- [x] 基础功能

- [x] 落子声音

- [x] 音视频录制

- [x] 联网对战

- [x] 在线存档

- [x] 复盘

- [x] 双方单独记时

## 编译与配置

  ### 目录结构

```
-src
	-Client
		-res				#项目资源
		-Bridge.py			#沟通桥
		-FiveInARow.sln		#MFC主程序
		-...
	-Server
		-Server.py			#服务器程序
-dist
	-Client
		-Bridge.exe			#沟通桥可执行文件
		-config.ini			#配置文件
		-ffmpeg.exe			#录屏与解码工具
		-FiveInARow.exe		#主程序
	-Server
		-Server.exe			#服务器程序
	-present
		-Presentation.pptx	#项目介绍（ppt格式）
		-Presentation.mp4	#项目介绍（视频）
		-…					
```

### 编译方法

本包中```/dist```目录下是已预编译的可执行文件。如果希望重新编译，可以删除该文件夹，并遵循以下步骤：

1.确保已正确安装Visual Studio 2022、Python 3

2.在程序根目录下右键，选择“在终端中打开”；

3.执行：

```bash
mkdir ./dist
mkdir ./dist/Client
mkdir ./dist/Server
pip install pyinstaller
pyinstaller --onefile ./src/Client/Bridge.py
move ./src/Client/dist/Bridge.exe ./dist/Client/Bridge.exe
pyinstaller --onefile ./src/Server/Server.py
move ./src/Server/Server.exe ./dist/Server/Server.exe
move ./src/Client/ffmpeg.exe ./dist/Client/ffmpeg.exe
echo "[Local]" >> ./src/Client/config.ini
echo "Sound=true" >> ./src/Client/config.ini
echo "Record=false" >> ./src/Client/config.ini
echo "Focus=false" >> ./src/Client/config.ini
echo "[PVP]" >> ./src/Client/config.ini
echo "Enable=false" >> ./src/Client/config.ini
```

4.打开```./src/Client/FiveInARow.sln```,按**F5**编译

5.将```./src/Client/x64/Release/FiveInARow.exe```复制到```./dist/Client/```路径下

即可得到本包附带的预编译的可执行文件。

## 运行方法

双击```./dist/Client/FiveInARow.exe```即可运行程序。

## 配置方法

用户可在```config.ini```中配置本软件的功能。对该配置文件的具体说明如下：

```ini
[Local]
Sound=true			#是否在落子时播放声音
Record=true			#是否在对局时录制音视频
Focus=true			#是否在对局时始终将窗口置于最顶层
SaveLog=true		#是否在对局时保存对局记录
Replay=false		#是否开启复盘模式
Logfile=Replay.log	#对局记录文件名称

[PVP]
Enable=false		#是否启用多人联机对局
URL=http://127.0.0.1:1234	#服务器URL及端口号
ID=1				#对局者的ID（1或2）
```

注1：出于性能和流畅度方面的考虑，不建议同时启用联机和录制功能。

注2：联网对局时程序只会记录己方用时。

注3：保存记录和复盘模式不能同时开启。

### 服务器配置方法

#### 运行方法

双击```Server.exe```启动

抄录命令行窗口中显示的网址并填写至客户端的```config.ini```中

#### 查看在线对局记录方法

打开服务器网址，在网页上显示的列表中选择对应的文件



## 疑难解答

**Q1：无法录屏**

A1：检查ffmpeg.exe是否缺失、电脑麦克风是否就绪

**Q2：显示”网络错误“**

A2：检查服务器是否就绪、检查防火墙、检查网络连接。

​		 若仍未解决尝试执行```pip install requests```

**Q3：服务器无法启动**

A3：检查防火墙和网络连接。如仍无法启动，尝试```pip install flask```

