#include "Packer.h"

#include <sys/stat.h>

#include "FileManager.h"
void Packer::pack() {
  File* file;
  while ((file = fm->get_file()) != NULL) {
    if (S_ISDIR(file->get_stat().st_mode)) {
      write_header(file);
      write_data(file);
    }
    fm->next_file();
  }
}
void Packer::write_header(File* file) {
  struct stat s = file->get_stat();
  write(this->fd, &s, sizeof(struct stat));
}
void Packer::write_data(File* file) {
  char* buffer = (char*)malloc(BUFFERSIZE);
  int bak_fd = open(file->get_path().c_str(), O_RDONLY);
  int n;
  while (n = read(bak_fd, buffer, BUFFERSIZE) > 0) {
    write(this->fd, buffer, n);
  }
  close(bak_fd);
}
