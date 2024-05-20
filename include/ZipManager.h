#ifndef _ZIP_MANAGER_H_
#define _ZIP_MANAGER_H_
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
class Zip {
 private:
  int from_fd;
  int to_fd;
  int mode;
  std::string unzip_path;

 public:
  void zip();
  void unzip();
  std::string get_unzip_path() { return this->unzip_path; }
  Zip(std::string from) {
    from_fd = open(from.c_str(), O_RDONLY);
    int pos = from.find_last_of('/');
    std::string file_name = from.substr(pos + 1, from.size() - pos - 1);
    int pos2 = file_name.find_last_of('.');
    std::string sub_file_name_pre = file_name.substr(0, pos2 + 1);
    std::string sub_file_name_tail =
        file_name.substr(pos2 + 1, file_name.size() - pos2 - 1);
    unzip_path =
        from.substr(0, pos + 1) + sub_file_name_pre + "z" + sub_file_name_tail;
    mode_t modet = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    to_fd = open(unzip_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, modet);
  }
  ~Zip() {
    close(from_fd);
    close(to_fd);
  };
};

#endif