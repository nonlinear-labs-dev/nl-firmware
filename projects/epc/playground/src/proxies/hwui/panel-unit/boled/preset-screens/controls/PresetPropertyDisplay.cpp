#include "PresetPropertyDisplay.h"
#include "proxies/hwui/controls/Label.h"
#include <proxies/hwui/Font.h>

PresetPropertyDisplay::PresetPropertyDisplay()
    : ControlWithChildren({ 0, 0, 0, 0 })
{
  m_type = addControl(new Label({ 0, 0, 0, 0 }));
  m_unison = addControl(new Label({ 0, 0, 0, 0 }));
  m_mono = addControl(new Label({ 0, 0, 0, 0 }));
  m_scale = addControl(new Label({ 0, 0, 0, 0 }));

  for(auto l : { m_type, m_unison, m_mono, m_scale })
  {
    l->setJustification(Font::Justification::Left);
  }
}

void PresetPropertyDisplay::setPosition(const Rect &rect)
{
  Control::setPosition(rect);
  repositionLabels();
}

void PresetPropertyDisplay::repositionLabels()
{
  auto rect = getPosition();

  auto margin = 2;

  auto to_int = [](auto v) { return static_cast<int>(v); };

  auto fontW = to_int(m_type->getFont()->getStringWidth(m_type->getText().text));
  auto unisonW = to_int(m_unison->getFont()->getStringWidth(m_unison->getText().text));
  auto monoW = to_int(m_mono->getFont()->getStringWidth(m_mono->getText().text));
  auto scaleW = to_int(m_scale->getFont()->getStringWidth(m_scale->getText().text));

  int xPos = 0;
  auto height = rect.getHeight();

  m_type->setPosition({ xPos, 0, fontW, height });
  xPos += (fontW + margin);
  m_unison->setPosition({ xPos, 0, unisonW, height });
  xPos += (unisonW + margin);
  m_mono->setPosition({ xPos, 0, monoW, height });
  xPos += (monoW + margin);
  m_scale->setPosition({ xPos, 0, scaleW, height });
}

void PresetPropertyDisplay::updateFrom(const Glib::ustring &text)
{
  auto highlightColor = FrameBufferColors::C128;
  auto lowlightColor = FrameBufferColors::C77;

  if(!text.empty())
  {
    auto splits = StringTools::splitStringOnAnyDelimiter(text, ',');
    assert(splits.size() == 4);
    m_type->setText(splits[0]);
    m_unison->setText(splits[1]);
    m_mono->setText(splits[2]);
    m_scale->setText(splits[3]);
    //set colors based on 'defaults'
    auto defaults = Preset::getDefaultProperties();
    auto defaultSplits = StringTools::splitStringOnAnyDelimiter(defaults, ',');
    assert(defaultSplits.size() == 4);

    m_type->setFontColor(defaultSplits[0] != splits[0] ? highlightColor : lowlightColor);
    m_unison->setFontColor(defaultSplits[1] != splits[1] ? highlightColor : lowlightColor);
    m_mono->setFontColor(defaultSplits[2] != splits[2] ? highlightColor : lowlightColor);
    m_scale->setFontColor(defaultSplits[3] != splits[3] ? highlightColor : lowlightColor);
  }
  else
  {
    m_type->setText("");
    m_unison->setText("");
    m_mono->setText("");
    m_scale->setText("");

    m_type->setFontColor(highlightColor);
    m_unison->setFontColor(highlightColor);
    m_mono->setFontColor(highlightColor);
    m_scale->setFontColor(highlightColor);
  }

  repositionLabels();
}
