#ifndef _FILTER_MANAGER_H_
#define _FILTER_MANAGER_H_
#include <FileManager.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
class Config {
 public:
  std::string fileSize;
  std::string fileName;
  std::string fileType;
  std::string fileUser;
  std::string fileModifiedTime;
  std::string fileAuthority;

  bool parse_config(const std::string& configFilePath);
};

class Filter {
 private:
  FileManager* fm;
  Config* conf;

 public:
  Filter(FileManager* fm, Config* conf) : fm(fm), conf(conf){};
  ~Filter(){};
  void filter();
  std::string escapeRegex(const std::string& regex);
};
#endif