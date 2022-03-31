#include "playground-helpers.h"
#include <device-settings/DebugLevel.h>
#include <execinfo.h>
#include <nltools/logging/Log.h>

#include <glib.h>
#include <iomanip>
#include <limits>

std::string to_string(double d)
{
  std::stringstream ss;
  ss << std::setprecision(std::numeric_limits<double>::max_digits10) << d;
  return ss.str();
}

std::ostream& wirteIntToStream(std::ostream& stream, int i)
{
  return stream << i;
}

namespace Environment
{
  void printStackTrace(int i)
  {
    nltools::Log::warning("Crash signal caught!");

    const size_t max_frames = 64;
    void* addrlist[max_frames + 1];

    // retrieve current stack addresses
    guint32 addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if(addrlen == 0)
    {
      DebugLevel::warning("");
      return;
    }

    nltools::Log::warning("\n\nThe stack trace:");

    // create readable strings to each frame. __attribute__((no_instrument_function))
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // print the stack trace.
    for(guint32 i = 0; i < addrlen; i++)
      nltools::Log::warning(symbollist[i]);

    free(symbollist);
    exit(EXIT_FAILURE);
  }

  std::string getStackTrace(const std::string& prefix)
  {
    std::stringstream str;
    str << prefix << ":" << std::endl << std::endl;

    const size_t max_frames = 16;
    void* addrlist[max_frames + 1];
    guint32 addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if(addrlen != 0)
    {
      char** symbollist = backtrace_symbols(addrlist, addrlen);

      for(guint32 i = 0; i < addrlen; i++)
        str << symbollist[i] << std::endl;

      free(symbollist);
    }

    DebugLevel::error("Collected StackTrace: ", str.str());
    return str.str();
  }

  using nlLevel = nltools::Log::Level;

  void printbacktrace(nlLevel level, int maxFrames)
  {
    std::vector<void*> stack;
    stack.reserve(maxFrames);
    auto numFrames = backtrace(stack.data(), maxFrames);
    if(numFrames == maxFrames)
    {
      nltools::Log::error("You could be missing some frames from your backtrace. Try to increase your maxFrames");
    }

    auto symbols = backtrace_symbols(stack.data(), numFrames); //symbols is mallocced here

    for(auto i = 0; i < numFrames; i++)
    {
      nltools::Log::printWithLevel(level, i, symbols[i]);
    }

    free(symbols);
  }

  void setupLocale()
  {
    const char* desiredLocales[] = { "en_US.utf8@nonlinear", "en_US.utf8" };

    for(const auto desiredLocale : desiredLocales)
    {
      if(auto ret = setlocale(LC_ALL, desiredLocale))
      {
        if(g_strcmp0(ret, desiredLocale))
        {
          DebugLevel::warning("Desired locale was", desiredLocale, ", but current locale is:", ret);
        }
        else
        {
          DebugLevel::info("Successfully set locale to", desiredLocale);
          return;
        }
      }
    }

    DebugLevel::error("Could not set locale to any desired");
  }
}
