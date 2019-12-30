#pragma once

#include <stdint.h>
#include <glibmm/refptr.h>

enum class Domain : uint8_t
{
  Lpc = 1,
};

void printCurrentTime(const char *s);
void printCurrentTime(const char *s, int64_t ms);

namespace Gio
{
  class File;
  class InputStream;
  class AsyncResult;
  class Cancellable;
}

namespace Glib
{
  class Bytes;
  class IOChannel;

}
