#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include "VoiceGroupIndicator.h"
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/SwitchVoiceGroupButton.h>
#include <groups/MacroControlsGroup.h>
#include <parameter_declarations.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>

VoiceGroupIndicator::VoiceGroupIndicator(const Rect& r, bool allowLoadToPart, bool alwaysDraw)
    : Control(r)
    , m_allowLoadToPart(allowLoadToPart)
    , m_alwaysDraw(alwaysDraw)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vgManager = Application::get().getVGManager();
  auto vg = vgManager->getCurrentVoiceGroup();

  eb->onSoundTypeChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onSoundTypeChanged));
  eb->onSelectionChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onParameterSelectionChanged), vg);

  vgManager->onCurrentVoiceGroupChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onVoiceGroupSelectionChanged));
  vgManager->onLoadToPartModeChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onLoadModeChanged));
}

VoiceGroupIndicator::~VoiceGroupIndicator()
{
  m_parameterChanged.disconnect();
}

bool VoiceGroupIndicator::redraw(FrameBuffer& fb)
{
  if(shouldDraw() || m_alwaysDraw)
  {
    if(m_currentSoundType == SoundType::Split)
      return drawSplit(fb);
    else if(m_currentSoundType == SoundType::Layer)
      return drawLayer(fb);
    else if(m_currentSoundType == SoundType::Single)
      return drawSingle(fb);
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

  if(m_inLoadToPart)
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
    fb.setColor(FrameBufferColors::C43);

    auto centerX = absPos.getLeft() + 5;
    auto centerY = absPos.getTop() + 2;
    drawCrossForLayer(fb, centerX, centerY);
  }

  if(isLayerPartMuted(VoiceGroup::II))
  {
    fb.setColor(FrameBufferColors::C43);

    auto centerX = absPos.getLeft() + 5;
    auto centerY = absPos.getTop() + 9;
    drawCrossForLayer(fb, centerX, centerY);
  }

  return true;
}

void VoiceGroupIndicator::drawCrossForLayer(FrameBuffer& fb, int centerX, int centerY) const
{
  fb.setPixel(centerX - 2, centerY - 2);
  fb.setPixel(centerX - 2, centerY + 2);
  fb.setPixel(centerX - 1, centerY - 1);
  fb.setPixel(centerX - 1, centerY + 1);
  fb.setPixel(centerX, centerY);
  fb.setPixel(centerX + 1, centerY);
  fb.setPixel(centerX + 3, centerY - 2);
  fb.setPixel(centerX + 3, centerY + 2);
  fb.setPixel(centerX + 2, centerY - 1);
  fb.setPixel(centerX + 2, centerY + 1);
}

void VoiceGroupIndicator::drawCrossForSingle(FrameBuffer& fb, int centerX, int centerY) const
{
  fb.setPixel(centerX - 1, centerY - 1);
  fb.setPixel(centerX - 1, centerY + 1);
  fb.setPixel(centerX, centerY);
  fb.setPixel(centerX + 1, centerY);
  fb.setPixel(centerX + 2, centerY - 1);
  fb.setPixel(centerX + 2, centerY + 1);
}

