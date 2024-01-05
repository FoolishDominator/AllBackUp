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