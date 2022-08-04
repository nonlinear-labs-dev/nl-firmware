#include <Application.h>
#include <clipboard/Clipboard.h>
#include <libundo/undo/Scope.h>
#include <http/UndoScope.h>
#include <presets/BankActions.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenameBankLayout.h>
#include <tools/FileSystem.h>
#include <serialization/PresetBankSerializer.h>
#include <xml/FileOutStream.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/panel-unit/boled/file/RenameExportLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/USBStickAvailableView.h>
#include <proxies/hwui/panel-unit/boled/file/FileDialogLayout.h>
#include <device-info/DateTimeInfo.h>
#include <xml/VersionAttribute.h>
#include <tools/TimeTools.h>
#include <nltools/GenericScopeGuard.h>
#include <device-settings/Settings.h>
#include "use-cases/SettingsUseCases.h"
#include "use-cases/PresetManagerUseCases.h"

static size_t s_lastSelectedButton = 0;

BankEditButtonMenu::BankEditButtonMenu(const Rect& rect)
    : super(rect)
{
  Application::get().getClipboard()->onClipboardChanged(
      mem_fun(this, &BankEditButtonMenu::rebuildMenuOnClipboardChange));
  Application::get().getPresetManager()->onNumBanksChanged(mem_fun(this, &BankEditButtonMenu::rebuildMenu));

  Application::get().getPresetManager()->onMidiBankSelectionHappened(
      mem_fun(this, &BankEditButtonMenu::onMidiSelectionChanged));
}

void BankEditButtonMenu::rebuildMenuOnClipboardChange()
{
  rebuildMenu(Application::get().getPresetManager()->getNumBanks());
}

void BankEditButtonMenu::rebuildMenu(size_t numBanks)
{
  clear();

  if(numBanks != 0)
    rebuildFullMenu();
  else
    rebuildNoBankAvailableMenu();

  correctMenuSelection();
  selectButton(s_lastSelectedButton);
}

void BankEditButtonMenu::onMidiSelectionChanged(Uuid bankUuid)
{
  auto numbanks = Application::get().getPresetManager()->getNumBanks();
  rebuildMenu(numbanks);
}

void BankEditButtonMenu::rebuildFullMenu()
{
  addButton("New", std::bind(&BankEditButtonMenu::newBank, this));

  if(USBStickAvailableView::usbIsReady())
  {
    addButton("Export ...", std::bind(&BankEditButtonMenu::exportBank, this));
    addButton("Import ...", std::bind(&BankEditButtonMenu::importBank, this));
  }

  addButton("Rename", std::bind(&BankEditButtonMenu::renameBank, this));
  addButton("Copy", std::bind(&BankEditButtonMenu::copyBank, this));

  if(Application::get().getClipboard()->hasContent())
  {
    addButton("Paste", std::bind(&BankEditButtonMenu::pasteBank, this));
  }

  addButton("Delete", std::bind(&BankEditButtonMenu::deleteBank, this));

  if(Application::get().getPresetManager()->findMidiSelectedBank()
     != Application::get().getPresetManager()->getSelectedBank())
  {
    addButton("MIDI PC: On", std::bind(&BankEditButtonMenu::selectMidi, this));
  }
  else
  {
    addButton("MIDI PC: Off", std::bind(&BankEditButtonMenu::removeMidi, this));
  }

  addButton("Move Left", std::bind(&BankEditButtonMenu::moveLeft, this));
  addButton("Move Right", std::bind(&BankEditButtonMenu::moveRight, this));
}

void BankEditButtonMenu::rebuildNoBankAvailableMenu()
{

  addButton("New", std::bind(&BankEditButtonMenu::newBank, this));

  if(USBStickAvailableView::usbIsReady())
  {
    addButton("Import ...", std::bind(&BankEditButtonMenu::importBank, this));
  }

  if(Application::get().getClipboard()->hasContent())
  {
    addButton("Paste", std::bind(&BankEditButtonMenu::pasteBank, this));
  }
}

void BankEditButtonMenu::selectButton(size_t i)
{
  s_lastSelectedButton = i;
  super::selectButton(i);
}

void BankEditButtonMenu::newBank()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCases(*pm, *settings);
  auto pos = pm->calcDefaultBankPositionForNewBank();
  useCases.newBank(std::to_string(pos.first), std::to_string(pos.second), std::nullopt);

  auto layout = new RenameBankLayout();
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(layout);
}

