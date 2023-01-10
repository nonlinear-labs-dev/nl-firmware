#include "Application.h"
#include "groups/HardwareSourcesGroup.h"
#include "PlayModeLayout.h"
#include "RibbonLabel.h"
#include "PedalMappedToRibbonIndication.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <device-settings/Settings.h>
#include <device-settings/SelectedRibbonsSetting.h>

PlayModeLayout::PlayModeLayout()
    : super()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto& settings = eb->getSettings();
  auto mcm = eb->getParameterGroupByID({ "MCM", VoiceGroup::Global });
  mcm->onGroupChanged(sigc::mem_fun(this, &PlayModeLayout::onMacroControlMappingsChanged));

  settings.getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &PlayModeLayout::onRibbonSelectionChanged));

  createUpperLabels();
}

PlayModeLayout::~PlayModeLayout()
{
}

void PlayModeLayout::onMacroControlMappingsChanged()
{
  createUpperLabels();
}

void PlayModeLayout::onRibbonSelectionChanged(const Setting* s)
{
  createUpperLabels();
}

namespace
{
  Rect getPedalIndicationRect(bool isMapped)
  {
    if(isMapped)
      return { 26, 1, 11, 13 };
    else
      return { 26, 1, 0, 13 };
  }

  Rect getRibbonLabelRect(bool isMapped)
  {
    if(isMapped)
      return { 36, 1, 77, 14 };
    else
      return { 25, 1, 88, 14 };
  }
}

void PlayModeLayout::createUpperLabels()
{
  remove(std::exchange(m_pedalSymbol, nullptr));
  remove(std::exchange(m_ribbonLabel, nullptr));

  const auto id = HardwareSourcesGroup::getUpperRibbon1ParameterID();
  auto pedalMapped = isPedalMappedToCurrentUpperRibbon();
  m_pedalSymbol = addControl(new PedalMappedToRibbonIndication(getPedalIndicationRect(pedalMapped)));
  m_ribbonLabel = addControl(new RibbonLabel(id, getRibbonLabelRect(pedalMapped)));
}
