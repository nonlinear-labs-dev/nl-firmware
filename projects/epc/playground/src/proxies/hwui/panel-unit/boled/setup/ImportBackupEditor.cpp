#include <Application.h>
#include <glibmm/ustring.h>
#include <http/UndoScope.h>
#include <libundo/undo/Transaction.h>
#include <libundo/undo/TransactionCreationScope.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/Point.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/ImportBackupEditor.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <serialization/PresetManagerSerializer.h>
#include <xml/FileInStream.h>
#include <presets/EditBuffer.h>
#include <xml/XmlReader.h>
#include <xml/VersionAttribute.h>
#include <memory>
#include <thread>
#include "USBStickAvailableView.h"
#include "device-settings/DebugLevel.h"
#include "use-cases/PresetManagerUseCases.h"
#include <tools/FileSystem.h>
#include <proxies/hwui/panel-unit/boled/file/FileDialogLayout.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <use-cases/SettingsUseCases.h>

static const Rect c_fullRightSidePosition(129, 16, 126, 48);

ImportBackupEditor::ImportBackupEditor()
    : ControlWithChildren(Rect(0, 0, 0, 0))
{
  if(USBStickAvailableView::usbIsReady())
  {
    addLabel("Attention! This action will delete all current banks!");
    addControl(new Button("OK", Button::getButtonPos(Buttons::BUTTON_C).getMovedBy(Point(-128, -16))));
    addControl(new Button("Cancel", Button::getButtonPos(Buttons::BUTTON_D).getMovedBy(Point(-128, -16))));
  }
  else
  {
    addControl(new MultiLineLabel("Please insert an USB-Stick and reload this page!"))
        ->setPosition(Rect(3, 0, 119, 24));
    addControl(new Button("Back", Button::getButtonPos(Buttons::BUTTON_D).getMovedBy(Point(-128, -16))));
  }
}

ImportBackupEditor::~ImportBackupEditor() = default;

void ImportBackupEditor::addLabel(const Glib::ustring &text)
{
  addControl(new MultiLineLabel(text))->setPosition(Rect(3, 0, 119, 24));
}

void ImportBackupEditor::setPosition(const Rect &)
{
  ControlWithChildren::setPosition(c_fullRightSidePosition);
}

bool ImportBackupEditor::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  if(down)
  {
    if(i == Buttons::BUTTON_C && USBStickAvailableView::usbIsReady())
    {
      importBackup();
      return true;
    }
    else if(i == Buttons::BUTTON_D)
    {
      diveUp();
      return true;
    }
  }
  return false;
}

Glib::ustring ImportBackupEditor::generateFileDialogCompliantNameFromPath(std::filesystem::directory_entry file)
{
  auto nameWithoutZipAtTheEnd = file.path().stem().string();
  auto parentPathToFile = file.path().parent_path().string();
  return parentPathToFile + "/" + nameWithoutZipAtTheEnd;
}

bool ImportBackupEditor::filterApplicableFileNames(std::filesystem::directory_entry term)
{
  auto fileName = term.path().filename().string();
  std::string endA = ".xml.zip";
  std::string endB = ".xml.tar.gz";
  return !(std::equal(endA.rbegin(), endA.rend(), fileName.rbegin())
           || std::equal(endB.rbegin(), endB.rend(), fileName.rbegin()));
}

void ImportBackupEditor::importBackupFileFromPath(std::filesystem::directory_entry file)
{
  using namespace std::chrono_literals;
  auto &app = Application::get();
  auto hwui = app.getHWUI();
  auto &boled = hwui->getPanelUnit().getEditPanel().getBoled();
  auto settings = app.getSettings();

  if(file != std::filesystem::directory_entry())
  {
    auto path = generateFileDialogCompliantNameFromPath(file);

    FileInStream in(path, true);

    boled.setOverlay(new SplashLayout(hwui));
    hwui->addSplashStatus("Restoring Backup from File!");

    PresetManagerUseCases useCase(*app.getPresetManager(), *settings);
    auto& ae = *app.getAudioEngineProxy();

    auto start = [hwui](){
      hwui->startSplash();
    };

    auto addStatus = [hwui](auto str){
      hwui->addSplashStatus(str);
    };

    auto finish = [hwui](){
      hwui->finishSplash();
    };

    auto ret = useCase.importBackupFile(in, { start, addStatus, finish }, ae);
    if(ret ==  PresetManagerUseCases::ImportExitCode::OK)
    {
      hwui->setSplashStatus("Restore Complete!");
      std::this_thread::sleep_for(0.7s);
    }
    else
    {
      hwui->setSplashStatus(PresetManagerUseCases::exitCodeToErrorMessage(ret));
      std::this_thread::sleep_for(2s);
    }
  }

  SettingsUseCases useCases(*Application::get().getSettings());
  useCases.setFocusAndMode(FocusAndMode{ UIFocus::Presets, UIMode::Select });
}

void ImportBackupEditor::importBackup()
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(
      new FileDialogLayout(&ImportBackupEditor::filterApplicableFileNames,
                           &ImportBackupEditor::importBackupFileFromPath, "Select Backup for Restore"));
}
