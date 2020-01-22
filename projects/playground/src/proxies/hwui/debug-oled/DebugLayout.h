#pragma once

#include <proxies/hwui/DFBLayout.h>

class DebugLayout : public DFBLayout
{
 protected:
  using super = DFBLayout;

 public:
  explicit DebugLayout(Glib::ustring e);
  virtual bool onButton(Buttons i, bool down, ::ButtonModifiers modifiers);
};
