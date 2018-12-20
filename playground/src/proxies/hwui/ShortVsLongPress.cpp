#include "ShortVsLongPress.h"

ShortVsLongPress::ShortVsLongPress(Callback shortPressAction, Callback longPressAction)
    : m_shortPressAction(shortPressAction)
    , m_longPressAction(longPressAction)
    , m_exp(bind(&ShortVsLongPress::expired, this))
{
}

void ShortVsLongPress::onButtonEvent(bool state)
{
  if(state)
    onButtonDown();
  else
    onButtonUp();
}

void ShortVsLongPress::onButtonDown()
{
  m_exp.refresh(std::chrono::milliseconds(600));
}

void ShortVsLongPress::onButtonUp()
{
  if(m_exp.isPending())
  {
    m_shortPressAction();
    m_exp.cancel();
  }
}

void ShortVsLongPress::expired()
{
  m_longPressAction();
}
