#pragma once
#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>

class SoundEditMenu : public ScrollMenu {
public:
  explicit SoundEditMenu(const Rect& r);
private:
  void onTypeChanged();
  sigc::connection m_connection;
};

