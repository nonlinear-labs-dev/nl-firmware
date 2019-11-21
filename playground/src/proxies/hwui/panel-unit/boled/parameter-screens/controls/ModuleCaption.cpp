#include <proxies/hwui/TextCropper.h>
#include "ModuleCaption.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"

ModuleCaption::ModuleCaption(const Rect &pos)
    : super(pos)
{
  Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
      sigc::hide<0>(sigc::mem_fun(this, &ModuleCaption::onParameterSelected)));

  Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged(
      sigc::mem_fun(this, &ModuleCaption::onSelectionChanged));
}

Label::StringAndSuffix ModuleCaption::shortenStringIfNeccessary(std::shared_ptr<Font> font,
                                                                const Label::StringAndSuffix &text) const
{
  return TextCropper::shortenStringIfNeccessary(font, text.text, getWidth());
}

void ModuleCaption::onParameterSelected(Parameter *newOne)
{
  updateText(newOne);
}

bool ModuleCaption::enableVoiceGroupSuffix() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto selected = eb->getSelected();

  if(selected->getVoiceGroup() == VoiceGroup::Global)
    return false;

  if(dynamic_cast<MonoParameter *>(selected))
    return eb->getType() == SoundType::Split;
  if(dynamic_cast<UnisonGroup *>(selected->getParent()))
    return eb->getType() == SoundType::Split;

  if(dynamic_cast<MasterGroup *>(selected->getParent()))
    return false;
  return dynamic_cast<ScaleGroup *>(selected->getParent()) == nullptr;
};

void ModuleCaption::updateText(Parameter *newOne)
{
  if(newOne)
  {
    auto group = newOne->getParentGroup();
    auto groupName = group->getShortName();

    if(enableVoiceGroupSuffix())
    {
      auto sel = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
      auto suffix = std::string{};
      if(Application::get().getPresetManager()->getEditBuffer()->getType() != SoundType::Single)
        suffix = " " + toString(sel);
      setText(groupName + suffix);
    }
    else
      setText(groupName);
  }
}

void ModuleCaption::onSelectionChanged()
{
  auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected();
  updateText(selected);
}

bool ModuleCaption::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBuffer::Colors::C128);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}

void ModuleCaption::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBuffer::Colors::C43);
}

std::shared_ptr<Font> ModuleCaption::getFont() const
{
  return Oleds::get().getFont("Emphase_8_Regular", getFontHeight());
}

int ModuleCaption::getFontHeight() const
{
  return 8;
}
