#include <Application.h>
#include <libundo/undo/Scope.h>
#include <http/UndoScope.h>
#include <libundo/undo/Transaction.h>
#include <libundo/undo/TransactionCreationScope.h>
#include <presets/Preset.h>
#include <presets/Bank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/EditPresetInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenamePresetLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <clipboard/Clipboard.h>

int PresetEditButtonMenu::s_lastSelection = PresetEditButtonMenu::Actions::Rename;

PresetEditButtonMenu::PresetEditButtonMenu(const Rect &rect)
    : super(rect)
{
  Application::get().getClipboard()->onClipboardChanged(mem_fun(this, &PresetEditButtonMenu::onClipboardChanged));
}

PresetEditButtonMenu::~PresetEditButtonMenu()
{
}

void PresetEditButtonMenu::onClipboardChanged()
{
  clear();
  clearActions();

  addButton("Rename", bind(&PresetEditButtonMenu::renamePreset, this));
  addButton("Cut", bind(&PresetEditButtonMenu::cutPreset, this));
  addButton("Copy", bind(&PresetEditButtonMenu::copyPreset, this));

  if(Application::get().getClipboard()->hasContent())
  {
    addButton("Paste", bind(&PresetEditButtonMenu::pasteClipboard, this));
  }

  addButton("Delete", bind(&PresetEditButtonMenu::deletePreset, this));
  selectButton(s_lastSelection);
}

void PresetEditButtonMenu::selectButton(size_t i)
{
  super::selectButton(i);
  s_lastSelection = i;
}

void PresetEditButtonMenu::renamePreset()
{
  auto layout = new RenamePresetLayout([=](const Glib::ustring &newName) {
    if(auto bank = Application::get().getPresetManager()->getSelectedBank())
    {
      if(auto preset = bank->getSelectedPreset())
      {
        auto scope = Application::get().getUndoScope()->startTransaction("Rename Preset");
        preset->setName(scope->getTransaction(), newName);
      }
    }
  });

  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(layout);
}

void PresetEditButtonMenu::cutPreset()
{
  auto pm = Application::get().getPresetManager();
  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->cutPreset(bank->getSelectedPresetUuid());
    selectButton(Paste);
    m_lastCopyingAction = Cut;
  }
}

void PresetEditButtonMenu::copyPreset()
{
  auto pm = Application::get().getPresetManager();
  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->copyPreset(bank->getSelectedPresetUuid());
    selectButton(Paste);
    m_lastCopyingAction = Copy;
  }
}

void PresetEditButtonMenu::deletePreset()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    if(auto preset = bank->getSelectedPreset())
    {
      auto scope = bank->getUndoScope().startTransaction("Delete preset '%0'", preset->getName());
      bank->deletePreset(scope->getTransaction(), preset->getUuid());
    }
  }
}

void PresetEditButtonMenu::pasteClipboard()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    if(auto selctedPreset = bank->getSelectedPreset())
      Application::get().getClipboard()->pasteOnPreset(selctedPreset->getUuid());

    selectButton(m_lastCopyingAction);
  }
}
