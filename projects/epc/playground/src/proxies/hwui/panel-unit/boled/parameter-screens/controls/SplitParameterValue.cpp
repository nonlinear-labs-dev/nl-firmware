#include <proxies/hwui/Oleds.h>
#include <tools/StringTools.h>
#include <nltools/Types.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/FrameBuffer.h>
#include <parameter_declarations.h>
#include <proxies/hwui/controls/PNGControl.h>
#include "SplitParameterValue.h"
#include "proxies/hwui/controls/Label.h"

SplitParameterValue::SplitParameterValue(const Rect& pos)
    : Label(pos)
{
  setFontColor(FrameBufferColors::C179);

  init();
}

SplitParameterValue::SplitParameterValue(const Label::StringAndSuffix& text, const Rect& pos)
    : Label(text, pos)
{
  setFontColor(FrameBufferColors::C179);

  init();
}

void SplitParameterValue::init()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto split1 = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto split2 = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });

  auto pos = getPosition();
  pos.setWidth(9);
  pos.setLeft(pos.getRight() - 9);
  pos.setTop(pos.getTop() + 2);

  overlapIndicator = std::make_shared<PNGControl>(pos, "overlap-a.png");
  overlapIndicator->useImageColors(true);

  splitIConnection = split1->onParameterChanged(sigc::mem_fun(this, &SplitParameterValue::onSplitIChanged));
  splitIIConnection = split2->onParameterChanged(sigc::mem_fun(this, &SplitParameterValue::onSplitIIChanged));
}

void SplitParameterValue::onSplitIChanged(const Parameter* splitI)
{
  m_splitICP = splitI->getControlPositionValue();
  onSplitValuesChanged();
}

void SplitParameterValue::onSplitIIChanged(const Parameter* splitII)
{
  m_splitIICP = splitII->getControlPositionValue();
  onSplitValuesChanged();
}

void SplitParameterValue::onSplitValuesChanged()
{
  auto overlap = m_splitICP >= m_splitIICP;
  overlapIndicator->setVisible(overlap);
}

void SplitParameterValue::drawParts(FrameBuffer& fb, const std::vector<Glib::ustring>& parts)
{
  auto w = 0;
  for(auto& p : parts)
    w += getFont()->getStringWidth(p);

  int x = getPosition().getCenter().getX() - (w / 2);
  int y = getPosition().getCenter().getY() + 4;

  for(auto i = 0; i < parts.size(); i++)
  {
    auto part = parts[i];
    fb.setColor(getColorForSplit(i));
    getFont()->draw(part, x, y);
    x += getFont()->getStringWidth(part);
  }
}

FrameBufferColors SplitParameterValue::getColorForSplit(int i)
{
  VoiceGroup selectedVg = Application::get().getHWUI()->getCurrentVoiceGroup();
  const FrameBufferColors highlighted = FrameBufferColors::C255;

  switch(i)
  {
    case 0:
      return selectedVg == VoiceGroup::I ? highlighted : getFontColor();
    case 2:
      return selectedVg == VoiceGroup::II ? highlighted : getFontColor();
    default:
    case 1:
      return getFontColor();
  }
}

bool SplitParameterValue::redraw(FrameBuffer& fb)
{
  auto text = getText();
  auto splits = text.getSplits();
  auto parts = StringTools::splitStringOnAnyDelimiter(splits.first, '\t');

  if(parts.size() > 1)
  {
    drawParts(fb, parts);
  }
  else
  {
    Label::redraw(fb);
  }

  overlapIndicator->redraw(fb);

  return true;
}

Label::StringAndSuffix SplitParameterValue::getText() const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto sI = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto sII = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });
  return { sI->getDisplayString() + "\t|\t" + sII->getDisplayString(), "" };
}
