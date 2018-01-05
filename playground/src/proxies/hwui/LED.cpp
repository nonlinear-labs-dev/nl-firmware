#include "LED.h"

LED::LED()
{
  m_ledID = 0;
}

LED::~LED()
{
}

void LED::setID(int id)
{
  m_ledID = id;
}

int LED::getID() const
{
  return m_ledID;
}
