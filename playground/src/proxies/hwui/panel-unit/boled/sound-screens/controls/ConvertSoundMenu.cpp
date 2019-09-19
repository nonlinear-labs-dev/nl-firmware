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

void ConvertSoundMenu::convertSoundTo(EditBufferType newType)
{
  Application::get().getPresetManager()->getEditBuffer()->setType(newType);
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().bruteForce();
}

void ConvertSoundMenu::setup()
{
  clear();

  auto type = Application::get().getPresetManager()->getEditBuffer()->getType();

  if(type != EditBufferType::Single)
    addButton("Single", std::bind(&ConvertSoundMenu::convertSoundTo, this, EditBufferType::Single));

  if(type != EditBufferType::Split)
    addButton("Split", std::bind(&ConvertSoundMenu::convertSoundTo, this, EditBufferType::Split));

  if(type != EditBufferType::Layer)
    addButton("Layer", std::bind(&ConvertSoundMenu::convertSoundTo, this, EditBufferType::Layer));

  sanitizeIndex();
  selectButton(getSelectedButton());
}
