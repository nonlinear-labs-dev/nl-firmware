#include <proxies/hwui/Oleds.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetNameHeadlineLabel.h>
#include <proxies/hwui/FrameBuffer.h>
#include <presets/Preset.h>
#include <presets/Bank.h>

PresetNameHeadlineLabel::PresetNameHeadlineLabel(const Rect &pos)
    : super(pos)
{
  setText("Preset");

  Application::get().getPresetManager()->onBankSelection(mem_fun(this, &PresetNameHeadlineLabel::onBankChanged));
}

std::shared_ptr<Font> PresetNameHeadlineLabel::getFont() const
{
  return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());
}

int PresetNameHeadlineLabel::getFontHeight() const
{
  return 9;
}

void PresetNameHeadlineLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C255);
}

bool PresetNameHeadlineLabel::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C77);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}

void PresetNameHeadlineLabel::onBankChanged(const Uuid &newBankUuid)
{
  auto bank = Application::get().getPresetManager()->findBank(newBankUuid);
  m_currentBank = bank;
  if(m_currentBank)
    bank->onBankChanged(sigc::mem_fun(this, &PresetNameHeadlineLabel::onPresetSelectionChanged));
}

void PresetNameHeadlineLabel::onPresetSelectionChanged()
{
  if(m_currentBank == nullptr || m_currentBank->getSelectedPreset() == nullptr)
  {
    setText("");
    return;
  }

  auto selectedPreset = m_currentBank->getSelectedPreset();
  if(selectedPreset)
    setText(selectedPreset->getName());
  else
    setText("");
}
