#include "MCAssignedIndicator.h"
#include <proxies/hwui/FrameBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <groups/ParameterGroup.h>
#include <proxies/hwui/controls/LabelRegular8.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

MCAssignedIndicator::MCAssignedIndicator(const Rect& r, const Parameter* p)
    : ControlWithChildren(r)
    , m_parameter { dynamic_cast<const MacroControlParameter*>(p) }
{
  auto pos = r;
  pos.setWidth(25);
  pos.setLeft(r.getLeft());
  pos.setHeight(8);

  auto offset = 0;

  m_topRowLabel = addControl(new LabelRegular8(pos));
  m_middleRowLabel = addControl(new LabelRegular8(pos));
  m_bottomRowLabel = addControl(new LabelRegular8(pos));

  for(auto l : { m_topRowLabel, m_middleRowLabel, m_bottomRowLabel })
  {
    pos.setTop(r.getTop() + offset);
    l->setPosition(pos);
    l->setJustification(Font::Justification::Right);
    offset += 8;
  }
}

bool MCAssignedIndicator::redraw(FrameBuffer& fb)
{
  if(m_parameter && m_parameter->getTargets().empty())
  {
    drawBackground(fb);
    return false;
  }

  drawNonLEDTargets(fb);
  return true;
}

void MCAssignedIndicator::drawArrow(FrameBuffer& fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C204);

  Rect r = getPosition();
  Point c = r.getCenter();

  const auto margin = m_middleRowLabel->getWidth();
  const auto middle = r.getCenter().getY();

  fb.setPixel(r.getLeft() + margin + 1, middle - 1);
  fb.setPixel(r.getLeft() + margin + 2, middle - 2);
  fb.setPixel(r.getLeft() + margin + 1, middle + 1);
  fb.setPixel(r.getLeft() + margin + 2, middle + 2);

  for(int i = r.getLeft() + margin; i <= r.getRight(); i += 2)
    fb.setPixel(i, middle);
}

void MCAssignedIndicator::drawSingle(FrameBuffer& fb, const AffectedGroups& mods)
{
  std::stringstream ss;

  if(mods.mono[0])
  {
    ss << "\uE040";
    if(mods.scale || mods.master || mods.unison[0])
      ss << " ";
  }

  if(mods.unison[0])
  {
    ss << "\uE041";
    if(mods.scale || mods.master)
      ss << " ";
  }

  if(mods.scale)
  {
    ss << "S";
    if(mods.master)
      ss << " ";
  }

  if(mods.master)
  {
    ss << "M";
  }

  m_middleRowLabel->setText({ ss.str(), 0 });
  m_middleRowLabel->redraw(fb);

  if(mods.affected(SoundType::Single))
  {
    drawArrow(fb);
  }
}

void MCAssignedIndicator::drawSplit(FrameBuffer& fb, const AffectedGroups& mods)
{
  std::stringstream middleSs;
  std::stringstream topSs;
  std::stringstream bottomSs;

  if(mods.mono[0])
  {
    topSs << "\uE040";
    if(mods.unison[0] || mods.part[0])
      topSs << " ";
  }

  if(mods.mono[1])
  {
    bottomSs << "\uE040";
    if(mods.unison[1] || mods.part[1])
      bottomSs << " ";
  }

  if(mods.unison[0])
  {
    topSs << "\uE041";
    if(mods.part[0])
      topSs << " ";
  }

  if(mods.unison[1])
  {
    bottomSs << "\uE041";
    if(mods.part[1])
      bottomSs << " ";
  }

  if(mods.scale)
  {
    middleSs << "S";
    if(mods.master)
      middleSs << " ";
  }

  if(mods.master)
  {
    middleSs << "M";
  }

  if(mods.part[0])
  {
    topSs << "P";
  }

  if(mods.part[1])
  {
    bottomSs << "P";
  }

  m_topRowLabel->setText({ topSs.str(), 0 });
  m_topRowLabel->redraw(fb);

  m_middleRowLabel->setText({ middleSs.str(), 0 });
  m_middleRowLabel->redraw(fb);

  m_bottomRowLabel->setText({ bottomSs.str(), 0 });
  m_bottomRowLabel->redraw(fb);

  if(mods.affected(SoundType::Split))
  {
    drawArrow(fb);
  }
}

void MCAssignedIndicator::drawLayer(FrameBuffer& fb, const AffectedGroups& mods)
{
  std::stringstream ss;

  if(mods.mono[0])
  {
    ss << "\uE040";
    if(mods.unison[0] || mods.master || mods.scale)
      ss << " ";
  }

  if(mods.unison[0])
  {
    ss << "\uE041";
    if(mods.master || mods.scale)
      ss << " ";
  }

  if(mods.scale)
  {
    ss << "S";
    if(mods.master)
      ss << " ";
  }

  if(mods.master)
  {
    ss << "M";
  }

  if(mods.part[0])
  {
    m_topRowLabel->setText({ "P", 0 });
    m_topRowLabel->redraw(fb);
  }

  if(mods.part[1])
  {
    m_bottomRowLabel->setText({ "P", 0 });
    m_bottomRowLabel->redraw(fb);
  }

  m_middleRowLabel->setText({ ss.str(), 0 });
  m_middleRowLabel->redraw(fb);

  if(mods.affected(SoundType::Layer))
  {
    drawArrow(fb);
  }
}

void MCAssignedIndicator::drawNonLEDTargets(FrameBuffer& fb)
{
  auto targets = m_parameter->getTargets();

  AffectedGroups mods {};

  auto color = isHighlight() ? FrameBufferColors::C128 : FrameBufferColors::C204;

  for(auto l : { m_topRowLabel, m_middleRowLabel, m_bottomRowLabel })
  {
    l->setText({ "", 0 });
    l->setFontColor(color);
  }

  for(auto& t : targets)
  {
    auto groupName = t->getParentGroup()->getID().getName();
    mods.scale |= groupName == "Scale";
    mods.master |= groupName == "Master";

    if(t->getID().getVoiceGroup() != VoiceGroup::Global)
    {
      auto index = static_cast<int>(t->getID().getVoiceGroup());
      mods.unison.at(index) |= groupName == "Unison";
      mods.mono.at(index) |= groupName == "Mono";
      mods.part.at(index) |= groupName == "Part";
    }
  }

  switch(Application::get().getPresetManager()->getEditBuffer()->getType())
  {
    case SoundType::Split:
      drawSplit(fb, mods);
      break;
    case SoundType::Layer:
      drawLayer(fb, mods);
      break;
    case SoundType::Single:
      drawSingle(fb, mods);
      break;
  }
}