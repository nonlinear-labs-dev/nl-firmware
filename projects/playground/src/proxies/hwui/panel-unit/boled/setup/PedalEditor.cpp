#include <Application.h>
#include <device-settings/PedalType.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalEditor.h>

PedalEditor::PedalEditor(std::shared_ptr<PedalType> m)
    : m_mode(m)
{
  m_mode->onChange(mem_fun(this, &PedalEditor::onSettingChanged));
}

PedalEditor::~PedalEditor() = default;

void PedalEditor::incSetting(int inc)
{
  m_mode->inc(inc, false);
}

const std::vector<Glib::ustring> &PedalEditor::getDisplayStrings() const
{
  return m_mode->getDisplayStrings();
}

int PedalEditor::getSelectedIndex() const
{
  auto v = getDisplayStrings();
  auto it = std::find(v.begin(), v.end(), m_mode->getDisplayString());
  if(it != v.end())
    return std::distance(v.begin(), it);
  return 0;
}
