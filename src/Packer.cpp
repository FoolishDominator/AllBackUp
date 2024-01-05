#include "Packer.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "FileManager.h"
struct FileHeader {
  char sub_path[512];
  struct stat s;
};
void Packer::pack() {
  File* file;
  while ((file = fm->get_file()) != NULL) {
    if (!S_ISDIR(file->get_stat().st_mode)) {
      bool whs = write_header(file);
      bool wds = write_data(file);
      std::cout << "FILE|" << file->get_path() << "|"
                << ((whs && wds) ? "TRUE" : "FALSE") << std::endl;
    } else {
      bool whs = write_header(file);
      std::cout << "DIR|" << file->get_path() << "|"
                << ((whs) ? "TRUE" : "FALSE") << std::endl;
    }
    fm->next_file();
  }
}
bool Packer::write_header(File* file) {
  FileHeader header;
  header.s = file->get_stat();
  std::string path = file->get_path();
  int pos = this->from.size();
  std::string sub_path = path.substr(pos, path.size() - pos);
  memcpy(header.sub_path, sub_path.c_str(), sub_path.size() + 1);
  if (write(this->fd, &header, sizeof(FileHeader)) == sizeof(FileHeader))
    return true;
  else
    return false;
}
bool Packer::write_data(File* file) {
  char* buffer = (char*)malloc(BUFFERSIZE);
  int bak_fd = open(file->get_path().c_str(), O_RDONLY);
  ssize_t n;
  ssize_t cur_size = 0;
  ssize_t file_size = file->get_stat().st_size;
  while ((n = read(bak_fd, buffer, BUFFERSIZE)) > 0) {
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
bool UnPacker::read_header(char* buffer) {
  if (read(this->fd, buffer, sizeof(FileHeader)) == sizeof(FileHeader))
    return true;
  else
    return false;
}

bool UnPacker::read_data(std::string targer_path, int size) {
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = open(targer_path.c_str(), O_WRONLY | O_CREAT, mode);
  char* buffer = (char*)malloc(BUFFERSIZE);
  ssize_t remain_size = size;
  ssize_t write_size = 0;
  int n;
  while (remain_size > 0) {
    if (remain_size > BUFFERSIZE) {
      read(this->fd, buffer, BUFFERSIZE);
      remain_size -= BUFFERSIZE;
      ssize_t n = write(fd, buffer, (size_t)BUFFERSIZE);
      write_size += n;
    } else {
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
void UnPacker::unpack() {
  FileHeader fileheader;
  while (read_header((char*)&fileheader)) {
    std::string sub_path = fileheader.sub_path;
    std::string target_path = this->to + sub_path;
    struct stat s = fileheader.s;
    if (!S_ISDIR(s.st_mode)) {
      bool wds = this->read_data(target_path, s.st_size);
      std::cout << "FILE|" << target_path << "|" << (wds ? "TRUE" : "FALSE")
                << std::endl;
    } else {
      mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH |
                    S_IXUSR | S_IXGRP | S_IXOTH;

      bool wds;
      if (mkdir(target_path.c_str(), mode) == 0) wds = true;
      std::cout << "DIR|" << target_path << "|" << (wds ? "TRUE" : "FALSE")
                << std::endl;
    }
  }
}