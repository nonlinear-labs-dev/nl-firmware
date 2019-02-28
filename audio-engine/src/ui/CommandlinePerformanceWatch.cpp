#include "CommandlinePerformanceWatch.h"
#include "io/audio/AudioOutput.h"
#include <glibmm.h>

CommandlinePerformanceWatch::CommandlinePerformanceWatch(const AudioOutput *device)
    : m_device(device)
{
  Glib::MainContext::get_default()->signal_timeout().connect_seconds(
      sigc::mem_fun(this, &CommandlinePerformanceWatch::printPerformance), 1);
}

bool CommandlinePerformanceWatch::printPerformance()
{
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(1);
  std::cout << "\rPerformance: " << m_device->getPerformance() << "%" << std::flush;
  return true;
}
