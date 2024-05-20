#include "Terminal.h"

#include <sys/xattr.h>

#include <iostream>
#include <string>

#include "AlarmManager.h"
#include "CLI11.hpp"
#include "EncryptManager.h"
#include "FileManager.h"
#include "FilterManager.h"
#include "PackerManager.h"
#include "Task.h"
#include "ZipManager.h"
#include "colorful.h"

void Terminal::showVersion() { std::cout << "CLI Tool Version 1.0\n"; }

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr
        << "Error: No arguments provided. Use --help for usage information.\n";
    return 1;
  }
  CLI::App app{"------------------BackUpTool------------------------"};
  app.footer("--------------------@Kita@--------------------------");
  // Version
  bool verbose = false;
  auto opt_v = app.add_flag("-v,--verbose", verbose, "Enable verbose output");
  // Backup
  bool backup = false;
  auto opt_b = app.add_flag("-b,--backup", backup, "Start Backup");
  std::string bakname, bakDirPath, bakSavePath, confPath, password;
  auto filename_opt =
      app.add_option("--bakname", bakname, "Specify the bakname")
          ->needs("-b")

          ->group("-b");
  auto originPath_opt =
      app.add_option("--bakDirPath", bakDirPath, "Specify the original path")
          ->needs("-b")

          ->check(CLI::ExistingDirectory)
          ->group("-b");
  auto targetPath_opt =
      app.add_option("--bakSavePath", bakSavePath, "Specify the target path")
          ->needs("-b")

          ->check(CLI::ExistingDirectory)
          ->group("-b");
  auto confPath_opt =
      app.add_option("--config", confPath, "Specify the config path")
          ->needs("-b")
          ->check(CLI::ExistingFile)
          ->group("-b");
  auto password_opt =
      app.add_option("--password", password, "Specify the password to encrypt")
          ->needs("-b")
          ->group("-b");
  // recover
  bool rec = false;
  auto opt_r = app.add_flag("-r,--recover", rec, "Start Recovery");
  std::string recFilePath, recSavePath;
  auto recname_opt =
      app.add_option("--recFilePath", recFilePath, "Specify the recFilePath")
          ->needs("-r")

          ->group("-r");
  auto recPath_opt =
      app.add_option("--recSavePath", recSavePath, "Specify the recSavePath")
          ->needs("-r")

          ->group("-r");
  // parse
  CLI11_PARSE(app, argc, argv);

  // operation
  if (verbose)
    Terminal::showVersion();
  else if (backup) {
    if (filename_opt->count() == 0) {
      throw CLI::RequiredError("Option --bakname");
    }
    if (originPath_opt->count() == 0) {
      throw CLI::RequiredError("Option --bakDirPath");
    }
    if (targetPath_opt->count() == 0) {
      throw CLI::RequiredError("Option --bakSavePath");
    }
    // 备份操作
    FileManager fm;
    fm.read_dir(bakDirPath);

    if (!confPath.empty()) {
      Config config;
      config.parse_config(confPath);
      Filter filter(&fm, &config);
      filter.filter();
    }
    Packer packer(bakDirPath, bakSavePath, bakname, &fm);
    packer.pack();
    std::string file_path = bakSavePath + bakname + ".bak";
    std::string zfile_path = bakSavePath + bakname + ".zbak";
    std::string sfile_path = bakSavePath + bakname + ".sbak";
    setxattr(file_path.c_str(), "user.normal", "true", 5, 0);

    if (password.empty()) password = "123";
    Encryptor encryptor(file_path, password);
    encryptor.encrypt();
    unlink(file_path.c_str());
    std::rename(sfile_path.c_str(), file_path.c_str());

    Zip zip(file_path);
    zip.zip();
    unlink(file_path.c_str());
    std::rename(zfile_path.c_str(), file_path.c_str());

  } else if (rec) {
    if (recname_opt->count() == 0) {
      throw CLI::RequiredError("Option --recFilePath");
    }
    if (recPath_opt->count() == 0) {
      throw CLI::RequiredError("Option --recSavePath");
    }

    Zip zip(recFilePath);
    zip.unzip();
    std::string unzip_path = zip.get_unzip_path();
    unlink(recFilePath.c_str());
    rename(unzip_path.c_str(), recFilePath.c_str());

    std::string password2;
    std::cout << "Please Input Password(if empty,press enter to skip):";
    std::cin >> password2;

    if (password2.empty()) password2 = "123";
    Decryptor decrytor(recFilePath, password2);
    decrytor.decrypt();
    unlink(recFilePath.c_str());
    rename(decrytor.get_decrypted_path().c_str(), recFilePath.c_str());

    UnPacker unpacker(recFilePath, recSavePath);
    unpacker.unpack();
  }
  return 0;
}