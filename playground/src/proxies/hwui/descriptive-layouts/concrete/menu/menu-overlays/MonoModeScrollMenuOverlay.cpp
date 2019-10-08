#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/MonoParameterItem.h>
#include <parameters/Parameter.h>
#include "MonoModeScrollMenuOverlay.h"
#include <Application.h>
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

bool MonoModeScrollMenuOverlay::onButton(Buttons i, bool down, ButtonModifiers mod)
{
  m_menu.doLayout();
  return m_menu.onButton(i, down, mod);
}

MonoModeScrollMenuOverlay::MonoModeScrollMenuOverlay(const Rect &r)
    : MenuOverlay(r)
    , m_menu{ r }
{
  const auto ids = std::vector<Parameter::ID>{ 12345, 12346, 12347, 12348 };

  int i = 0;
  for(auto &id : ids)
  {
    const auto height = r.getHeight() / ids.size();
    auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    auto param = eb->findParameterByID(id, vg);
    m_menu.addItem<MonoParameterItem>(param->getLongName(), param, Rect{ 0, static_cast<int>(height * i), r.getWidth(), static_cast<int>(height) });
    i++;
  }

  m_menu.doLayout();
}

MonoModeScrollMenuOverlay::ScrollMenuOverlayMenu::ScrollMenuOverlayMenu(const Rect &r)
    : ScrollMenu(r)
{
}

void MonoModeScrollMenuOverlay::ScrollMenuOverlayMenu::init()
{
}

bool MonoModeScrollMenuOverlay::redraw(FrameBuffer &fb) {
  auto ret = MenuOverlay::redraw(fb);
  ret |= m_menu.redraw(fb);
  return ret;
}