bool VoiceGroupIndicator::drawSingle(FrameBuffer& fb)
{
  Rect polyRect = { getPosition().getX(), getPosition().getY() + 3, 6, 6 };
  Rect monoI = { getPosition().getX() + 6, getPosition().getY(), 6, 5 };
  Rect monoII = { getPosition().getX() + 6, getPosition().getY() + 7, 6, 5 };

  fb.setColor(FrameBufferColors::C179);
  fb.fillRect(polyRect);
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::I ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(monoI);
  fb.setColor(m_selectedVoiceGroup == VoiceGroup::II ? FrameBufferColors::C255 : FrameBufferColors::C128);
  fb.fillRect(monoII);

  if(m_inLoadToPart)
  {
    const auto startX = monoI.getRight();
    auto startY = m_selectedVoiceGroup == VoiceGroup::I ? monoI.getTop() : monoII.getTop();
    fb.setColor(FrameBufferColors::C255);
    fb.drawHorizontalLine(startX + 2, startY - 2, 1);
    fb.drawHorizontalLine(startX + 1, startY - 1, 2);
    fb.drawHorizontalLine(startX, startY, 3);
    fb.drawHorizontalLine(startX + 1, startY + 1, 2);
    fb.drawHorizontalLine(startX + 2, startY + 2, 1);
  }

  if(isSingleFXNotActive(VoiceGroup::I))
  {
    fb.setColor(FrameBufferColors::C43);
    auto centerX = monoI.getCenter().getX() - 1;
    auto centerY = monoI.getTop() + 2;
    drawCrossForSingle(fb, centerX, centerY);
  }

  if(isSingleFXNotActive(VoiceGroup::II))
  {
    fb.setColor(FrameBufferColors::C43);
    auto centerX = monoII.getCenter().getX() - 1;
    auto centerY = monoII.getTop() + 2;
    drawCrossForSingle(fb, centerX, centerY);
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

  if(m_inLoadToPart)
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

void VoiceGroupIndicator::onSoundTypeChanged(SoundType type)
{
  m_currentSoundType = type;
  setDirty();
}

void VoiceGroupIndicator::onParameterChanged(const Parameter* parameter)
{
  const auto paramNum = parameter->getID().getNumber();

  if(paramNum == C15::PID::Split_Split_Point || MacroControlsGroup::isMacroControl(paramNum))
    m_selectedVoiceGroup = Application::get().getVGManager()->getCurrentVoiceGroup();

  setDirty();
}

namespace
{
  bool isLayerGlobal(SoundType type, int parameterNumber)
  {
    if(type == SoundType::Layer)
    {
      static auto sGlobalGroupsInLayer
          = { C15::PID::Mono_Grp_Enable, C15::PID::Mono_Grp_Glide, C15::PID::Mono_Grp_Legato, C15::PID::Mono_Grp_Prio,
              C15::PID::Unison_Detune,   C15::PID::Unison_Voices,  C15::PID::Unison_Pan,      C15::PID::Unison_Phase };
      auto it = std::find(sGlobalGroupsInLayer.begin(), sGlobalGroupsInLayer.end(), parameterNumber);
      return it != sGlobalGroupsInLayer.end();
    }
    return false;
  }
}

void VoiceGroupIndicator::onParameterSelectionChanged(const Parameter* old, const Parameter* newParam)
{
  m_parameterChanged.disconnect();
  if(newParam)
  {
    m_param = newParam;

    auto ebType = Application::get().getPresetManager()->getEditBuffer()->getType();

    if(!ParameterId::isGlobal(newParam->getID().getNumber()) && !isLayerGlobal(ebType, newParam->getID().getNumber()))
    {
      m_selectedVoiceGroup = newParam->getID().getVoiceGroup();
      setDirty();
    }

    m_parameterChanged = newParam->onParameterChanged(sigc::mem_fun(this, &VoiceGroupIndicator::onParameterChanged));
  }
}

bool VoiceGroupIndicator::shouldDraw()
{
  if(m_param)
  {
    auto id = m_param->getID();
    auto num = id.getNumber();

    auto ret = SwitchVoiceGroupButton::allowToggling(m_param, Application::get().getPresetManager()->getEditBuffer());
    ret |= MacroControlsGroup::isMacroControl(num);
    ret |= num == C15::PID::Split_Split_Point;

    return ret;
  }
  return false;
}

void VoiceGroupIndicator::onVoiceGroupSelectionChanged(VoiceGroup vg)
{
  m_selectedVoiceGroup = vg;
  setDirty();
}

void VoiceGroupIndicator::onLoadModeChanged(bool loadModeActive)
{
  m_inLoadToPart = m_allowLoadToPart && loadModeActive;
  setDirty();
}

VoiceGroup VoiceGroupIndicator::getCurrentDisplayedVoiceGroup() const
{
  return m_selectedVoiceGroup;
}

bool VoiceGroupIndicator::isSingleFXNotActive(VoiceGroup vg) const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  auto fx_mix = eb->findParameterByID({ C15::PID::Master_FX_Mix, VoiceGroup::Global })->getControlPositionValue();
  auto serial_fx = eb->findParameterByID({ C15::PID::Master_Serial_FX, VoiceGroup::Global })->getControlPositionValue();
  auto to_fx = eb->findParameterByID({ C15::PID::Out_Mix_To_FX, VoiceGroup::I })->getControlPositionValue();

  auto fb_fx = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I })->getControlPositionValue();
  auto from_fx = eb->findParameterByID({ C15::PID::FB_Mix_FX_Src, VoiceGroup::I })->getControlPositionValue();

  auto fx_mix_cond = vg == VoiceGroup::I ? fx_mix == 1 : fx_mix == 0;
  auto serial_fx_cond = vg == VoiceGroup::I ? serial_fx > 0 : serial_fx < 0;
  auto to_fx_cond = vg == VoiceGroup::I ? to_fx < 1 : to_fx > 0;

  auto fb_fx_cond = fb_fx != 0;
  auto from_fx_cond = vg == VoiceGroup::I ? from_fx < 1 : from_fx > 0;

  auto fb_is_used = eb->findParameterByID({ C15::PID::FB_Mix_FX, VoiceGroup::I })->getControlPositionValue() != 0
      && eb->findParameterByID({ C15::PID::FB_Mix_Lvl, VoiceGroup::I })->getControlPositionValue() > 0
      && (eb->findParameterByID({ C15::PID::Osc_A_PM_FB, VoiceGroup::I })->getControlPositionValue() != 0
          || eb->findParameterByID({ C15::PID::Osc_B_PM_FB, VoiceGroup::I })->getControlPositionValue() != 0
          || eb->findParameterByID({ C15::PID::Shp_A_FB_Mix, VoiceGroup::I })->getControlPositionValue() > 0
          || eb->findParameterByID({ C15::PID::Shp_B_FB_Mix, VoiceGroup::I })->getControlPositionValue() > 0);

  return fx_mix_cond && !((serial_fx_cond || (from_fx_cond && fb_fx_cond)) && (to_fx_cond || fb_is_used));
}
