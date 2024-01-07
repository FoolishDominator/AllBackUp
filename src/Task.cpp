#include "Task.h"

#include <stdlib.h>

#include <string>

#include "AlarmManager.h"
#include "EncryptManager.h"
#include "FileManager.h"
#include "PackerManager.h"
#include "ZipManager.h"
#include "colorful.h"
bool Task::check_argv() {
  if (this->arg1 == "-b" || this->arg1 == "--backup") {
    if (access(arg2.c_str(), F_OK) == -1) {
      RED("The dir in arg2 is not exist");
      return false;
    }
    if (this->arg2.back() != '/') this->arg2 += '/';
    if (access(arg3.c_str(), F_OK) == -1) {
      RED("The dir in arg3 is not exist");
      return false;
    }
    if (this->arg5 != "-s" && this->arg5 != "") {
      RED("Unknow Option");
      return false;
    }
    if (this->arg3.back() != '/') this->arg3 += '/';
  }
  if (this->arg1 == "-r" || this->arg1 == "--recover") {
    if (this->arg3.back() != '/') this->arg3 += '/';
    if (access(arg2.c_str(), F_OK) == -1) {
      RED("The file in arg2 is not exist");
      return false;
    }
    if (access(arg3.c_str(), F_OK) == -1) {
      RED("The dir in arg3 is not exist");
      return false;
    }
  }
  if (this->arg1 == "-a" || this->arg1 == "--alarm") {
    if (access(arg2.c_str(), F_OK) == -1) {
      RED("The file in arg2 is not exist");
      return false;
    }
    if (access(arg3.c_str(), F_OK) == -1) {
      RED("The dir in arg3 is not exist");
      return false;
    }
    if (atoi(arg5.c_str()) < 0 || atoi(arg5.c_str()) > 23) {
      RED("The hour is illegal");
      return false;
    }
    if (atoi(arg6.c_str()) < 0 || atoi(arg6.c_str()) > 59) {
      RED("The minute is illegal");
      return false;
    }
    if (this->arg7 != "-s" && this->arg7 != "") {
      RED("Unknow Option");
      return false;
    }
  }
  return true;
}
void Task::back_up() {
  FileManager fm;
  fm.read_dir(this->arg2);
  Packer packer(this->arg2, this->arg3, this->arg4, &fm);
  packer.pack();

  if (this->arg7 == "-z") {
    std::string file_path = this->arg3 + this->arg4 + ".bak";
    Zip zip(file_path, 0);
    zip.zip();
    // unlink(file_path.c_str());
    if (this->arg5 == "-s") {
      std::string file_path = this->arg3 + this->arg4 + ".zbak";
      Encryptor encryptor(file_path, this->arg6);
      encryptor.encrypt();
      // unlink(file_path.c_str());
    }
  } else {
    if (this->arg5 == "-s") {
      std::string file_path = this->arg3 + this->arg4 + ".bak";
      Encryptor encryptor(file_path, this->arg6);
      encryptor.encrypt();
      // unlink(file_path.c_str());
    }
  }
}
void Task::recover() {
  int pos = arg2.find_last_of('.');
  std::string back_string = arg2.substr(pos + 1, arg2.size() - pos - 1);
  if (memcmp(back_string.c_str(), "sbak", arg2.size() - pos - 1) == 0) {
    std::string password;
    std::cout << "Need Password:";
    std::cin >> password;
    Decryptor decryptor(this->arg2, password);
    decryptor.decrypt();
    UnPacker unpacker(decryptor.get_decrypted_path(), this->arg3);
    unpacker.unpack();
    unlink(decryptor.get_decrypted_path().c_str());
  } else if (memcmp(back_string.c_str(), "szbak", arg2.size() - pos - 1) == 0) {
    std::string password;
    std::cout << "Need Password:";
    std::cin >> password;
    Decryptor decryptor(this->arg2, password);
    decryptor.decrypt();
    Zip zip(decryptor.get_decrypted_path(), 1);
    zip.unzip();
    UnPacker unpacker(zip.get_unzip_path(), this->arg3);
    unpacker.unpack();
  } else if (memcmp(back_string.c_str(), "zbak", arg2.size() - pos - 1) == 0) {
    Zip zip(this->arg2, 1);
    zip.unzip();
    UnPacker unpacker(zip.get_unzip_path(), this->arg3);
    unpacker.unpack();
  } else {
    UnPacker unpacker(this->arg2, this->arg3);
    unpacker.unpack();
  }
}
void Task::alarm() {
  if (arg7 == "-s") {
    Alarm alarm(arg2, arg3, arg4, atoi(arg5.c_str()), atoi(arg6.c_str()), arg8);
    alarm.start_alarm();
  } else {
    Alarm alarm(arg2, arg3, arg4, atoi(arg5.c_str()), atoi(arg6.c_str()), "");
    alarm.start_alarm();
  }
}