# AllBackUp
文件备份软件
## 服务器版本
Ubuntu 22.04.1
## 安装方法
将软件安装进系统目录
```shell
mkdir build
cd build
cmake ..
make
sudo make install
```
## 卸载方法
将软件从系统目录中删除：

在build目录下
```shell
sudo xargs rm < install_manifest.txt
```
## docker
切换到项目根目录,构建镜像
```shell
sudo docker build -t allbackup .
```
启动容器
```shell
sudo docker run -it --name backuptest allbackup /bin/bash
```
## 使用说明
安装完成后键入`allbackup -h`查看详细说明