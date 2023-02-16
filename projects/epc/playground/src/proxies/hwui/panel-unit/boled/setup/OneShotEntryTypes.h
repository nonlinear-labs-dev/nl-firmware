#pragma once
#include <functional>
#include <optional>

namespace OneShotTypes
{
  using CB = std::function<void()>;
  struct StartCB
  {
    StartCB()
    {
      cb = std::nullopt;
    }

    StartCB(const CB& c)
    {
      cb = c;
    }
    std::optional<CB> cb;
  };

  struct FinishCB
  {
    FinishCB()
    {
      cb = std::nullopt;
    }

    FinishCB(const CB& c)
    {
      cb = c;
    }
    std::optional<CB> cb;
  };

  struct CatchAllCB
  {
    CatchAllCB()
    {
      cb = std::nullopt;
    }

    CatchAllCB(const CB& c)
    {
      cb = c;
    }
    std::optional<CB> cb;
  };
}