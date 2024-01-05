#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <iostream>
#include <string>
class Terminal {
 public:
  static void showHelp();
  static void showVersion();
  static void error(std::string str);
};
#endif