#include "Task.h"

#include "FileManager.h"
#include "Packer.h"
void Task::back_up() {
  FileManager fm;
  fm.read_dir(this->from);
  Packer packer(this->to, &fm);
  packer.pack();
}
void Task::recover() {}