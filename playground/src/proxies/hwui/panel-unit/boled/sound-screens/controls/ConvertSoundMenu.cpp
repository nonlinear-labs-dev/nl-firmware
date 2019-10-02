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
  if(newType != SoundType::Single)
  {
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToType(newType);
  }
  else
  {
    Application::get().getPresetManager()->getEditBuffer()->undoableConvertToType(
        SoundType::Single, Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection());
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
