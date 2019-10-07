#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include "ConvertSoundMenu.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

ConvertSoundMenu::ConvertSoundMenu(const Rect &rect)
    : ButtonMenu(rect, 2)
{
  setup();
}

void ConvertSoundMenu::convertSoundTo(SoundType newType)
{
  auto pm = Application::get().getPresetManager();
  if(newType != SoundType::Single)
  {
    auto scope = pm->getUndoScope().startTransaction("Convert to " + toString(newType));
    auto transaction = scope->getTransaction();
    auto currentSelection = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToDual(transaction, newType,
                                                                                  currentSelection);
  }
  else
  {
    auto scope = pm->getUndoScope().startTransaction("Convert to Single");
    auto transaction = scope->getTransaction();
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToSingle(transaction);
  }
}

void ConvertSoundMenu::setup()
{
  clear();

  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();

  if(type != SoundType::Single)
    addButton("Single", std::bind(&ConvertSoundMenu::convertSoundTo, this, SoundType::Single));

  if(type != SoundType::Split)
    addButton("Split", std::bind(&ConvertSoundMenu::convertSoundTo, this, SoundType::Split));

  if(type != SoundType::Layer)
    addButton("Layer", std::bind(&ConvertSoundMenu::convertSoundTo, this, SoundType::Layer));

  sanitizeIndex();
  selectButton(getSelectedButton());
}
