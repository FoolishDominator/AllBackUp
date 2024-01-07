#ifndef _ENCRPYT_MANAGER_H_
#define _ENCRPYT_MANAGER_H_
#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>
class Encryptor {
 private:
  int ori_fp;
  int res_fp;
  std::string password;

 public:
  void encrypt();
  Encryptor(std::string ori_file_path, std::string password)
      : password(password) {
    this->ori_fp = open(ori_file_path.c_str(), O_RDONLY);
    int pos = ori_file_path.find_last_of('.');

    std::string res_file_path =
        ori_file_path.substr(0, pos + 1) + "s" +
        ori_file_path.substr(pos + 1, ori_file_path.size() - pos - 1);
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    this->res_fp = open(res_file_path.c_str(), O_CREAT | O_WRONLY, mode);
    OpenSSL_add_all_algorithms();
  };
  ~Encryptor() {
    close(this->ori_fp);
    close(this->res_fp);
  };
};

class Decryptor {
 private:
  int ori_fp;
  int res_fp;
  std::string password;
  std::string decrypted_path;

 public:
  Decryptor(std::string safe_file_path, std::string password)
      : password(password) {
    this->ori_fp = open(safe_file_path.c_str(), O_RDONLY);
    int pos = safe_file_path.find_last_of('.');
    decrypted_path =
        safe_file_path.substr(0, pos) + "_decrypted." +
        safe_file_path.substr(pos + 2, safe_file_path.size() - pos - 2);
    ;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    this->res_fp = open(decrypted_path.c_str(), O_CREAT | O_WRONLY, mode);
  }
  ~Decryptor() {
    close(this->ori_fp);
    close(this->res_fp);
  };
  void decrypt();
  std::string get_decrypted_path() { return decrypted_path; }
};

#endif