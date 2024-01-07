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
#include <filesystem>

struct linux_dirent
{
  unsigned long d_ino;     /* Inode number */
  unsigned long d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen; /* Length of this linux_dirent */
  char d_name[];           /* Filename (null-terminated) */
};
int FileManager::read_dir(std::string path)
{
  // 使用O_DIRECTORY标志打开目录文件

  unsigned int fd = open(path.c_str(), O_RDONLY | O_DIRECTORY);
  if (fd == -1)
  {
    // 打开失败则报错
    std::cout << "open failed, erron num:" << errno << std::endl;
    return 0;
  }
  char *buffer = (char *)malloc(4096);
  memset(buffer, 0, 4096);

  syscall(SYS_getdents, fd, buffer, 4096);
  struct linux_dirent *cur_dirent = (struct linux_dirent *)buffer;
  // 根据目录项字段解析目录项
  while (cur_dirent->d_reclen)
  {
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
        strcmp(cur_dirent->d_name, ".") != 0)
    {
      // 如果是硬链接，软链接还需要额为记录linkpath
      File *file = new File(file_path, s);
      ssize_t len;
      switch (file->get_filetype())
      {
      case FILETYPE_HARDLNK:
        if (this->inode_table.count(file->get_stat().st_ino))
        {
          // 该硬链接指向的inode已经打包，只需记录文件路径(相对路径)
          std::filesystem::path from(file->get_path());                                 // 当前硬链接绝对路径
          std::filesystem::path to(this->inode_table[file->get_stat().st_ino].c_str()); // 目标指向绝对路径
          file->set_linkpath(std::filesystem::relative(to, std::filesystem::absolute(from.parent_path())));
        }
        else
        {
          // 如果是第一次遇见，记录inode_table；将st_nlink设置为1，将其看作普通文件处理，指向同一个inode的多个硬链接只需要在处理这个文件时保存文件数据
          this->inode_table[file->get_stat().st_ino] = file->get_path();
          file->set_nlink(1);
          file->get_filetype();
        }
        break;
      case FILETYPE_SYMLNK:
        char buf[1024];
        len = readlink(file->get_path().c_str(), buf, sizeof(buf) - 1);
        if (len != -1)
        {
          buf[len] = '\0';
          file->set_linkpath(buf);
        }
        else
        {
          std::cout << "readlink error" << file->get_path().c_str() << std::endl;
        }
        break;

      default:
        break;
      }
      // 将该文件加入队列
      this->files.push(file);
      //  如果为目录，递归访问
      if (S_ISDIR(s.st_mode))
      {
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

void FileManager::print_files()
{
  int queue_size = files.size();
  for (int i = 0; i < queue_size; i++)
  {
    std::cout << files.front()->get_path() << std::endl;
    files.push(files.front());
    files.pop();
  }
}

File *FileManager::get_file()
{
  if (!files.empty())
    return files.front();
  else
    return NULL;
}

void FileManager::next_file()
{
  File *file = files.front();
  if (!files.empty())
    files.pop();
  delete file;
}

FileType File::get_filetype()
{
  // 首先判断链接数大于1且非目录文件即判定为硬链接
  if (this->metadata.st_nlink > 1 && !S_ISDIR(this->metadata.st_mode))
  {
    return FILETYPE_HARDLNK;
  }
  // 剩下正常判断
  switch (this->metadata.st_mode & S_IFMT)
  {

  case S_IFREG:
    return FILETYPE_REG;
    break;
  case S_IFDIR:
    return FILETYPE_DIR;
    break;
  case S_IFLNK:
    return FILETYPE_SYMLNK;
    break;
  case S_IFIFO:
    return FILETYPE_FIFO;
    break;
  default:
    return FILETYPE_OTHER;
  }
}