#include "PackerManager.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "FileManager.h"
#include "colorful.h"
struct FileHeader
{
  char sub_path[1024];
  struct stat s;
  char linkpath[512];
};
FileType gettype(struct FileHeader fileheader)
{
  // 首先判断链接数大于1且非目录文件即判定为硬链接
  if (fileheader.s.st_nlink > 1 && !S_ISDIR(fileheader.s.st_mode))
  {
    return FILETYPE_HARDLNK;
  }
  // 剩下正常判断
  switch (fileheader.s.st_mode & S_IFMT)
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

void Packer::pack()
{
  File *file;
  while ((file = fm->get_file()) != NULL)
  {
    bool whs, wds;
    switch (file->get_filetype())
    {
    case FILETYPE_REG:
      whs = write_header(file);
      wds = write_data(file);
      BLUE_WITHOUTENTER("FILE|");
      std::cout << file->get_path();
      if (whs && wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_DIR:
      whs = write_header(file);
      BLUE_WITHOUTENTER("DIR|");
      std::cout << file->get_path();
      if (whs)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_HARDLNK:
      whs = write_header(file);
      BLUE_WITHOUTENTER("HARDLINK");
      std::cout << file->get_path();
      if (whs)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_SYMLNK:
      whs = write_header(file);
      BLUE_WITHOUTENTER("SYMLINK");
      std::cout << file->get_path();
      if (whs)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_FIFO:
      whs = write_header(file);
      BLUE_WITHOUTENTER("FIFO");
      std::cout << file->get_path();
      if (whs)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    default:
      break;
    }

    fm->next_file();
  }
}
bool Packer::write_header(File *file)
{
  FileHeader header;
  header.s = file->get_stat();
  std::string path = file->get_path();
  int pos = this->from.size();
  std::string sub_path = path.substr(pos, path.size() - pos);
  memcpy(header.sub_path, sub_path.c_str(), sub_path.size() + 1);
  memcpy(header.linkpath, file->get_linkpath().c_str(),
         file->get_linkpath().size() + 1);
  if (write(this->fd, &header, sizeof(FileHeader)) == sizeof(FileHeader))
    return true;
  else
    return false;
}
bool Packer::write_data(File *file)
{
  char *buffer = (char *)malloc(BUFFERSIZE);
  int bak_fd = open(file->get_path().c_str(), O_RDONLY);
  ssize_t n;
  ssize_t cur_size = 0;
  ssize_t file_size = file->get_stat().st_size;
  while ((n = read(bak_fd, buffer, BUFFERSIZE)) > 0)
  {
    ssize_t write_size = write(this->fd, buffer, (size_t)n);

    cur_size += write_size;
  }
  close(bak_fd);
  free(buffer);
  if (cur_size == file_size)
    return true;
  else
    return false;
}

bool UnPacker::read_header(char *buffer)
{
  if (read(this->fd, buffer, sizeof(FileHeader)) == sizeof(FileHeader))
    return true;
  else
    return false;
}

bool UnPacker::read_data(std::string targer_path, int size)
{
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = open(targer_path.c_str(), O_WRONLY | O_CREAT, mode);
  char *buffer = (char *)malloc(BUFFERSIZE);
  ssize_t remain_size = size;
  ssize_t write_size = 0;
  int n;
  while (remain_size > 0)
  {
    if (remain_size > BUFFERSIZE)
    {
      read(this->fd, buffer, BUFFERSIZE);
      remain_size -= BUFFERSIZE;
      ssize_t n = write(fd, buffer, (size_t)BUFFERSIZE);
      write_size += n;
    }
    else
    {
      read(this->fd, buffer, remain_size);
      ssize_t n = write(fd, buffer, (size_t)remain_size);
      remain_size -= remain_size;
      write_size += n;
    }
  }
  free(buffer);
  close(fd);
  // std::cout << write_size << " " << size;
  if (write_size == size)
    return true;
  else
    return false;
}
void UnPacker::unpack()
{
  FileHeader fileheader;
  while (read_header((char *)&fileheader))
  {
    std::string sub_path = fileheader.sub_path;
    std::string target_path = this->to + sub_path;
    struct stat s = fileheader.s;
    bool wds = false;
    mode_t mode;
    int pos;
    std::string real_linkpath;

    switch (gettype(fileheader))
    {
    case FILETYPE_REG:
      wds = this->read_data(target_path, s.st_size);
      BLUE_WITHOUTENTER("FILE|");
      std::cout << target_path;
      if (wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_DIR:
      mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH |
             S_IXUSR | S_IXGRP | S_IXOTH;
      if (mkdir(target_path.c_str(), mode) == 0)
        wds = true;
      BLUE_WITHOUTENTER("DIR|");
      std::cout << target_path;
      if (wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_HARDLNK:
      // linkpath是原文件和当前文件的相对地址，而real_linkpath是以当前工作目录做参照，原文件的相对地址
      pos = target_path.find_last_of('/');
      real_linkpath.assign(target_path.substr(0, pos + 1) +
                           fileheader.linkpath);
      if (link(real_linkpath.c_str(), target_path.c_str()) == 0)
        wds = true;
      BLUE_WITHOUTENTER("HARDLINK|");
      std::cout << target_path;
      if (wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_SYMLNK:
      if (symlink(fileheader.linkpath, target_path.c_str()) == 0)
        wds = true;
      BLUE_WITHOUTENTER("SYMLINK|");
      std::cout << target_path;
      if (wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    case FILETYPE_FIFO:
      mode = 0666;
      if (mkfifo(target_path.c_str(), mode) == 0)
        wds = true;
      BLUE_WITHOUTENTER("FIFO|");
      std::cout << target_path;
      if (wds)
        GREEN("|TRUE");
      else
        RED("|FALSE");
      break;

    default:
      break;
    }
  }
}