# AllBackUp
文件备份软件
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