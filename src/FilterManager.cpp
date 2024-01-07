#include "FilterManager.h"

#include <pwd.h>

time_t StringToDatetime(std::string str) {
  char* cha = (char*)str.data();               // 将string转换成char*。
  tm tm_;                                      // 定义tm结构体。
  int year, month, day, hour, minute, second;  // 定义时间的各个int临时变量。
  sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute,
         &second);  // 将string存储的日期时间，转换为int临时变量。
  tm_.tm_year =
      year -
      1900;  // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
  tm_.tm_mon =
      month -
      1;  // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
  tm_.tm_mday = day;         // 日。
  tm_.tm_hour = hour;        // 时。
  tm_.tm_min = minute;       // 分。
  tm_.tm_sec = second;       // 秒。
  tm_.tm_isdst = 0;          // 非夏令时。
  time_t t_ = mktime(&tm_);  // 将tm结构体转换成time_t格式。
  return t_;                 // 返回值。
}

bool Config::parse_config(const std::string& config_file_path) {
  std::ifstream config_file(config_file_path);
  if (!config_file.is_open()) {
    std::cerr << "Failed to open config file: " << config_file_path
              << std::endl;
    return false;
  }

  std::string content((std::istreambuf_iterator<char>(config_file)),
                      std::istreambuf_iterator<char>());
  config_file.close();

  std::regex configPattern(
      R"(\[(fileSize|fileName|fileUser|fileType|fileModifiedTime|fileAuthority):([^\]]+)\])");
  std::smatch matches;
  auto begin =
      std::sregex_iterator(content.begin(), content.end(), configPattern);
  auto end = std::sregex_iterator();

  for (std::sregex_iterator i = begin; i != end; ++i) {
    matches = *i;
    if (matches.size() == 3) {  // We expect 3 matches: the whole string, the
                                // type, and the capture group for the value
      std::string type = matches[1].str();
      std::string value = matches[2].str();

      if (type == "fileSize") {
        fileSize = value;
      } else if (type == "fileName") {
        fileName = value;
      } else if (type == "fileUser") {
        fileUser = value;
      } else if (type == "fileType") {
        fileType = value;
      } else if (type == "fileModifiedTime") {
        fileModifiedTime = value;
      } else if (type == "fileAuthority") {
        fileAuthority = value;
      }
    }
  }
  std::cout << "File Size: " << fileSize << std::endl;
  std::cout << "File Name: " << fileName << std::endl;
  std::cout << "File User: " << fileUser << std::endl;
  std::cout << "File Type: " << fileType << std::endl;
  std::cout << "File Modified Time: " << fileModifiedTime << std::endl;
  std::cout << "File Authority:" << fileAuthority << std::endl;
  return true;
}
std::string Filter::escapeRegex(const std::string& regex) {
  static const std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
  return std::regex_replace(regex, specialChars, R"(\$&)");
}
void Filter::filter() {
  File* file;
  int length = fm->get_length();

  for (int i = 0; i < length; i++) {
    bool select = true;
    file = fm->get_file();
    //
    if (file->get_filetype() != FILETYPE_DIR) {
      if (conf->fileName != "") {
        std::string escapedExtension = escapeRegex(conf->fileName);
        std::string patternStr = R"((.*))" + escapedExtension + R"($)";
        std::regex r(patternStr);
        if (!std::regex_search(file->get_name(), r)) select = false;
      }
      if (conf->fileType != "") {
        FileType conf_type;
        if (conf->fileType == "REGULAR") conf_type = FILETYPE_REG;
        if (conf->fileType == "DIR") conf_type = FILETYPE_DIR;
        if (conf->fileType == "SYMLINK") conf_type = FILETYPE_SYMLNK;
        if (conf->fileType == "HARDLINK") conf_type = FILETYPE_HARDLNK;
        if (conf->fileType == "FIFO") conf_type = FILETYPE_FIFO;
        if (file->get_filetype() != conf_type) select = false;
      }
      if (conf->fileSize != "") {
        int conf_size =
            atoi((conf->fileSize).substr(1, conf->fileSize.size() - 1).c_str());
        if (conf->fileSize[0] == '-' && (file->get_file_size() >= conf_size))
          select = false;
        if (conf->fileSize[0] == '+' && (file->get_file_size() <= conf_size))
          select = false;
      }
      if (conf->fileUser != "") {
        int conf_user_id = getpwnam(conf->fileUser.c_str())->pw_uid;
        if (conf_user_id != file->get_user_id()) select = false;
      }
      if (conf->fileAuthority != "") {
        int AuthorityNum = std::stoi(conf->fileAuthority, nullptr, 8);
        mode_t file_authority =
            file->get_mode() & (S_IRWXU | S_IRWXG | S_IRWXO);
        std::cout << (file_authority ^ AuthorityNum);
        if ((file_authority ^ AuthorityNum)) select = false;
      }
      if (conf->fileModifiedTime != "") {
        time_t conf_modifed_time =
            StringToDatetime(conf->fileModifiedTime.substr(
                1, conf->fileModifiedTime.size() - 1));
        if ((conf->fileModifiedTime[0] == '-') &&
            (file->get_modified_time() >= conf_modifed_time))
          select = false;
        if ((conf->fileModifiedTime[0] == '+') &&
            (file->get_modified_time() <= conf_modifed_time))
          select = false;
      }
    }
    if (select == true)
      fm->next_file_2();
    else
      fm->next_file();
  }
}

/* void printConfig() const {
  std::cout << "File Size: " << fileSize << std::endl;
  std::cout << "File Name: " << fileName << std::endl;
  std::cout << "File Path: " << filePath << std::endl;
  std::cout << "File Type: " << fileType << std::endl;
} */

/* int main() {
  Config myConfig;
  if (myConfig.parseConfig("config.conf")) {
    myConfig.printConfig();
  }
  return 0;
} */
