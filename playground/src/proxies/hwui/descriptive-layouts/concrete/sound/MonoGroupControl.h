#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/controls/Rect.h>

class MonoGroupControl : public ControlWithChildren {
public:
  MonoGroupControl(const Rect& r);
  void rebuild();
private:
  sigc::connection m_connection;
};
