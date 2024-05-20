#include "Task.h"

#include <stdlib.h>

#include <string>

#include "AlarmManager.h"
#include "EncryptManager.h"
#include "FileManager.h"
#include "FilterManager.h"
#include "PackerManager.h"
#include "ZipManager.h"
#include "colorful.h"
void Task::alarm() {
  bool need_zip = false;
  if (arg9 == "-z") {
    need_zip = true;
  }
  std::string password = "";
  if (arg7 == "-s") {
    password = arg8;
  }
  Alarm alarm(arg2, arg3, arg4, atoi(arg5.c_str()), atoi(arg6.c_str()),
              password, need_zip);
  alarm.start_alarm();
}