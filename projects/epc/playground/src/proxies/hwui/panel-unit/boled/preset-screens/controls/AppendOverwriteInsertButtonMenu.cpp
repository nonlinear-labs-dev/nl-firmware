#include "AppendOverwriteInsertButtonMenu.h"
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenamePresetLayout.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include "Application.h"
#include "device-settings/Settings.h"
#include "use-cases/SettingsUseCases.h"
#include "use-cases/PresetManagerUseCases.h"
#include "use-cases/BankUseCases.h"
#include <presets/PresetManager.h>
#include <presets/Bank.h>
#include <presets/Preset.h>
#include <presets/EditBuffer.h>
#include <http/UndoScope.h>
#include <use-cases/PresetUseCases.h>

AppendOverwriteInsertButtonMenu::AppendOverwriteInsertButtonMenu(PresetManagerLayout& parent, const Rect& rect)
    : super(rect)
    , m_parent(parent)
{
  m_prestmanager = Application::get().getPresetManager();
  m_prestmanager->onNumBanksChanged(sigc::hide<0>(mem_fun(this, &AppendOverwriteInsertButtonMenu::buildMenu)));
}

void AppendOverwriteInsertButtonMenu::buildMenu()
{
  clear();

  addButton("Append", std::bind(&AppendOverwriteInsertButtonMenu::executeAction, this));
  if(m_prestmanager->getSelectedBank() && m_prestmanager->getSelectedBank()->getNumPresets() != 0)
    addButton("Overwrite", std::bind(&AppendOverwriteInsertButtonMenu::executeAction, this));
  addButton("Insert", std::bind(&AppendOverwriteInsertButtonMenu::executeAction, this));

  auto setting = Application::get().getSettings()->getSetting<PresetStoreModeSetting>()->get();
  selectButton(enumToIndex(setting));
}

AppendOverwriteInsertButtonMenu::~AppendOverwriteInsertButtonMenu() = default;

void AppendOverwriteInsertButtonMenu::selectButton(size_t i)
{
  super::selectButton(i);
  Application::get().getSettings()->getSetting<PresetStoreModeSetting>()->set(indexToEnum(i));
}

PresetStoreModeSettings AppendOverwriteInsertButtonMenu::indexToEnum(size_t i) const
{
  return static_cast<PresetStoreModeSettings>(i);
}

size_t AppendOverwriteInsertButtonMenu::enumToIndex(PresetStoreModeSettings i) const
{
  return static_cast<size_t>(i);
}

bool AppendOverwriteInsertButtonMenu::animateSelectedPreset()
{
  if(auto selBank = m_prestmanager->getSelectedBank())
  {
    if(auto selPreset = selBank->getSelectedPreset())
    {
      return animatePreset(selPreset);
    }
  }
  return false;
}

bool AppendOverwriteInsertButtonMenu::animatePreset(Preset* target)
{
  auto hwuiPtr = Application::get().getHWUI();
  return m_parent.animateSomePreset(
      target,
      []
      {
        SettingsUseCases useCases(*Application::get().getSettings());
        useCases.setFocusAndMode(FocusAndMode { UIFocus::Presets, UIMode::Select });
      });
}

void AppendOverwriteInsertButtonMenu::executeAction()
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCases(*pm, *settings);
  auto actionPosition = m_parent.getSelectedPosition();

  if(auto selectedBank = pm->getBankAt(actionPosition.first))
  {
    BankUseCases bankUseCases(selectedBank, *settings);
    if(auto selectedPreset = selectedBank->getPresetAt(actionPosition.second))
    {
      PresetUseCases presetUseCases(*selectedPreset, *settings);

      Application::get().getUndoScope()->resetCukooTransaction();

      auto setting = Application::get().getSettings()->getSetting<PresetStoreModeSetting>()->get();
      bool modified = pm->getEditBuffer()->isModified();

      switch(setting)
      {
        case PresetStoreModeSettings::PRESET_STORE_MODE_APPEND:
          if(modified)
            pushRenameScreen(bankUseCases.appendEditBuffer());
          else
          {
            auto newPreset = bankUseCases.appendEditBuffer();
            animatePreset(newPreset);
          }
          break;

        case PresetStoreModeSettings::PRESET_STORE_MODE_INSERT:
          if(modified)
            pushRenameScreen(bankUseCases.insertEditBufferAtPosition(
                selectedBank->getPresetPosition(selectedPreset->getUuid()) + 1));
          else
          {
            auto newPreset = bankUseCases.insertEditBufferAtPosition(
                selectedBank->getPresetPosition(selectedPreset->getUuid()) + 1);
            animatePreset(newPreset);
          }
          break;

        case PresetStoreModeSettings::PRESET_STORE_MODE_OVERWRITE:
          presetUseCases.overwriteWithEditBuffer(*eb);
          animateSelectedPreset();
          break;
      }
    }
    else
    {
      auto preset = bankUseCases.insertEditBufferAtPosition(0);
      pushRenameScreen(preset);
    }
  }
  else
  {
    auto newBank = useCases.createBankAndStoreEditBuffer();
    animatePreset(newBank->getPresetAt(0));
  }
}

void AppendOverwriteInsertButtonMenu::pushRenameScreen(Preset* target)
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(new RenamePresetLayout(
      [=](const Glib::ustring& newName)
      {
        PresetUseCases useCases(*target, *Application::get().getSettings());
        useCases.rename(newName);
        animatePreset(target);
      },
      [=]() { animateSelectedPreset(); }));
}
