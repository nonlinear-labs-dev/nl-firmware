#pragma once

#include <io/Bridge.h>
#include <io/Receiver.h>
#include <list>

class ToOledsBridge : public Bridge
{
  using super = Bridge;

 public:
  ToOledsBridge();
  virtual ~ToOledsBridge();

 private:
  void handleTimeStampedFramebuffer(Receiver::tMessage msg);
  void removeOldRecords();
  bool printTurnaroundTime();

  std::unique_ptr<Receiver> m_timeStampedFramebufferReceiver;

  struct Record
  {
    std::chrono::steady_clock::time_point captureTime;
    int64_t ms;
  };

  std::list<Record> m_timeStamps;
};
