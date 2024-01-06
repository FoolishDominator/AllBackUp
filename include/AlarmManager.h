#ifndef _ALARM_MANAGER_H_
#define _ALARM_MANAGER_H_
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

class Alarm {
 private:
  static const int BACKUP_INTERVAL = 60;
  int hour;
  int min;
  std::string from;
  std::string to;
  std::string bak_name;
  std::string old_path;
  std::string password;
  bool old_exist;

 public:
  Alarm(std::string from, std::string to, std::string bak_name, int hour,
        int min, std::string password)
      : from(from),
        to(to),
        bak_name(bak_name),
        hour(hour),
        min(min),
        password(password),
        old_exist(false) {
    instance = this;
  };
  ~Alarm() { instance = NULL; };
  void start_alarm();
  static const int get_alarm() { return BACKUP_INTERVAL; }
  static void handle_sigalrm(int sig);
  std::time_t to_timestamp(int year, int month, int day, int hour, int minute,
                           int second);
  void become_daemon();
  static Alarm* instance;
};
#endif