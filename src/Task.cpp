#include "Task.h"

#include <string>

#include "EncryptManager.h"
#include "FileManager.h"
#include "PackerManager.h"
#include "colorful.h"
bool Task::check_argv() {
  if (this->arg1 == "-b") {
    if (access(arg2.c_str(), F_OK) == -1) {
      RED("The dir in arg2 is not exist");
      return false;
    }
    if (access(arg3.c_str(), F_OK) == -1) {
      RED("The dir in arg3 is not exist");
      return false;
    }
    if (this->arg5 != "-s" || this->arg5 != "") {
      RED("Unknow Option");
      return false;
    }
  }
  if (this->arg1 == "-r") {
    if (access(arg2.c_str(), F_OK) == -1) {
      RED("The file in arg2 is not exist");
      return false;
    }
    if (access(arg3.c_str(), F_OK) == -1) {
      RED("The dir in arg3 is not exist");
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
  if (this->arg5 == "-s") {
    std::string file_path = this->arg3 + "/" + this->arg4 + ".bak";
    Encryptor encryptor(file_path, this->arg6);
    encryptor.encrypt();
    unlink(file_path.c_str());
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
  } else {
    UnPacker unpacker(this->arg2, this->arg3);
    unpacker.unpack();
  }
}