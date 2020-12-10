#include <proxies/hwui/TextCropper.h>
#include "BankNameLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include <presets/Bank.h>
#include <proxies/hwui/FrameBuffer.h>

BankNameLabel::BankNameLabel(const Rect &pos)
    : super(pos)
{
  Application::get().getPresetManager()->onMidiBankSelectionHappened(
      sigc::mem_fun(this, &BankNameLabel::onMidiSelectionHappened));
}

BankNameLabel::~BankNameLabel()
{
}

void BankNameLabel::updateLabel(Bank *newBank)
{
  if(newBank)
  {
    auto pm = Application::get().getPresetManager();
    auto bankName = newBank->getName(true);
    lastBank = newBank;
    auto pos = pm->getBankPosition(newBank->getUuid()) + 1;
    auto s = to_string(pos) + ": " + bankName;
    setText({ s });
  }
  else
  {
    setText("");
  }
}

Label::StringAndSuffix BankNameLabel::getText() const
{
  auto t = Label::getText();
  if(isMidiBank())
  {
    return addMidiSuffix(t);
  }

  return t;
}

void BankNameLabel::onMidiSelectionHappened(const Uuid &uuid)
{
  if(auto midiBank = Application::get().getPresetManager()->findMidiSelectedBank())
  {
    updateLabel(midiBank);
  }
}

Label::StringAndSuffix BankNameLabel::shortenStringIfNeccessary(std::shared_ptr<Font> font,
                                                                const StringAndSuffix &text) const
{
  auto availableWidth = getPosition().getWidth() - 2 * getXOffset();
  return TextCropper::shortenStringIfNeccessary(font, text.text, availableWidth);
}

void BankNameLabel::setBackgroundColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C255);
  else
    fb.setColor(FrameBufferColors::C103);
}

void BankNameLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C43);
}

Font::Justification BankNameLabel::getJustification() const
{
  return Font::Justification::Center;
}

int BankNameLabel::getXOffset() const
{
  return 2;
}

bool BankNameLabel::isMidiBank() const
{
  return lastBank && Application::get().getPresetManager()->findMidiSelectedBank() == lastBank;
}

Label::StringAndSuffix BankNameLabel::addMidiSuffix(Label::StringAndSuffix text) const
{
  return { text.text + " ^", text.suffix };
}
