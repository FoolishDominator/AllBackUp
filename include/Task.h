#ifndef _TASK_H_
#define _TASK_H_
#include <iostream>
#include <string>

class Task {
 private:
  std::string from;
  std::string to;
  std::string mode;

 public:
  Task(std::string from, std::string to, std::string mode)
      : from(from), to(to), mode(mode){};
  void check_argv();
  void back_up();
  void recover();
};

#endif