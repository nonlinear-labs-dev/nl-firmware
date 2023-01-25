#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <sigc++/connection.h>
#include <proxies/hwui/ShortVsLongPress.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUIEnums.h>

class BankButton : public ControlWithChildren
{
 public:
  BankButton(const Rect& pos, bool bankFocus);
  bool onButton(Buttons b, bool down, ButtonModifiers modifiers);

 private:
  void installDefault(FocusAndMode focusAndMode);
  void installSmall(FocusAndMode focusAndMode);
  void onFocusAndModeChanged(const Setting* setting);

  std::unique_ptr<ShortVsLongPress> m_buttonAHandler;
  const bool m_bankFocus;
};
