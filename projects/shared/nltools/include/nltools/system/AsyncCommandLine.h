#pragma once
#include <string>
#include <functional>
#include "glibmm/spawn.h"
#include "glibmm/iochannel.h"

class AsyncCommandLine
{
 public:
  AsyncCommandLine(const std::vector<std::string>& command, std::function<void(const std::string&)> onSuccess,
                   std::function<void(const std::string&)> onError);

  ~AsyncCommandLine();
  bool isRunning() const;

 private:
  void watchHandler(Glib::Pid pid, int status);

  std::function<void(const std::string&)> m_onSuccess;
  std::function<void(const std::string&)> m_onError;

  Glib::Pid m_childPid;

  Glib::RefPtr<Glib::IOChannel> m_streamCout;
  Glib::RefPtr<Glib::IOChannel> m_streamCerr;

  int m_coutFD = 0;
  int m_cerrFD = 0;

  bool m_isRunning;
  void closeWatch(Glib::Pid pid);
};