bool BankEditButtonMenu::applicableBackupFilesFilter(const std::filesystem::directory_entry& term)
{
  auto fileName = term.path().filename().string();
  std::string end = ".xml";
  return !std::equal(end.rbegin(), end.rend(), fileName.rbegin());
}

void BankEditButtonMenu::importBankFromPath(const std::filesystem::directory_entry& file)
{
  auto hwui = Application::get().getHWUI();

  if(file != std::filesystem::directory_entry())
  {
    hwui->startSplash();
    auto pm = Application::get().getPresetManager();
    auto settings = Application::get().getSettings();
    PresetManagerUseCases useCase(*pm, *settings);
    useCase.importBankFromPath(file, [hwui](const std::string& name) { hwui->addSplashStatus("Importing " + name); });
    hwui->finishSplash();
  }

  hwui->getPanelUnit().getEditPanel().getBoled().resetOverlay();
  SettingsUseCases useCases(*Application::get().getSettings());
  useCases.setFocusAndMode(FocusAndMode{ UIFocus::Presets, UIMode::Select });
}

void BankEditButtonMenu::importBank()
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(
      new FileDialogLayout(&BankEditButtonMenu::applicableBackupFilesFilter, &BankEditButtonMenu::importBankFromPath,
                           "Select Bank for Import"));
}

Glib::ustring BankEditButtonMenu::createValidOutputPath(const Glib::ustring& bankName)
{
  auto fileName = FileSystem::findSuitableFileName(bankName, "/mnt/usb-stick/", 0);
  return "/mnt/usb-stick/" + fileName + ".xml";
}

void BankEditButtonMenu::exportBank()
{
  auto& panelunit = Application::get().getHWUI()->getPanelUnit();
  auto& boled = panelunit.getEditPanel().getBoled();

  if(auto selBank = Application::get().getPresetManager()->getSelectedBank())
  {
    boled.setOverlay(new RenameExportLayout(selBank, [](Glib::ustring newExportName, auto bank) {
      auto hwui = Application::get().getHWUI();
      auto& panelunit = hwui->getPanelUnit();
      auto& boled = panelunit.getEditPanel().getBoled();
      auto outPath = BankEditButtonMenu::createValidOutputPath(newExportName);
      boled.resetOverlay();
      boled.setOverlay(new SplashLayout(hwui));
      BankEditButtonMenu::writeSelectedBankToFile(bank, outPath);
      boled.resetOverlay();
    }));
  }
}

void BankEditButtonMenu::writeSelectedBankToFile(Bank* selBank, const std::string& outFile)
{
  if(selBank)
  {
    Application::get().getHWUI()->addSplashStatus("Exporting " + selBank->getName(true));
    BankUseCases useCase(selBank, *Application::get().getSettings());
    useCase.exportBankToFile(outFile);
  }
}

void BankEditButtonMenu::renameBank()
{
  auto layout = new RenameBankLayout();
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(layout);
}

void BankEditButtonMenu::copyBank()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->copyBank(bank->getUuid());
  }
}

void BankEditButtonMenu::pasteBank()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->pasteOnBank(bank->getUuid());
  }
}

void BankEditButtonMenu::deleteBank()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  if(auto bank = pm->getSelectedBank())
  {
    useCase.deleteBank(bank);
  }

  SettingsUseCases useCases(*Application::get().getSettings());
  useCases.setFocusAndMode(FocusAndMode(UIFocus::Banks, UIMode::Select));
}

void BankEditButtonMenu::moveLeft()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  if(auto bank = pm->getSelectedBank())
    useCase.moveLeft(bank);
}

void BankEditButtonMenu::moveRight()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*pm, *settings);

  if(auto bank = pm->getSelectedBank())
    useCase.moveRight(bank);
}

void BankEditButtonMenu::selectMidi()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  auto bank = pm->getSelectedBank();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.selectMidiBank(bank);
}

void BankEditButtonMenu::removeMidi()
{
  auto pm = Application::get().getPresetManager();
  auto settings = Application::get().getSettings();
  PresetManagerUseCases useCase(*pm, *settings);
  useCase.selectMidiBank(nullptr);
}

void BankEditButtonMenu::correctMenuSelection()
{
  if(s_lastSelectedButton >= getItemCount())
    s_lastSelectedButton = 0;
}
