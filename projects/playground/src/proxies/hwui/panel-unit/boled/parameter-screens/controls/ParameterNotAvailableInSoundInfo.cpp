#include "ParameterNotAvailableInSoundInfo.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterLayout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <sigc++/adaptors/hide.h>
#include <proxies/hwui/controls/CenterAlignedLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModuleCaption.h>

ParameterNotAvailableInSoundInfo::ParameterNotAvailableInSoundInfo(const Rect &r, ParameterLayout2 *parent)
    : ControlWithChildren(r)
    , m_parent { parent }
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  m_parameterSelectionConnection
      = eb->onSelectionChanged(sigc::mem_fun(this, &ParameterNotAvailableInSoundInfo::onSelectionChanged));

  m_soundTypeConnection
      = eb->onSoundTypeChanged(sigc::hide(sigc::mem_fun(this, &ParameterNotAvailableInSoundInfo::onSoundTypeChanged)));

  addControl(new CenterAlignedLabel("Only available with", { 0, 8, 128, 10 }));
  addControl(new CenterAlignedLabel("Layer Sounds", { 0, 22, 128, 10 }));
}

void ParameterNotAvailableInSoundInfo::setBackgroundColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

void ParameterNotAvailableInSoundInfo::onSelectionChanged(const Parameter *old, const Parameter *newParam)
{
  auto vis = !ParameterLayout2::isParameterAvailableInSoundType(newParam);
  setVisible(vis);
}

void ParameterNotAvailableInSoundInfo::onSoundTypeChanged()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto current = eb->getSelected();
  const auto vis = !ParameterLayout2::isParameterAvailableInSoundType(current, eb);
  setVisible(vis);
}

void ParameterNotAvailableInSoundInfo::setVisible(bool b)
{
  for(auto &c : m_parent->getControls())
  {
    if(c.get() != this && dynamic_cast<const ModuleCaption *>(c.get()) == nullptr)
    {
      c->setVisible(!b);
    }
  }

  Control::setVisible(b);
}