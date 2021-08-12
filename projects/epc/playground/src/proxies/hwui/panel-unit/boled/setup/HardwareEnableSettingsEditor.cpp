#include "HardwareEnableSettingsEditor.h"
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/midi/RoutingSettings.h>

HardwareEnableSettingsEditor::HardwareEnableSettingsEditor(RoutingSettings::tRoutingIndex id)
: MenuEditor()
, m_id{id}
{
  Application::get().getSettings()->getSetting<RoutingSettings>()->onChange(sigc::mem_fun(this, &HardwareEnableSettingsEditor::onSettingChanged));
}

void HardwareEnableSettingsEditor::setPosition(const Rect& r)
{
  static const Rect menuEditorPosition(0, 16, 256, 96);
  MenuEditor::setPosition(menuEditorPosition);

  int y = 0;
  int h = 12;
  int w = menuEditorPosition.getWidth();
  int x = (menuEditorPosition.getWidth() - w) / 2;

  for(auto& c : getControls())
  {
    c->setPosition(Rect(x, y, w, h));
    y += h;
  }
}

void HardwareEnableSettingsEditor::drawBackground(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C128);
  fb.fillRect(getPosition());
}

void HardwareEnableSettingsEditor::incSetting(int inc)
{
}

const std::vector<Glib::ustring>& HardwareEnableSettingsEditor::getDisplayStrings() const
{
  static std::vector<Glib::ustring> sTest = { "Foo", "Bar" };
  return sTest;
}

int HardwareEnableSettingsEditor::getSelectedIndex() const
{
  return 0;
}
