#ifndef _TASK_H_
#define _TASK_H_
#include <iostream>
#include <string>

class Task {
 private:
  std::string arg1;
  std::string arg2;
  std::string arg3;
  std::string arg4;
  std::string arg5;
  std::string arg6;

 public:
  Task(std::string arg1, std::string arg2, std::string arg3, std::string arg4,
       std::string arg5, std::string arg6)
      : arg1(arg1),
        arg2(arg2),
        arg3(arg3),
        arg4(arg4),
        arg5(arg5),
        arg6(arg6){};
  bool check_argv();
  void back_up();
  void recover();
  void encrypt();
  void decrypt();
};

#endif