#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include <sys/stat.h>

#include <iostream>
#include <queue>
#include <string>
class FileHeader {};
class File {
 private:
  std::string path;
  struct stat metadata;

 public:
  File(std::string path, struct stat s) : path(path), metadata(s){};
  ~File(){};
  std::string get_path() { return path; }
  struct stat get_stat() { return metadata; }
};

class FileManager {
 private:
  std::queue<File*> files;

 public:
  int read_dir(std::string path);
  void print_files();
  File* get_file();
  void next_file();
  FileManager(/* args */){};
  ~FileManager(){};
};

#endif