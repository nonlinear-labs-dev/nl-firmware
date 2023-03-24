#include <Application.h>
#include <groups/HardwareSourcesGroup.h>
#include <parameters/Parameter.h>
#include <proxies/hwui/base-unit/BaseUnit.h>
#include <proxies/hwui/base-unit/PlayPanel.h>
#include <proxies/hwui/base-unit/soled/NoteShiftDisplay.h>
#include <proxies/hwui/base-unit/soled/PlayOrEditModeLayout.h>
#include <proxies/hwui/base-unit/soled/RibbonLabel.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/DottedLine.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/controls/VerticalLine.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <proxies/hwui/controls/Label.h>
#include "SelectedRibbonsIndication.h"
#include "PedalMappedToRibbonIndication.h"
#include "parameters/RibbonParameter.h"
#include "device-settings/BaseUnitUIDetail.h"
#include "RibbonModeLabel.h"
#include "LastTouchedRibbonLabel.h"
#include "device-settings/BaseUnitUIMode.h"
#include <device-settings/SelectedRibbonsSetting.h>
#include <device-settings/Settings.h>

PlayOrEditModeLayout::PlayOrEditModeLayout()
    : super(Application::get().getHWUI()->getBaseUnit().getPlayPanel().getSOLED())
{
  addControl(new Label(StringAndSuffix { "Shift" }, Rect(1, 1, 23, 14)));
  addControl(new NoteShiftDisplay(Rect(1, 17, 23, 14)));
  addControl(new VerticalLine(Rect(24, 0, 1, 32)));

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto& settings = eb->getSettings();
  eb->onSelectionChanged(mem_fun(this, &PlayOrEditModeLayout::onParameterSelectionChanged), {});

  settings.getSetting<SelectedRibbonsSetting>()->onChange(
      sigc::mem_fun(this, &PlayOrEditModeLayout::onRibbonSelectionChanged));

  settings.getSetting<BaseUnitUIDetail>()->onChange(
      sigc::mem_fun(this, &PlayOrEditModeLayout::onBaseUnitUIDetailChanged));

  auto mcm = eb->getParameterGroupByID({ "MCM", VoiceGroup::Global });
  mcm->onGroupChanged(sigc::mem_fun(this, &PlayOrEditModeLayout::onMacroMappingsChanged));
}

PlayOrEditModeLayout::~PlayOrEditModeLayout() = default;

void PlayOrEditModeLayout::onParameterSelectionChanged(const Parameter* old, Parameter* newP)
{
}

namespace
{
  Rect getPedalIndicationRect(bool isMapped)
  {
    if(isMapped)
      return { 26, 17, 11, 13 };
    else
      return { 26, 17, 0, 13 };
  }

  Rect getRibbonLabelRect(bool isMapped)
  {
    if(isMapped)
      return { 36, 17, 77, 14 };
    else
      return { 25, 17, 88, 14 };
  }

  Rect getRibbonModeLabelRect()
  {
    return { 113, 17, 8, 14 };
  }

  Rect getRibbonTouchedIndicatorRect()
  {
    return { 121, 17, 8, 14 };
  }

  Rect getDottedLineRect(bool isRibbonSelect)
  {
    if(isRibbonSelect)
      return { 25, 16, 86, 1 };
    else
      return { 25, 16, 120, 1 };
  }

  ParameterId getLowerIDForSetting(SelectedRibbons s)
  {
    if(s == SelectedRibbons::Ribbon1_2)
    {
      return HardwareSourcesGroup::getLowerRibbon2ParameterID();
    }
    else
    {
      return HardwareSourcesGroup::getLowerRibbon4ParameterID();
    }
  }

  SelectedRibbonsSetting* getSelectedRibbonsSetting()
  {
    return Application::get().getSettings()->getSetting<SelectedRibbonsSetting>();
  }
}

void PlayOrEditModeLayout::createLowerLabels()
{
  remove(std::exchange(m_pedalIndicationLower, nullptr));
  remove(std::exchange(m_ribbonLabelLower, nullptr));
  remove(std::exchange(m_ribbonModeLower, nullptr));
  remove(std::exchange(m_ribbonTouchedIndicatorLower, nullptr));

  const auto id = getLowerIDForSetting(getSelectedRibbonsSetting()->get());
  auto isMapped = isPedalMappedToCurrentLowerRibbon();

  m_pedalIndicationLower = addControl(new PedalMappedToRibbonIndication(getPedalIndicationRect(isMapped)));
  m_ribbonLabelLower = addControl(new RibbonLabel(id, getRibbonLabelRect(isMapped)));
  if(getBaseUnitUIDetails()->get() == BaseUnitUIDetails::RibbonMode)
  {
    m_ribbonModeLower = addControl(new RibbonModeLabel(id, getRibbonModeLabelRect()));
    m_ribbonTouchedIndicatorLower = addControl(new LastTouchedRibbonLabel(id, getRibbonTouchedIndicatorRect()));
  }
}

void PlayOrEditModeLayout::onRibbonSelectionChanged(const Setting* s)
{
  createLowerLabels();
}

bool is1_2Selected()
{
  return Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->get() == SelectedRibbons::Ribbon1_2;
}

bool PlayOrEditModeLayout::isPedalMappedToCurrentLowerRibbon()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto* param = eb->findAndCastParameterByID<RibbonParameter>(
      { is1_2Selected() ? C15::PID::Ribbon_2 : C15::PID::Ribbon_4, VoiceGroup::Global });
  return param->isMCAssignedToThisAlsoAssignedToAnyPedal();
}

bool PlayOrEditModeLayout::isPedalMappedToCurrentUpperRibbon()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto* param = eb->findAndCastParameterByID<RibbonParameter>(
      { is1_2Selected() ? C15::PID::Ribbon_1 : C15::PID::Ribbon_3, VoiceGroup::Global });
  return param->isMCAssignedToThisAlsoAssignedToAnyPedal();
}

void PlayOrEditModeLayout::onMacroMappingsChanged()
{
  createRightSideControls();
  createLowerLabels();
}

void PlayOrEditModeLayout::onBaseUnitUIDetailChanged(const Setting* s)
{
  createRightSideControls();
  createLowerLabels();
}

void PlayOrEditModeLayout::createRightSideControls()
{
  remove(std::exchange(m_dottedLine, nullptr));
  remove(std::exchange(m_selectedRibbonsIndication, nullptr));

  auto isRibbonSelect = getBaseUnitUIDetails()->get() == BaseUnitUIDetails::RibbonSelect;
  auto isEditMode
      = Application::get().getSettings()->getSetting<BaseUnitUIMode>()->get() == BaseUnitUIModes::ParameterEdit;
  if(isRibbonSelect && !isEditMode)
    m_selectedRibbonsIndication = addControl(new SelectedRibbonsIndication(Rect(115, 10, 15, 20)));
  m_dottedLine = addControl(new DottedLine(getDottedLineRect(isRibbonSelect)));
}

BaseUnitUIDetail* PlayOrEditModeLayout::getBaseUnitUIDetails()
{
  return Application::get().getSettings()->getSetting<BaseUnitUIDetail>();
}
