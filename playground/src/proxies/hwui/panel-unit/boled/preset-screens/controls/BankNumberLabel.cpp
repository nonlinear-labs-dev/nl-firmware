#include "BankNumberLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/Bank.h"
#include <proxies/hwui/FrameBuffer.h>

BankNumberLabel::BankNumberLabel(const Rect &pos)
    : super(pos)
{
  Application::get().getPresetManager()->onBankSelection(sigc::mem_fun(this, &BankNumberLabel::onBankSelectionChanged));
}

BankNumberLabel::~BankNumberLabel()
{
}

void BankNumberLabel::onBankSelectionChanged(const Uuid &selectedBank)
{
  if(auto newBank = Application::get().getPresetManager()->getSelectedBank())
    setText({ to_string(Application::get().getPresetManager()->getBankPosition(newBank->getUuid()) + 1) });
  else
    setText("");
}

void BankNumberLabel::setBackgroundColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C255);
  else
    fb.setColor(FrameBufferColors::C103);
}

void BankNumberLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

Font::Justification BankNumberLabel::getJustification() const
{
  return Font::Justification::Right;
}
