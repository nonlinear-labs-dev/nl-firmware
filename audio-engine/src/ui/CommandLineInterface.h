#pragma once

#include <glibmm.h>
#include <giomm.h>

class CommandLineInterface
{
 public:
  CommandLineInterface();
  virtual ~CommandLineInterface();

 protected:
  virtual void processLine(const std::string &line) = 0;

 private:
  void onKeyboardLineRead(Glib::RefPtr<Gio::AsyncResult> &res);

  Glib::RefPtr<Gio::Cancellable> m_readersCancel;
  Glib::RefPtr<Gio::DataInputStream> m_keyboardInput;
};
