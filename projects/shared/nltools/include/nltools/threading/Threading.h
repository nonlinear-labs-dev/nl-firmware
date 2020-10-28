#pragma once

namespace nltools
{
  namespace threading
  {
    enum class Priority
    {
      Normal,
      AlmostRealtime,
      Realtime
    };

    void setThisThreadPrio(Priority p);
  }
}