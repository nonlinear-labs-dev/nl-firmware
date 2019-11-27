#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Button.h>
#include <nltools/Types.h>
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
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToDual(transaction, newType);
    Application::get().getHWUI()->setFocusAndMode(FocusAndMode{ UIFocus::Sound, UIMode::Select, UIDetail::Init });
  }
  else
  {
    auto scope = pm->getUndoScope().startTransaction("Convert to Single");
    auto transaction = scope->getTransaction();
    auto copyFromVoiceGroup = Application::get().getHWUI()->getCurrentVoiceGroup();
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToSingle(transaction, copyFromVoiceGroup);
    Application::get().getHWUI()->setFocusAndMode(FocusAndMode{ UIFocus::Sound, UIMode::Select, UIDetail::Init });
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
