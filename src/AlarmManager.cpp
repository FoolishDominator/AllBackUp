#include "AlarmManager.h"

#include "EncryptManager.h"
#include "PackerManager.h"

Alarm* Alarm::instance = nullptr;
void Alarm::handle_sigalrm(int sig) {
  // ...执行备份...
  time_t now_time_t;
  time(&now_time_t);
  char buf[20];
  tm* local_time = std::localtime(&now_time_t);
  strftime(buf, sizeof(buf), "%F %X", local_time);
  std::cout << "Backup started at " << buf;
  std::string new_bak_name = instance->bak_name + buf;
  FileManager fm;
  fm.read_dir(instance->from);
  Packer packer(instance->from, instance->to, new_bak_name, &fm);
  packer.pack();

  std::string file_path = instance->to + new_bak_name + ".bak";
  if (instance->password != "") {
    Encryptor encryptor(file_path, instance->password);
    encryptor.encrypt();
    unlink(file_path.c_str());
    file_path = instance->to + new_bak_name + ".sbak";
  }

  if (instance->old_exist) {
    unlink(file_path.c_str());
    instance->old_path = new_bak_name;
  } else {
    instance->old_path = new_bak_name;
    instance->old_exist = true;
  }
  // 重新设置定时器
  alarm(Alarm::get_alarm());
}

std::time_t Alarm::to_timestamp(int year, int month, int day, int hour,
                                int minute, int second) {
  char buffer[128];
  struct tm temp_tm;
  memset(&temp_tm, 0, sizeof(tm));
  temp_tm.tm_year = year - 1900;
  temp_tm.tm_mon = month - 1;
  temp_tm.tm_mday = day;
  temp_tm.tm_hour = hour;
  temp_tm.tm_min = minute;
  temp_tm.tm_sec = second;
  temp_tm.tm_isdst = 0;

  int ret;
  ret = mktime(&temp_tm);
  if (ret == -1) {
    std::cout << "Error: unable to make time using mktime" << std::endl;
  }
  return ret;
}
void Alarm::start_alarm() {
  become_daemon();

  // 休眠到指定时间点
  time_t now_time_t;
  time(&now_time_t);
  struct tm* t = gmtime(&now_time_t);
  std::time_t input_time;
  char buf[20];
  tm* local_time = std::localtime(&now_time_t);
  strftime(buf, sizeof(buf), "%F %X", local_time);
  std::cout << "Backup started at " << buf << std::endl
            << t->tm_mday << std::endl;

  if (t->tm_hour > hour || (t->tm_hour == hour && t->tm_min >= min)) {
    input_time = to_timestamp(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday + 1,
                              hour, min, 0);
  } else {
    input_time = to_timestamp(t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                              hour, min, 0);
  }
  std::cout << input_time << std::endl;
  std::cout << now_time_t << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(input_time - now_time_t));

  // 设置SIGALRM的处理函数
  signal(SIGALRM, handle_sigalrm);
  handle_sigalrm(SIGALRM);
  //  设置定时器
  alarm(BACKUP_INTERVAL);

  // 守护进程的主循环
  while (1) {
    pause();
  }
}
void Alarm::become_daemon() {
  pid_t pid = fork();
  if (pid < 0) exit(EXIT_FAILURE);
  if (pid > 0) exit(EXIT_SUCCESS);

  umask(0);
  setsid();

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}
