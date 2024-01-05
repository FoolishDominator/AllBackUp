#ifndef _PACKER_H_
#define _PACKER_H_
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "FileManager.h"

#define BUFFERSIZE 4096
class Packer {
 private:
  FileManager* fm;
  std::string to;
  int fd;

 public:
  Packer(std::string to, FileManager* fm) : to(to), fm(fm) {
    // 新建一个文件用于打包
    std::string file_path = to + "/kita.bak";
    fd = open(file_path.c_str(), O_CREAT | O_RDWR, 777);
    // fm进行初始化
  };
  ~Packer() { close(fd); };
  void pack();
  void write_header(File* file);
  void write_data(File* file);
};

#endif