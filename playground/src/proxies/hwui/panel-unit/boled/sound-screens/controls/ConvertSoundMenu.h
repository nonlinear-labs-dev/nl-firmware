#pragma once

#include "proxies/hwui/controls/ButtonMenu.h"
#include <nltools/Types.h>

class ConvertSoundMenu : public ButtonMenu
{
 public:
  explicit ConvertSoundMenu(const Rect& rect);

 protected:
  void setup();
  void convertSoundTo(SoundType newType);
};
