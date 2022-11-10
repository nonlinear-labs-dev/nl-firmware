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

void PlayModeLayout::createUpperLabels()
{
  remove(m_pedalSymbol);
  m_pedalSymbol = nullptr;
  remove(m_ribbonLabel);
  m_ribbonLabel = nullptr;

  if(isPedalMappedToCurrentUpperRibbon())
  {
    m_pedalSymbol = addControl(new PedalMappedToRibbonIndication(Rect(25, 1, 11, 14)));
    m_ribbonLabel
        = addControl(new RibbonLabel(HardwareSourcesGroup::getUpperRibbon1ParameterID(), Rect(36, 1, 77, 14)));
  }
  else
  {
    m_pedalSymbol = addControl(new PedalMappedToRibbonIndication(Rect(25, 1, 0, 14)));
    m_ribbonLabel
        = addControl(new RibbonLabel(HardwareSourcesGroup::getUpperRibbon1ParameterID(), Rect(25, 1, 88, 14)));
  }
}
