#include "ArrowDown.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"
#include <proxies/hwui/Oleds.h>

ArrowDown::ArrowDown(const Rect &pos)
    : super(true, false, "w", pos)
{
}

ArrowDown::~ArrowDown()
{
}

std::shared_ptr<Font> ArrowDown::getFont() const
{
#warning "Remove Emphase Symbol references"
  return Oleds::get().getFont("Emphase-8-Symbol", getFontHeight());
}
