#include "Terminal.h"

#include <iostream>
#include <string>

#include "Task.h"
void Terminal::showHelp()
{
  std::cout << "Usage: allbackup [option]\n"
            << "Options:\n"
            << "  -h, --help       Show this help message\n"
            << "  -v, --version    Show Version\n"
            << " -----------------backup------------------------\n"
            << "  -b/--backup [from] [to] [bakname]    Backup\n"
            << "  -b/--backup [from] [to] [bakname] -s [password]    Backup "
               "With Password\n"
            << "  -b/--backup [from] [to] [bakname] -s [password] -z  Backup "
               "With Password and Zip it\n"
            << " -----------------recover------------------------\n"
            << "  -r [from] [to], --recover  Recovery\n"
            << " -----------------alarm------------------------\n"
            << "  -a/--alarm [from] [to] [bakname] [hour] [minute], Scheduled "
               "backup\n"
            << " -----------------custom backup ------------------------\n"
            << "  -b/--backup [from] [to] [bakname] -s [password] -z -c [configfilepath]\n"
            << "\tthe format of the config file is like: \n"
            << "\t[fileSize:value] [fileName:value] [fileUser:value] [fileType:value] [fileModifiedTime:value] [fileAuthority:value]\n";
}
void Terminal::showVersion() { std::cout << "CLI Tool Version 1.0\n"; }
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cerr
        << "Error: No arguments provided. Use --help for usage information.\n";
    return 1;
  }

  std::string arg1 = argv[1];
  std::string arg2 = (argv[2] == NULL) ? ("") : (argv[2]);
  std::string arg3 = (argv[3] == NULL) ? ("") : (argv[3]);
  std::string arg4 = (argv[4] == NULL) ? ("") : (argv[4]);
  std::string arg5 = (argv[5] == NULL) ? ("") : (argv[5]);
  std::string arg6 = (argv[6] == NULL) ? ("") : (argv[6]);
  std::string arg7 = (argv[7] == NULL) ? ("") : (argv[7]);
  std::string arg8 = (argv[8] == NULL) ? ("") : (argv[8]);
  std::string arg9 = (argv[9] == NULL) ? ("") : (argv[9]);

  Task task(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
  if (arg1 == "-h" || arg1 == "--help")
  {
    Terminal::showHelp();
  }
  else if (arg1 == "-v" || arg1 == "--version")
  {
    Terminal::showVersion();
  }
  else if (arg1 == "-b" || arg1 == "--backup")
  {
    if (task.check_argv())
      task.back_up();
  }
  else if (arg1 == "-r" || arg1 == "--recover")
  {
    if (task.check_argv())
      task.recover();
  }
  else if (arg1 == "-a" || arg1 == "--alarm")
  {
    task.alarm();
  }
  else
  {
    std::cerr << "Unknown option: " << arg1
              << "\nUse --help for usage information.\n";
    return 1;
  }

  return 0;
}