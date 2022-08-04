#include <utility>

#include <Application.h>
#include <device-settings/DirectLoadSetting.h>
#include <device-settings/Settings.h>
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/AppendOverwriteInsertButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankAndPresetNumberLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/NumBanksLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/NumPresetsInBankLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetList.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetListSelectStorePosition.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <functional>
#include <memory>
#include <vector>
#include <proxies/hwui/panel-unit/boled/undo/UndoIndicator.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/AnyParameterLockedIndicator.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/LoadModeMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/VoiceGroupIndicator.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MuteIndicator.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankButton.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/LoadToPartPresetList.h>
#include "presets/Preset.h"
#include "SelectVoiceGroupLayout.h"
#include "RenameBankLayout.h"
#include "clipboard/Clipboard.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/EditBufferUseCases.h"
#include "proxies/hwui/panel-unit/boled/preset-screens/controls/ChangedParameterIndicator.h"

PresetManagerLayout::PresetManagerLayout(FocusAndMode focusAndMode, FocusAndMode oldFocusAndMode)
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
    , m_focusAndMode(focusAndMode)
    , m_oldFocusAndMode(oldFocusAndMode)
{
  m_dlSettingConnection = Application::get().getSettings()->getSetting<DirectLoadSetting>()->onChange(
      sigc::hide(sigc::mem_fun(this, &PresetManagerLayout::setup)));

  m_loadToPartConnection = Application::get().getVGManager()->onLoadToPartModeChanged(
      sigc::hide(sigc::mem_fun(this, &PresetManagerLayout::setup)));
}

PresetManagerLayout::~PresetManagerLayout()
{
  m_focusAndMode.mode = UIMode::Edit;
  m_dlSettingConnection.disconnect();
  m_loadToPartConnection.disconnect();
}

void PresetManagerLayout::setFocusAndMode(FocusAndMode focusAndMode)
{
  if(m_focusAndMode != focusAndMode)
  {
    m_focusAndMode = focusAndMode;
    setup();
  }
}

void PresetManagerLayout::setup()
{
  m_menu = nullptr;
  m_presets = nullptr;
  m_loadMode = nullptr;
  m_bankButton = nullptr;
  m_changedIndicator = nullptr;

  clear();

  if(m_focusAndMode.focus == UIFocus::Banks)
  {
    setupBankFocus();
  }
  else
  {
    setupPresetFocus();
  }
}

PresetManager *PresetManagerLayout::getPresetManager() const
{
  return Application::get().getPresetManager();
}

void PresetManagerLayout::setupBankFocus()
{
  m_bankButton = addControl(new BankButton({ 3, 36, 58, 57 }, true));

  switch(m_focusAndMode.mode)
  {
    case UIMode::Edit:
      setupBankEdit();
      break;

    case UIMode::Select:
      setupBankSelect();
      break;

    case UIMode::Store:
      setupBankStore();
      break;
  }
}

void PresetManagerLayout::setupBankEdit()
{
  if(getStoreModeData() != nullptr)
  {
    setStoreModeData(nullptr);
  }

  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new InvertedLabel("Edit", Rect(8, 26, 48, 12)))->setHighlight(true);

  m_menu = addControl(new BankEditButtonMenu(Rect(195, 1, 58, 62)));
  m_presets = addControl(new PresetList(Rect(64, 0, 128, 63), true));
  m_presets->setBankFocus();
}

void PresetManagerLayout::setupBankSelect()
{
  auto hwui = Application::get().getHWUI();
  auto vgManager = Application::get().getVGManager();

  if(getStoreModeData() != nullptr)
  {
    setStoreModeData(nullptr);
  }
  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new NumBanksLabel(Rect(208, 1, 32, 14)))->setHighlight(false);

  addControl(new VoiceGroupIndicator(Rect(2, 15, 16, 16), true));
  addControl(new UndoIndicator(Rect(22, 15, 10, 8)));
  addControl(new AnyParameterLockedIndicator(Rect(244, 2, 10, 11)));
  m_loadMode = addControl(new LoadModeMenu(Rect(195, 36, 58, 62)));

  auto isDualEB = Application::get().getPresetManager()->getEditBuffer()->isDual();

  if(isDualEB && vgManager->isInLoadToPart())
    m_presets
        = addControl(new LoadToPartPresetList(Rect(64, 0, 128, 63), true, vgManager->getPresetPartSelection(VoiceGroup::I),
                                              vgManager->getPresetPartSelection(VoiceGroup::II)));
  else
    m_presets = addControl(new PresetList({ 64, 0, 128, 63 }, true));

  m_presets->setBankFocus();
}

