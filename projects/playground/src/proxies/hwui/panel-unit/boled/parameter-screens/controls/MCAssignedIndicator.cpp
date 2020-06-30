#include "MCAssignedIndicator.h"
#include <proxies/hwui/FrameBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <groups/ParameterGroup.h>
#include <proxies/hwui/controls/LabelRegular8.h>
#include <Application.h>

MCAssignedIndicator::MCAssignedIndicator(const Rect& r, const Parameter* p)
    : ControlWithChildren(r)
    , m_parameter { dynamic_cast<const MacroControlParameter*>(p) }
{
  auto pos = r;
  pos.setWidth(25);
  pos.setLeft(r.getLeft());
  pos.setTop(r.getTop());
  m_topRowLabel = addControl(new LabelRegular8(pos));
  m_topRowLabel->setJustification(Font::Justification::Right);
  pos.setTop(r.getTop() + 8);
  m_middleRowLabel = addControl(new LabelRegular8(pos));
  m_middleRowLabel->setJustification(Font::Justification::Right);
  pos.setTop(r.getTop() + 8);
  m_bottomRowLabel = addControl(new LabelRegular8(pos));
  m_bottomRowLabel->setJustification(Font::Justification::Right);
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

void MCAssignedIndicator::drawSingle(FrameBuffer& fb, const AffectedGroups& mods)
{

  std::stringstream ss;

  if(modPart)
  {
    ss << "P";
    if(modVoices || modMaster)
      ss << ",";
  }

  if(modVoices)
  {
    ss << "V";
    if(modMaster)
      ss << ",";
  }

  if(modMaster)
  {
    ss << "M";
  }

  m_middleRowLabel->setText({ ss.str(), 0 });
  m_middleRowLabel->redraw(fb);

  if(modVoices || modMaster || modPart)
  {
    drawArrow(fb);
  }
}

void MCAssignedIndicator::drawArrow(FrameBuffer& fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C128);
  else
    fb.setColor(FrameBufferColors::C204);

  Rect r = getPosition();
  Point c = r.getCenter();

  const auto margin = m_label->getWidth();
  const auto middle = r.getCenter().getY();

  fb.setPixel(r.getLeft() + margin + 1, middle - 1);
  fb.setPixel(r.getLeft() + margin + 2, middle - 2);
  fb.setPixel(r.getLeft() + margin + 1, middle + 1);
  fb.setPixel(r.getLeft() + margin + 2, middle + 2);

  for(int i = r.getLeft() + margin; i <= r.getRight(); i += 2)
    fb.setPixel(i, middle);
}

void MCAssignedIndicator::drawSplit(FrameBuffer& fb, const AffectedGroups& mods)
{
}

void MCAssignedIndicator::drawLayer(FrameBuffer& fb, const AffectedGroups& mods)
{
}

void MCAssignedIndicator::drawNonLEDTargets(FrameBuffer& fb)
{
  auto targets = m_parameter->getTargets();

  AffectedGroups mods {};

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

  switch(Application::get().getPresetManager().getEditBuffer()->getType())
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
