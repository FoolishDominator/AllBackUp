#include "Terminal.h"

#include <iostream>
#include <string>

#include "Task.h"
void Terminal::showHelp() {
  std::cout << "Usage: cli_tool [option]\n"
            << "Options:\n"
            << "  -h, --help       Show this help message\n"
            << "  -v, --version    Show Version\n"
            << "  -b [from] [to], --backup     Start Backup\n"
            << "  -r [from] [to], --recover    Start Recovery\n";
}
void Terminal::showVersion() { std::cout << "CLI Tool Version 1.0\n"; }
int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr
        << "Error: No arguments provided. Use --help for usage information.\n";
    return 1;
  }

  std::string arg1 = argv[1];

  if (arg1 == "-h" || arg1 == "--help") {
    Terminal::showHelp();
  } else if (arg1 == "-v" || arg1 == "--version") {
    Terminal::showVersion();
  } else if (arg1 == "-b" || arg1 == "--backup") {
    std::string arg2 = argv[2];
    std::string arg3 = argv[3];
    std::string arg4 = argv[4];
    Task task(arg2, arg3, arg4);
    // task.checkArgv();
    task.back_up();
  } else if (arg1 == "-r" || arg1 == "--recover") {
    std::string arg2 = (argv[2] == NULL) ? (NULL) : (argv[2]);
    std::string arg3 = (argv[3] == NULL) ? (NULL) : (argv[3]);
    std::string arg4 = (argv[4] == NULL) ? (NULL) : (argv[4]);
    Task task(arg2, arg3, arg4);
    // task.checkArgv();
    task.recover();
  } else {
    std::cerr << "Unknown option: " << arg1
              << "\nUse --help for usage information.\n";
    return 1;
  }

  return 0;
}