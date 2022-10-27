#include "playground-helpers.h"
#include <execinfo.h>
#include <nltools/logging/Log.h>
#include <glib.h>
#include <iomanip>
#include <limits>
#include <cstddef>

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
  void printStackTrace(int)
  {
    nltools::Log::error("Crash signal caught!");

    const size_t max_frames = 64;
    void* addrlist[max_frames + 1];

    // retrieve current stack addresses
    guint32 addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if(addrlen == 0)
    {
      nltools::Log::error("");
      return;
    }

    nltools::Log::error("\n\nThe stack trace:");

    // create readable strings to each frame. __attribute__((no_instrument_function))
    char** symbollist = backtrace_symbols(addrlist, static_cast<int>(addrlen));

    // print the stack trace.
    for(guint32 idx = 0; idx < addrlen; idx++)
      nltools::Log::error(symbollist[idx]);

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
      char** symbollist = backtrace_symbols(addrlist, static_cast<int>(addrlen));

      for(guint32 i = 0; i < addrlen; i++)
        str << symbollist[i] << std::endl;

      free(symbollist);
    }

    nltools::Log::error("Collected StackTrace: ", str.str());
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

    auto symbols = backtrace_symbols(stack.data(), numFrames);  //symbols is mallocced here

    for(auto i = 0; i < numFrames; i++)
    {
      nltools::Log::printWithLevel(level, i, symbols[i]);
    }

    free(symbols);
  }

  void setupLocale()
  {
    constexpr auto preferedLocale = "en_US.utf8@nonlinear.UTF-8";
    constexpr auto fallbackLocale = "en_US.utf8";

    if(auto ret = setlocale(LC_ALL, preferedLocale); ret && !g_strcmp0(ret, preferedLocale))
    {
      nltools::Log::info("Successfully set locale to", preferedLocale);
    }
    else if(auto ret2 = setlocale(LC_ALL, fallbackLocale); ret2 && !g_strcmp0(ret2, fallbackLocale))
    {
      nltools::Log::info("Could not set locale to", preferedLocale, "but installed fallback", fallbackLocale,
                        "- strings containing NL special chars will not be sortable, some tests will fail!");
    }
    else
    {
      nltools::Log::throwException("Could not set locale to", preferedLocale, "or at least", fallbackLocale);
    }
  }
}
