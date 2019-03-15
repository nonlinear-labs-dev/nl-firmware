#pragma once

#include <glibmm.h>
#include <giomm.h>

class CommandLineInterface
{
 public:
  CommandLineInterface();
  virtual ~CommandLineInterface();

 protected:
  virtual void processByte(char c) = 0;

 private:
  void onKeyboardByteRead(Glib::RefPtr<Gio::AsyncResult> &res);
  bool onIO();
};
