#include <glib.h>
#include "FourStateLED.h"
#include "device-settings/DebugLevel.h"

static const char* s_ledDevFile = "/dev/ribbon_led";

FourStateLED::FourStateLED ()
{
  m_state = -1;
}

FourStateLED::~FourStateLED ()
{
}

std::ofstream &FourStateLED::getDeviceFile ()
{
  static std::ofstream f (s_ledDevFile);
  return f;
}

void FourStateLED::setState (char state, bool flush)
{
  if (m_state != state)
  {
    m_state = state;

    auto &file = getDeviceFile ();

    uint8_t id = getID ();
    uint8_t stateMask = (state & 0x03);

    file << id;
    file << stateMask;

    if (flush)
      FourStateLED::flush();
  }
}

void FourStateLED::flush()
{
  getDeviceFile().flush ();
}

char FourStateLED::getState () const
{
  return m_state;
}
