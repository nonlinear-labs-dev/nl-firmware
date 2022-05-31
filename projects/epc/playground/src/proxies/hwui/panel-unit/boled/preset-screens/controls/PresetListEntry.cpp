#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <presets/Preset.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetListEntry.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetNameLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetNumberLabel.h>
#include <functional>
#include "PresetTypeLabel.h"
#include <glibmm/main.h>
#include <proxies/hwui/HWUI.h>

const float c_numAnimationSteps = 15;
const float c_animationLength = 500;  // ms

PresetListEntry::PresetListEntry(const Rect &pos)
    : super(pos)
    , m_animationProgress(0)
{
  auto inLoadToPart = Application::get().getHWUI()->isInLoadToPart();

  m_number = addControl(new PresetNumberLabel(Rect(0, 0, 19, 16)));

  if(inLoadToPart)
    m_name = addControl(new PresetNameLabel(Rect(19, 0, 98, 16)));
  else
    m_name = addControl(new PresetNameLabel(Rect(19, 0, 100, 16)));

  if(inLoadToPart)
    m_type = addControl(new PresetTypeLabel(Rect(111, 0, 14, 16)));
  else
    m_type = addControl(new PresetTypeLabel(Rect(119, 0, 7, 16)));
}

PresetListEntry::~PresetListEntry()
{
  m_animationConnection.disconnect();
  m_presetConnection.disconnect();
}

void PresetListEntry::setPreset(Preset *preset, bool selected)
{
  m_presetConnection.disconnect();
  m_selected = selected;

  if(preset)
    m_presetConnection = preset->onChanged(sigc::bind<0>(mem_fun(this, &PresetListEntry::onPresetChanged), preset));
  else
    onPresetChanged(nullptr);
}

void PresetListEntry::onPresetChanged(const Preset *preset)
{
  if(preset)
  {
    bool isLoaded
        = preset->getUuid() == Application::get().getPresetManager()->getEditBuffer()->getUUIDOfLastLoadedPreset();

    if(auto bank = dynamic_cast<Bank *>(preset->getParent()))
    {
      auto num = bank->getPresetPosition(preset->getUuid());
      m_number->update(num, m_selected, isLoaded);
      m_name->update(preset->getDisplayNameWithSuffixes(false), m_selected, isLoaded);
      m_type->update(preset);
      return;
    }
  }

  m_number->update(-1, false, false);
  m_name->update("", false, false);
  m_type->update(nullptr);
}

bool PresetListEntry::redraw(FrameBuffer &fb)
{
  super::redraw(fb);

  if(m_selected)
  {
    if(isHighlight())
      fb.setColor(FrameBufferColors::C179);
    else
      fb.setColor(FrameBufferColors::C103);

    const Rect &r = getPosition();
    fb.drawRect(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight());
  }

  if(m_animationProgress != 0)
  {
    Rect r = getPosition();

    if(m_animationProgress <= 50)
      r.setWidth(r.getWidth() * 2 * m_animationProgress / 100.0f);
    else
      r.setLeft(r.getLeft() + r.getWidth() * 2 * (m_animationProgress - 50) / 100.0f);

    fb.setColor(FrameBufferColors::C255);
    fb.fillRect(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight());
  }

  return true;
}

void PresetListEntry::animate(tCallback cb)
{
  m_animationConnection.disconnect();

  m_animationCallback = cb;
  m_animationProgress = 0;
  int interval = c_animationLength / c_numAnimationSteps;
  auto animation = sigc::mem_fun(this, &PresetListEntry::animationProgress);
  m_animationConnection = Glib::MainContext::get_default()->signal_timeout().connect(animation, interval);
}

bool PresetListEntry::isTransparent() const
{
  return true;
}

bool PresetListEntry::animationProgress()
{
  m_animationProgress += 100.0f / c_numAnimationSteps;

  if(m_animationProgress > 100)
  {
    m_animationProgress = 0;
    setDirty();
    doAnimationCallback();
  }
  else
  {
    setDirty();
  }

  return m_animationProgress != 0;
}

void PresetListEntry::doAnimationCallback()
{
  tCallback cb;
  swap(cb, m_animationCallback);

  if(cb)
    cb();
}
