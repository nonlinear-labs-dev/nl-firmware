#pragma once
#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuItems/EditorItem.h>
#include <proxies/hwui/descriptive-layouts/Concrete/Menu/MenuOverlay/ChangeSettingOverlay.h>

template <class tSetting> class SettingItem : public EditorItem
{
 public:
  template <class tCap>
  explicit SettingItem(tCap c)
      : EditorItem(c)
  {
    auto rightHalf = getPosition();
    rightHalf.setWidth((rightHalf.getWidth() / 2) - 20);
    rightHalf.setLeft(rightHalf.getWidth() + 30);

    m_label = addControl(new SettingLabel<tSetting>(rightHalf));
  }

  ScrollMenuOverlay* createOverlay() override
  {
 #warning"adlerauge ueberall nach Standart suchen"
    return new ChangeSettingOverlay<tSetting>(getStandartOverlayRect());
  }

 protected:
  Control* m_label;
};