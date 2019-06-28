#include <proxies/hwui/TextCropper.h>
#include "BankNameLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include <presets/Bank.h>

BankNameLabel::BankNameLabel(const Rect &pos)
    : super(pos)
{
}

BankNameLabel::~BankNameLabel()
{
}

void BankNameLabel::updateLabel(Bank *newBank)
{
  if(newBank)
  {
    auto bankName = newBank->getName(true);
    auto pos = Application::get().getPresetManager()->getBankPosition(newBank->getUuid()) + 1;
    auto s = to_string(pos) + ": " + bankName;
    setText({ s });
  }
  else
  {
    setText("");
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
    fb.setColor(FrameBuffer::Colors::C255);
  else
    fb.setColor(FrameBuffer::Colors::C103);
}

void BankNameLabel::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBuffer::Colors::C43);
}

Font::Justification BankNameLabel::getJustification() const
{
  return Font::Justification::Center;
}

int BankNameLabel::getXOffset() const
{
  return 2;
}