void PresetManagerLayout::setupBankStore()
{
  if(getStoreModeData() == nullptr)
  {
    setStoreModeData(std::make_unique<StoreModeData>());
  }
  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new InvertedLabel("Store", Rect(8, 26, 48, 12)))->setHighlight(true);
  addControl(new AnyParameterLockedIndicator(Rect(244, 2, 10, 11)));

  addControl(new UndoIndicator(Rect(22, 15, 10, 8)));
  m_menu = addControl(new AppendOverwriteInsertButtonMenu(*this, Rect(195, 1, 58, 62)));
  m_presets = addControl(new PresetListSelectStorePosition(Rect(64, 0, 128, 63), true, getStoreModeData()));
  m_presets->setBankFocus();
}

void PresetManagerLayout::setupPresetFocus()
{
  m_bankButton = addControl(new BankButton({ 3, 36, 58, 57 }, false));

  switch(m_focusAndMode.mode)
  {
    case UIMode::Edit:
      setupPresetEdit();
      break;

    case UIMode::Select:
      setupPresetSelect();
      break;

    case UIMode::Store:
      setupPresetStore();
      break;
  }
}

void PresetManagerLayout::setupPresetEdit()
{
  if(getStoreModeData() != nullptr)
  {
    setStoreModeData(nullptr);
  }
  auto selectedBank = Application::get().getPresetManager()->getSelectedBank();
  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new InvertedLabel("Edit", Rect(8, 26, 48, 12)))->setHighlight(true);
  m_presets = addControl(new PresetList(Rect(64, 0, 128, 63), true));

  if(selectedBank && (!selectedBank->empty() || Application::get().getClipboard()->hasContent()))
  {
    m_menu = addControl(new PresetEditButtonMenu(Rect(195, 1, 58, 62)));
  }
  else
  {
    addControl(new Button("", Buttons::BUTTON_D));
  }
}

void PresetManagerLayout::setupPresetSelect()
{
  auto hwui = Application::get().getHWUI();
  auto vgManager = Application::get().getVGManager();
  if(getStoreModeData() != nullptr)
  {
    setStoreModeData(nullptr);
  }

  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new NumPresetsInBankLabel(Rect(192, 1, 64, 14)));
  addControl(new AnyParameterLockedIndicator(Rect(244, 2, 10, 11)));
  m_loadMode = addControl(new LoadModeMenu(Rect(195, 36, 58, 62)));

  auto isDualEditBuffer = Application::get().getPresetManager()->getEditBuffer()->getType() != SoundType::Single;

  if(vgManager->isInLoadToPart() && isDualEditBuffer)
    m_presets
        = addControl(new LoadToPartPresetList(Rect(64, 0, 128, 63), true, vgManager->getPresetPartSelection(VoiceGroup::I),
                                              vgManager->getPresetPartSelection(VoiceGroup::II)));
  else
    m_presets = addControl(new PresetList(Rect(64, 0, 128, 63), true));

  addControl(new VoiceGroupIndicator(Rect(2, 15, 16, 16), true));
  addControl(new UndoIndicator(Rect(22, 15, 10, 8)));
  m_changedIndicator = addControl(new ChangedParameterIndicator(Rect(0, 14, 64, 14)));
  m_changedIndicator->setVisible(hwui->isModifierSet(ButtonModifier::SHIFT));
}

void PresetManagerLayout::setupPresetStore()
{
  if(getStoreModeData() == nullptr)
  {
    setStoreModeData(std::make_unique<StoreModeData>());
  }
  addControl(new BankAndPresetNumberLabel(Rect(0, 0, 64, 14)));
  addControl(new InvertedLabel("Store", Rect(8, 26, 48, 12)))->setHighlight(true);
  addControl(new AnyParameterLockedIndicator(Rect(244, 2, 10, 11)));

  addControl(new UndoIndicator(Rect(22, 15, 10, 8)));
  m_presets = addControl(new PresetListSelectStorePosition(Rect(64, 0, 128, 63), true, getStoreModeData()));
  m_menu = addControl(new AppendOverwriteInsertButtonMenu(*this, Rect(195, 1, 58, 62)));
}

