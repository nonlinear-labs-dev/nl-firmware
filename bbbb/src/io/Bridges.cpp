#include <io/Bridges.h>
#include <io/FromButtonsBridge.h>
#include <io/FromEncoderBridge.h>
#include <io/FromLPCBridge.h>
#include <io/ToLPCBridge.h>
#include <io/ToOledsBridge.h>
#include <list>

Bridges::Bridges()
{
  m_bridges.emplace_back(new ToLPCBridge());
  m_bridges.emplace_back(new FromLPCBridge());
  m_bridges.emplace_back(new FromButtonsBridge());
  m_bridges.emplace_back(new FromEncoderBridge());
  m_bridges.emplace_back(new ToOledsBridge());
}

Bridges::~Bridges()
{
}

