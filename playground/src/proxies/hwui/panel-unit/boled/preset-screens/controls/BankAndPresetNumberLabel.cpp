#include "BankAndPresetNumberLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include <presets/EditBuffer.h>
#include <presets/Bank.h>
#include <iomanip>
#include <locale>
#include <sstream>

BankAndPresetNumberLabel::BankAndPresetNumberLabel(const Rect &pos)
    : super(pos)
{
  auto pm = Application::get().getPresetManager();
  auto eb = pm->getEditBuffer();
  pm->onNumBanksChanged(sigc::hide<0>(mem_fun(this, &BankAndPresetNumberLabel::onEditBufferChanged)));
  eb->onChange(sigc::mem_fun(this, &BankAndPresetNumberLabel::onEditBufferChanged));
  eb->onModificationStateChanged(sigc::hide<0>(mem_fun(this, &BankAndPresetNumberLabel::onEditBufferChanged)));
}

void BankAndPresetNumberLabel::onEditBufferChanged()
{
  setDirty();
}

bool BankAndPresetNumberLabel::redraw(FrameBuffer &fb)
{
  auto pm = Application::get().getPresetManager();
  auto uuid = pm->getEditBuffer()->getUUIDOfLastLoadedPreset();

  if(auto bank = pm->findBankWithPreset(uuid))
  {
    auto bankPos = pm->getBankPosition(bank->getUuid()) + 1;
    auto presetPos = bank->getPresetPosition(uuid) + 1;
    ostringstream presetPosStr;
    presetPosStr.width(3);
    presetPosStr.fill('0');
    presetPosStr << presetPos;

    auto text = UNDO::StringTools::buildString(bankPos, "-", presetPosStr.str());

    if(pm->getEditBuffer()->isModified())
      text += "*";

    setText(text);
  }
  else
  {
    setText("");
  }

  Label::redraw(fb);
  return true;
}
