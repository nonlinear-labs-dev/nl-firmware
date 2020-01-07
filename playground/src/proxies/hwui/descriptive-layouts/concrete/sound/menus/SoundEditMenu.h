#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/ScrollMenu.h>
#include <sigc++/connection.h>

class SoundEditMenu : public ScrollMenu
{
 public:
  explicit SoundEditMenu(const Rect& r);

  void blockOnTypeChanged();
  void removeBlockOnTypeChanged();

 private:
  void onTypeChanged();
  sigc::connection m_connection;

  bool m_supressOnTypeChanged = false;

  struct OnTypeChangedSupressor
  {
    OnTypeChangedSupressor(SoundEditMenu* menu)
        : m_menu{ menu }
    {
      m_menu->m_supressOnTypeChanged = true;
    }

    ~OnTypeChangedSupressor()
    {
      m_menu->m_supressOnTypeChanged = false;
    }

   private:
    SoundEditMenu* m_menu;
  };

 public:
  OnTypeChangedSupressor aquireCallbackBlocker()
  {
    return OnTypeChangedSupressor(this);
  }

 private:
  friend class OnTypeChangedSupressor;
};