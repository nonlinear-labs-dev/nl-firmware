#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include "VoiceGroupIndicator.h"
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/SwitchVoiceGroupButton.h>
#include <groups/MacroControlsGroup.h>
#include <proxies/hwui/HWUIHelper.h>
#include <parameter_declarations.h>

VoiceGroupIndicator::VoiceGroupIndicator(const Rect& r)
    : Control(r)
{
  m_soundTypeChanged = Application::get().getPresetManager()->getEditBuffer()->onSoundTypeChanged(
      sigc::mem_fun(this, &VoiceGroupIndicator::onSoundTypeChanged));

  auto eb = Application::get().getPresetManager()->getEditBuffer();

  m_parameterSelectionChanged
      = eb->onSelectionChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onParameterSelectionChanged));

  m_voiceGroupChanged = Application::get().getHWUI()->onCurrentVoiceGroupChanged(
      sigc::mem_fun(this, &VoiceGroupIndicator::onVoiceGroupSelectionChanged));
}

VoiceGroupIndicator::~VoiceGroupIndicator()
{
  m_soundTypeChanged.disconnect();
  m_parameterChanged.disconnect();
  m_voiceGroupChanged.disconnect();
}

bool VoiceGroupIndicator::redraw(FrameBuffer& fb)
{
  if(m_shouldDraw)
  {
    if(m_currentSoundType == SoundType::Split)
      return drawSplit(fb);
    else if(m_currentSoundType == SoundType::Layer)
      return drawLayer(fb);
  }

  return true;
}

bool VoiceGroupIndicator::drawLayer(FrameBuffer& fb)
{
  auto absPos = getPosition();
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::I ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(Rect(absPos.getLeft(), absPos.getTop(), 12, 5));
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::II ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(Rect(absPos.getLeft(), absPos.getTop() + 7, 12, 5));

  if(HWUIHelper::isLoadToPartActive())
  {
    const auto startX = absPos.getLeft() + 13;
    auto startY = absPos.getTop() + (m_selectedVoiceGroup == VoiceGroup::I ? 2 : 9);
    fb.setColor(FrameBufferColors::C255);
    fb.drawHorizontalLine(startX + 2, startY - 2, 1);
    fb.drawHorizontalLine(startX + 1, startY - 1, 2);
    fb.drawHorizontalLine(startX, startY, 3);
    fb.drawHorizontalLine(startX + 1, startY + 1, 2);
    fb.drawHorizontalLine(startX + 2, startY + 2, 1);
  }

  if(isLayerPartMuted(VoiceGroup::I))
  {
    if(m_selectedVoiceGroup == VoiceGroup::I)
      fb.setColor(FrameBufferColors::C77);
    else
      fb.setColor(FrameBufferColors::C255);

    auto centerX = absPos.getLeft() + 6;
    auto centerY = absPos.getTop() + 2;
    fb.setPixel(centerX - 2, centerY - 2);
    fb.setPixel(centerX - 2, centerY + 2);
    fb.setPixel(centerX - 1, centerY - 1);
    fb.setPixel(centerX - 1, centerY + 1);
    fb.setPixel(centerX, centerY);
    fb.setPixel(centerX + 2, centerY - 2);
    fb.setPixel(centerX + 2, centerY + 2);
    fb.setPixel(centerX + 1, centerY - 1);
    fb.setPixel(centerX + 1, centerY + 1);
  }

  if(isLayerPartMuted(VoiceGroup::II))
  {
    if(m_selectedVoiceGroup == VoiceGroup::I)
      fb.setColor(FrameBufferColors::C77);
    else
      fb.setColor(FrameBufferColors::C255);

    auto centerX = absPos.getLeft() + 6;
    auto centerY = absPos.getTop() + 9;
    fb.setPixel(centerX - 2, centerY - 2);
    fb.setPixel(centerX - 2, centerY + 2);
    fb.setPixel(centerX - 1, centerY - 1);
    fb.setPixel(centerX - 1, centerY + 1);
    fb.setPixel(centerX, centerY);
    fb.setPixel(centerX + 2, centerY - 2);
    fb.setPixel(centerX + 2, centerY + 2);
    fb.setPixel(centerX + 1, centerY - 1);
    fb.setPixel(centerX + 1, centerY + 1);
  }

  return true;
}

bool VoiceGroupIndicator::drawSplit(FrameBuffer& fb)
{
  auto absPos = getPosition();
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::I ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(Rect(absPos.getLeft(), absPos.getTop(), 5, 12));
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::II ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(Rect(absPos.getLeft() + 7, absPos.getTop(), 5, 12));

  if(HWUIHelper::isLoadToPartActive())
  {
    const auto startY = absPos.getTop() + 13;
    auto startX = absPos.getLeft() + (m_selectedVoiceGroup == VoiceGroup::I ? 2 : 9);
    fb.setColor(FrameBufferColors::C255);
    fb.drawVerticalLine(startX - 2, startY + 2, 1);
    fb.drawVerticalLine(startX - 1, startY + 1, 2);
    fb.drawVerticalLine(startX, startY, 3);
    fb.drawVerticalLine(startX + 1, startY + 1, 2);
    fb.drawVerticalLine(startX + 2, startY + 2, 1);
  }

  return true;
}

bool VoiceGroupIndicator::isLayerPartMuted(VoiceGroup vg) const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto mute = eb->findParameterByID({ C15::PID::Voice_Grp_Mute, vg }))
  {
    return mute->getControlPositionValue() > 0 && eb->getType() == SoundType::Layer;
  }
  return false;
}

void VoiceGroupIndicator::onSoundTypeChanged()
{
  m_currentSoundType = Application::get().getPresetManager()->getEditBuffer()->getType();
  setDirty();
}

void VoiceGroupIndicator::onParameterChanged(const Parameter* parameter)
{
  auto isSplit = parameter->getParentGroup()->getID().getName() == "Split";
  auto isMC = MacroControlsGroup::isMacroControl(parameter->getID().getNumber());

  m_shouldDraw
      = SwitchVoiceGroupButton::allowToggling(parameter, Application::get().getPresetManager()->getEditBuffer());
  m_shouldDraw |= isMC;
  m_shouldDraw |= isSplit;

  if(isSplit || isMC)
    m_selectedVoiceGroup = Application::get().getHWUI()->getCurrentVoiceGroup();
  else
    m_selectedVoiceGroup = parameter->getID().getVoiceGroup();

  onSoundTypeChanged();
  setDirty();
}

void VoiceGroupIndicator::onParameterSelectionChanged(const Parameter* old, const Parameter* newParam)
{
  m_parameterChanged.disconnect();
  if(newParam)
  {
    m_parameterChanged = newParam->onParameterChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onParameterChanged));
  }
}

void VoiceGroupIndicator::onVoiceGroupSelectionChanged(VoiceGroup vg)
{
  m_selectedVoiceGroup = vg;
  setDirty();
}