bool PresetManagerLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(m_changedIndicator)
    m_changedIndicator->setVisible(modifiers[ButtonModifier::SHIFT]);

  if(m_loadMode)
    if(m_loadMode->onButton(i, down, modifiers))
      return true;

  if(m_bankButton)
    if(m_bankButton->onButton(i, down, modifiers))
      return true;

  if(!down)
  {
    removeButtonRepeat();
  }

  if(down)
  {
    auto &app = Application::get();
    SettingsUseCases useCases(*Application::get().getSettings());

    switch(i)
    {
      case Buttons::BUTTON_A:
        if(m_focusAndMode.focus == UIFocus::Banks)
          useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets));
        else
          useCases.setFocusAndMode(FocusAndMode(UIFocus::Banks));
        break;

      case Buttons::BUTTON_B:
      case Buttons::BUTTON_C:
        m_presets->onButton(i, down, modifiers);

        return true;

      case Buttons::BUTTON_D:
        if(m_menu)
        {
          if(modifiers[SHIFT] == 1)
          {
            m_menu->antiToggle();
          }
          else
          {
            m_menu->toggle();
          }
        }

        return true;

      case Buttons::BUTTON_STORE:
        if(m_focusAndMode.mode == UIMode::Store)
          useCases.setFocusAndMode(m_oldFocusAndMode);
        else
          useCases.setFocusAndMode({ UIFocus::Presets, UIMode::Store, UIDetail::Init });
        break;

      case Buttons::BUTTON_EDIT:
        if(m_focusAndMode.mode == UIMode::Edit)
          useCases.setFocusAndMode(UIMode::Select);
        else
          useCases.setFocusAndMode(UIMode::Edit);
        break;

      case Buttons::BUTTON_PRESET:
        if(m_focusAndMode.mode == UIMode::Store)
          useCases.setFocusAndMode({ UIMode::Select });
        else
          useCases.setFocusAndMode(m_oldFocusAndMode);
        break;

      case Buttons::BUTTON_INFO:
        useCases.setFocusAndMode(UIMode::Info);
        break;

      case Buttons::BUTTON_ENTER:
        if(m_menu)
          m_menu->doAction();
        else if(Application::get().getVGManager()->isInLoadToPart())
        {
          return m_presets->onButton(i, down, modifiers);
        }
        else if(m_focusAndMode.mode == UIMode::Select)
        {
          auto pm = Application::get().getPresetManager();
          EditBufferUseCases useCases(*pm->getEditBuffer());

          auto oldPreset = pm->getEditBuffer()->getUUIDOfLastLoadedPreset();

          if(auto currentSelectedPreset = pm->getSelectedPreset())
          {
            useCases.load(currentSelectedPreset);
            if(currentSelectedPreset->getUuid() == oldPreset)
            {
              animateSomePreset(currentSelectedPreset, [] {});
            }
          }
        }
    }
  }

  return super::onButton(i, down, modifiers);
}

bool PresetManagerLayout::onRotary(int inc, ButtonModifiers modifiers)
{
  m_presets->onRotary(inc, modifiers);
  return Layout::onRotary(inc, modifiers);
}

bool PresetManagerLayout::animateSomePreset(Preset *preset, std::function<void()> cb)
{
  return m_presets->animatePreset(preset, std::move(cb));
}

void PresetManagerLayout::animateSelectedPresetIfInLoadPartMode(std::function<void()> cb)
{
  auto pm = getPresetManager();
  auto selPreset = pm->getSelectedPreset();
  if(Application::get().getVGManager()->isInLoadToPart() && selPreset)
    m_presets->animatePreset(selPreset, std::move(cb));
  else
    cb();
}

std::pair<size_t, size_t> PresetManagerLayout::getSelectedPosition() const
{
  if(m_presets)
    return m_presets->getSelectedPosition();

  return {};
}

std::unique_ptr<StoreModeData> &PresetManagerLayout::getStoreModePtr()
{
  static std::unique_ptr<StoreModeData> s_storeModeData;
  return s_storeModeData;
}

StoreModeData *PresetManagerLayout::getStoreModeData()
{

  return getStoreModePtr().get();
}

void PresetManagerLayout::setStoreModeData(std::unique_ptr<StoreModeData> ptr)
{
  getStoreModePtr() = std::move(ptr);
}
