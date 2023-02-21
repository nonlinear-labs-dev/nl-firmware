#include "PresetPropertyDisplay.h"
#include "proxies/hwui/controls/Label.h"

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
  auto totalWidth = rect.getWidth();
  auto oneQuarter = totalWidth / 4;
  m_type->setPosition({ 0, 0, oneQuarter, rect.getHeight() });
  m_unison->setPosition({ 0 + oneQuarter, 0, oneQuarter, rect.getHeight() });
  m_mono->setPosition({ 0 + oneQuarter * 2, 0, oneQuarter, rect.getHeight() });
  m_scale->setPosition({ 0 + oneQuarter * 3, 0, oneQuarter, rect.getHeight() });
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
}
