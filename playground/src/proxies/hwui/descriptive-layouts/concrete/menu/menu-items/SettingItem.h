#pragma once

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/EditorItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-overlays/ChangeSettingOverlay.h>

template <class tSetting> class SettingItem : public EditorItem
{
 public:
  template <class tCap>
  explicit SettingItem(tCap c, const Rect& r)
      : EditorItem(c, r)
  {
    auto rightHalf = getPosition();
    rightHalf.setWidth((rightHalf.getWidth() / 2) - 20);
    rightHalf.setLeft(rightHalf.getWidth() + 30);

    m_label = addControl(new SettingLabel<tSetting>(rightHalf));
  }

  MenuOverlay* createOverlay() override
  {
    return new ChangeSettingOverlay<tSetting>(getDefaultOverlayRect());
  }

 protected:
  Control* m_label;
};
