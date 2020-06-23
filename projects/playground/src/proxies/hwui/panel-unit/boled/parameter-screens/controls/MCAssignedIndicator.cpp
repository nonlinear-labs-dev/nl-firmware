#include "MCAssignedIndicator.h"
#include <proxies/hwui/FrameBuffer.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <groups/ParameterGroup.h>
#include <proxies/hwui/controls/LabelRegular8.h>

MCAssignedIndicator::MCAssignedIndicator(const Rect& r, const Parameter* p)
    : ControlWithChildren(r)
    , m_parameter { dynamic_cast<const MacroControlParameter*>(p) }
{
  auto pos = r;
  pos.setWidth(25);
  pos.setLeft(r.getLeft());
  pos.setTop(r.getTop() + 1);
  m_label = addControl(new LabelRegular8(pos));
  m_label->setJustification(Font::Justification::Right);
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

void MCAssignedIndicator::drawNonLEDTargets(FrameBuffer& fb)
{
  auto targets = m_parameter->getTargets();

  bool modPart = false;
  bool modVoices = false;
  bool modMaster = false;

  for(auto& t : targets)
  {
    auto groupName = t->getParentGroup()->getID().getName();

    modPart |= groupName == "Part";
    modVoices |= (groupName == "Mono" || groupName == "Unison");
    modMaster |= groupName == "Master";
  }

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

  m_label->setText({ ss.str(), 0 });
  m_label->redraw(fb);

  if(modVoices || modMaster || modPart)
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
}
