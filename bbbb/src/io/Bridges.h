#pragma once

#include "bbbb.h"

class Bridge;

class Bridges
{
  public:
    Bridges();
    virtual ~Bridges();

  private:
    using tBridge = std::unique_ptr<Bridge>;
    std::list<tBridge> m_bridges;
};

