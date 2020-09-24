#include <io/Bridges.h>
#include <io/FromButtonsBridge.h>
#include <io/FromEncoderBridge.h>
#include <io/FromPlaycontrollerBridge.h>
#include <io/ToPlaycontrollerBridge.h>
#include <io/ToOledsBridge.h>
#include <io/ToPanelLEDsBridge.h>
#include <io/ToRibbonLEDsBridge.h>
#include <glibmm/bytes.h>

Bridges::Bridges()
{
  m_bridges.emplace_back(new ToPlaycontrollerBridge());
  m_bridges.emplace_back(new FromPlaycontrollerBridge());
  m_bridges.emplace_back(new FromButtonsBridge());
  m_bridges.emplace_back(new FromEncoderBridge());
  m_bridges.emplace_back(new ToOledsBridge());
  m_bridges.emplace_back(new ToPanelLEDsBridge());
  m_bridges.emplace_back(new ToRibbonLEDsBridge());
}

Bridges::~Bridges()
{
}
