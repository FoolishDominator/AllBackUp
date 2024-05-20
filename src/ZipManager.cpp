#include "ZipManager.h"

#include <zlib.h>

void Zip::zip() {
  char input[1024];
  uLong llen;
  while ((llen = read(from_fd, input, 1024)) > 0) {
    uLong blen = compressBound(llen);
    char* output = (char*)malloc(blen);
    compress((Bytef*)output, &blen, (Bytef*)input, llen);
    write(to_fd, &llen, sizeof(llen));
    write(to_fd, &blen, sizeof(blen));
    write(to_fd, output, (size_t)blen);
    free(output);
  }
}
void Zip::unzip() {
  char input[1024];
  char output[2048];
  uLong blen;
  uLong llen;
  while (1) {
    if (read(from_fd, &llen, sizeof(llen)) <= 0) break;
    read(from_fd, &blen, sizeof(blen));
    read(from_fd, input, blen);
    uncompress((Bytef*)output, &llen, (Bytef*)input, blen);
    write(to_fd, output, (size_t)llen);
  }
}