#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_
#include <sys/stat.h>

#include <filesystem>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

#define FILETYPE_REG 1
#define FILETYPE_DIR 2
#define FILETYPE_SYMLNK 4
#define FILETYPE_FIFO 8
#define FILETYPE_HARDLNK 16
#define FILETYPE_OTHER 32

typedef unsigned char FileType;
class File {
 private:
  std::string path;
  struct stat metadata;
  // 用于保存硬链接或软链接的链接目标（对于硬链接即为打包时第一次遇见的文件）
  std::string linkpath;

 public:
  // 初始化File时根据元数据判断是否需要填充linkpath
  File(std::string path, struct stat s) : path(path), metadata(s){};
  ~File(){};
  std::string get_path() { return path; }
  struct stat get_stat() { return metadata; }
  std::string get_linkpath() { return linkpath; }
  void set_linkpath(std::string linkpath) { this->linkpath.assign(linkpath); }
  void set_linkpath(char *linkpath) { this->linkpath.assign(linkpath); }
  void set_nlink(unsigned long new_nlink) {
    this->metadata.st_nlink = new_nlink;
  }
  std::string get_name() {
    int pos = this->path.find_last_of('/');
    std::string name = this->path.substr(pos + 1, this->path.size() - pos - 1);
    return name;
  }
  FileType get_filetype();
  int get_file_size() { return this->metadata.st_size; }
  int get_user_id() { return this->metadata.st_uid; }
  time_t get_modified_time() { return this->metadata.st_mtim.tv_sec; }
  mode_t get_mode() { return this->metadata.st_mode; }
};

class FileManager {
 private:
  std::queue<File *> files;
  // 用于记录同一个inode的不同硬链接的linkpath
  std::unordered_map<ino_t, std::string> inode_table;

 public:
  int read_dir(std::string path);
  void print_files();
  File *get_file();
  void next_file();
  void next_file_2();
  int get_length() { return files.size(); }
  FileManager(/* args */){};
  ~FileManager(){};
};

#endif