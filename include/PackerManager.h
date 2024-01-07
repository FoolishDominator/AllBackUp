#ifndef _PACKER_MANAGER_H_
#define _PACKER_MANAGER_H_
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "FileManager.h"

#define BUFFERSIZE 4096
class Packer {
 private:
  FileManager *fm;
  std::string from;
  std::string to;
  std::string bak_name;
  int fd;
  bool write_header(File *file);
  bool write_data(File *file);

 public:
  Packer(std::string from, std::string to, std::string bak_name,
         FileManager *fm)
      : from(from), to(to), fm(fm), bak_name(bak_name) {
    // 新建一个文件用于打包
    std::string file_path = to + bak_name + ".bak";
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    fd = open(file_path.c_str(), O_CREAT | O_RDWR | O_TRUNC, mode);
    // fm进行初始化
  };
  ~Packer() { close(fd); };
  void pack();
};

class UnPacker {
 private:
  std::string bak_path;
  std::string to;
  int fd;
  bool read_header(char *buffer);
  bool read_data(std::string targetpath, int size);

 public:
  UnPacker(std::string bak_path, std::string to) : to(to), bak_path(bak_path) {
    fd = open(bak_path.c_str(), O_RDWR);
    // fm进行初始化
  };
  ~UnPacker() { close(fd); };
  void unpack();
};
#endif