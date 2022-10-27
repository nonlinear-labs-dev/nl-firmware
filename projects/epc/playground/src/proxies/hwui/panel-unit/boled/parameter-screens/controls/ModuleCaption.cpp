#include <proxies/hwui/TextCropper.h>
#include "ModuleCaption.h"
#include "Application.h"
#include <proxies/hwui/HWUI.h>
#include <parameters/mono-mode-parameters/ModulateableMonoParameter.h>
#include <parameters/mono-mode-parameters/UnmodulateableMonoParameter.h>
#include <groups/MonoGroup.h>
#include <parameters/mono-mode-parameters/MonoGlideTimeParameter.h>
#include <parameters/UnisonVoicesParameter.h>
#include <groups/SplitParameterGroups.h>
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/FrameBuffer.h>
#include <sigc++/sigc++.h>

ModuleCaption::ModuleCaption(const Rect &pos, bool subscribeToSignals)
    : super(pos)
{
  if(subscribeToSignals)
  {
    auto vgManager = Application::get().getVGManager();
    auto currentVG = vgManager->getCurrentVoiceGroup();

    Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
        sigc::hide<0>(sigc::mem_fun(this, &ModuleCaption::onParameterSelected)), currentVG);

    vgManager->onCurrentVoiceGroupChanged(sigc::hide(sigc::mem_fun(this, &ModuleCaption::onSelectionChanged)));

    Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
        sigc::hide(sigc::mem_fun(this, &ModuleCaption::onSoundTypeChanged)));
  }
}

StringAndSuffix ModuleCaption::shortenStringIfNeccessary(const std::shared_ptr<Font> &font,
                                                         const StringAndSuffix &text) const
{
  return StringAndSuffix { TextCropper::shortenStringIfNeccessary(font, text.text, getWidth()) };
}

void ModuleCaption::onParameterSelected(Parameter *newOne)
{
  updateText(newOne);
}

void ModuleCaption::updateText(Parameter *newOne)
{
  if(newOne)
  {
    auto group = newOne->getParentGroup();
    auto groupName = group->getShortName();
    setText(StringAndSuffix { groupName });
  }
}

void ModuleCaption::onSelectionChanged()
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);
  updateText(selected);
}

void ModuleCaption::onSoundTypeChanged()
{
  auto vg = Application::get().getVGManager()->getCurrentVoiceGroup();
  auto selected = Application::get().getPresetManager()->getEditBuffer()->getSelected(vg);
  updateText(selected);
}

bool ModuleCaption::redraw(FrameBuffer &fb)
{
  const Rect &r = getPosition();

  fb.setColor(FrameBufferColors::C128);
  fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());

  super::redraw(fb);
  return true;
}

void ModuleCaption::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

std::shared_ptr<Font> ModuleCaption::getFont() const
{
  return Fonts::get().getFont("Emphase-8-Regular", getFontHeight());
}

int ModuleCaption::getFontHeight() const
{
  return 8;
}
