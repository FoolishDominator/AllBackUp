#include "Task.h"

#include "FileManager.h"
#include "Packer.h"
void Task::back_up() {
  FileManager fm;
  fm.read_dir(this->arg1);
  Packer packer(this->arg1, this->arg2, this->arg3, &fm);
  packer.pack();
}
void Task::recover() {
  UnPacker unpacker(this->arg1, this->arg2);
  unpacker.unpack();
}