#include <Application.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetList.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetListContent.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetListHeader.h>
#include <memory>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include "presets/Preset.h"
#include "use-cases/PresetManagerUseCases.h"
#include <device-settings/Settings.h>
#include <device-settings/DirectLoadSetting.h>

PresetList::PresetList(const Rect& pos, bool showBankArrows)
    : super(pos, showBankArrows)
{
  Application::get().getPresetManager()->onBankSelection(mem_fun(this, &PresetList::onBankSelectionChanged));
  Application::get().getPresetManager()->onRestoreHappened(mem_fun(this, &PresetList::onBankChanged));
  Application::get().getPresetManager()->getEditBuffer()->onPresetLoaded(
      mem_fun(this, &PresetList::onEditBufferChanged));
  Application::get().getPresetManager()->onMidiBankSelectionHappened(
      sigc::mem_fun(this, &PresetList::onMidiBankChanged));
}

PresetList::~PresetList() = default;

void PresetList::onBankSelectionChanged(const Uuid& selectedBank)
{
  m_bankChangedConnection.disconnect();

  if(auto bank = Application::get().getPresetManager()->getSelectedBank())
  {
    m_bankChangedConnection = bank->onBankChanged(mem_fun(this, &PresetList::onBankChanged));
  }
  else
  {
    onBankChanged();
  }
}

void PresetList::onEditBufferChanged()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();

  if(m_uuidOfLastLoadedPreset != eb->getUUIDOfLastLoadedPreset())
  {
    m_uuidOfLastLoadedPreset = eb->getUUIDOfLastLoadedPreset();
    onBankChanged();
  }
}

void PresetList::onMidiBankChanged(const Uuid& midiBank)
{
  onBankChanged();
}

void PresetList::onBankChanged()
{
  if(auto bank = Application::get().getPresetManager()->getSelectedBank())
  {
    m_header->setup(bank);

    if(auto p = bank->getSelectedPreset())
      m_content->setup(bank, bank->getPresetPosition(p));
    else
      m_content->setup(bank, size_t(-1));
  }
  else
  {
    m_header->setup(nullptr);
    m_content->setup(nullptr, size_t(-1));
  }
}

bool PresetList::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
    auto focusAndMode = famSetting.getState();
    auto pm = Application::get().getPresetManager();
    auto settings = Application::get().getSettings();
    PresetManagerUseCases useCase(*pm, *settings);

    switch(i)
    {
      case Buttons::BUTTON_B:
        if(focusAndMode.focus == UIFocus::Banks)
          useCase.selectPreviousPreset();
        else
          useCase.selectPreviousBank();
        return true;

      case Buttons::BUTTON_C:
        if(focusAndMode.focus == UIFocus::Banks)
          useCase.selectNextPreset();
        else
          useCase.selectNextBank();
        return true;
    }
  }

  return false;
}

void PresetList::onRotary(int inc, ButtonModifiers modifiers)
{
  auto& famSetting = *Application::get().getSettings()->getSetting<FocusAndModeSetting>();
  auto focusAndMode = famSetting.getState();
  auto pm = Application::get().getPresetManager();

  if(focusAndMode.focus == UIFocus::Banks)
  {
    stepBankSelection(inc, modifiers, pm);
  }
  else if(auto bank = pm->getSelectedBank())
  {
    if(modifiers[ButtonModifier::SHIFT])
      jumpToPresetListExtreme(inc, bank);
    else
      stepPresetSelection(inc, pm, bank);
  }
}

void PresetList::stepBankSelection(int inc, const ButtonModifiers& modifiers, PresetManager* pm) const
{
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.stepBankSelection(inc, modifiers[SHIFT]);
}

void PresetList::stepPresetSelection(int inc, PresetManager* pm, Bank* bank) const
{
  if(bank)
  {
    BankUseCases useCase(bank, *Application::get().getSettings());
    useCase.stepPresetSelection(inc);
  }
}

std::pair<size_t, size_t> PresetList::getSelectedPosition() const
{
  auto pm = Application::get().getPresetManager();

  if(auto b = pm->getSelectedBank())
  {
    auto bankPos = pm->getBankPosition(b->getUuid());
    auto presetPos = b->getPresetPosition(b->getSelectedPreset());
    return std::make_pair(bankPos, presetPos);
  }
  return { -1, -1 };
}

void PresetList::jumpToPresetListExtreme(int inc, Bank* bank)
{
  if(bank)
  {
    BankUseCases useCase(bank, *Application::get().getSettings());
    useCase.selectFirstOrLastPreset(inc);
  }
}
