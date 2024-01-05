#include "FileManager.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

struct linux_dirent {
  unsigned long d_ino;     /* Inode number */
  unsigned long d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen; /* Length of this linux_dirent */
  char d_name[];           /* Filename (null-terminated) */
};
int FileManager::read_dir(std::string path) {
  // 使用O_DIRECTORY标志打开目录文件

  unsigned int fd = open(path.c_str(), O_RDONLY | O_DIRECTORY);
  if (fd == -1) {
    // 打开失败则报错
    std::cout << "open failed，erron num:" << errno << std::endl;
    return 0;
  }
  char *buffer = (char *)malloc(4096);
  memset(buffer, 0, 4096);

  syscall(SYS_getdents, fd, buffer, 4096);
  struct linux_dirent *cur_dirent = (struct linux_dirent *)buffer;
  // 根据目录项字段解析目录项
  while (cur_dirent->d_reclen) {
    // 拼接获得子文件的路径
    std::string file_name = cur_dirent->d_name;
    std::string file_path;
    if (path.back() != '/')
      file_path = path + "/" + file_name;
    else
      file_path = path + file_name;

    // 获取子文件的属性
    struct stat s;
    lstat(file_path.c_str(), &s);
    //.和..不作处理
    if (strcmp(cur_dirent->d_name, "..") != 0 &&
        strcmp(cur_dirent->d_name, ".") != 0) {
      // 递归访问目录
      File *file = new File(file_path, s);
      this->files.push(file);
      if (S_ISDIR(s.st_mode)) {
        this->read_dir(file_path);
      }
    }
    // 访问下一个目录项
    cur_dirent =
        (struct linux_dirent *)((char *)cur_dirent + cur_dirent->d_reclen);
  }
  free(buffer);
  return 1;
}

void FileManager::print_files() {
  int queue_size = files.size();
  for (int i = 0; i < queue_size; i++) {
    std::cout << files.front()->get_path() << std::endl;
    files.push(files.front());
    files.pop();
  }
}

File *FileManager::get_file() {
  if (!files.empty())
    return files.front();
  else
    return NULL;
}

void FileManager::next_file() {
  File *file = files.front();
  if (!files.empty()) files.pop();
  delete file;
}