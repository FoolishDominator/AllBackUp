#ifndef _TASK_H_
#define _TASK_H_
#include <iostream>
#include <string>

class Task {
 private:
  std::string arg1;
  std::string arg2;
  std::string arg3;

 public:
  Task(std::string arg1, std::string arg2, std::string arg3)
      : arg1(arg1), arg2(arg2), arg3(arg3){};
  void check_argv();
  void back_up();
  void recover();
};

#endif