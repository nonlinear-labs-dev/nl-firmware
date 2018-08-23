#include <tools/TextSplitter.h>
#include "ParameterNameLabel.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"

ParameterNameLabel::ParameterNameLabel (const Rect &pos) :
    Label (pos)
{
  Application::get().getPresetManager ()->getEditBuffer ()->onSelectionChanged (
      sigc::hide < 0 > (sigc::mem_fun (this, &ParameterNameLabel::onParameterSelected)));
}

ParameterNameLabel::~ParameterNameLabel ()
{
}

void ParameterNameLabel::onParameterSelected(Parameter * param)
{
  if(param)
  {
    m_connection.disconnect();
    m_connection = param->onParameterChanged(mem_fun(this, &ParameterNameLabel::onParameterChanged));
  }
}

Glib::ustring removeLastChar(const Glib::ustring& s) {
  if (!s.empty())
    return s.substr(0, s.size() -1);
  return s;
}

Glib::ustring addStar(const Glib::ustring& s) {
  return s + "*";
}

void ParameterNameLabel::handleMCParameterName(const Parameter *pParameter) {
  const auto changed = pParameter->isChangedFromLoaded();
  auto name = pParameter->getLongName() + (changed ? "*" : "");
  name = truncateMCName(changed, name);
  setText (name, changed ? 1 : 0);
}

ustring &ParameterNameLabel::truncateMCName(const bool changed, ustring &name) const {
  if(getFont()->getStringWidth(name) > getWidth()) {
    if(changed)
      name = removeLastChar(name);

    while(getFont()->getStringWidth(ustring(name + ".." + (changed ? "*" : ""))) > getWidth()) {
      name = removeLastChar(name);
    }

    name += "..";

    if(changed)
      name = addStar(name);
  }
  return name;
}

void ParameterNameLabel::handleParameterName(const Parameter *pParameter) {
  const auto changed = pParameter->isChangedFromLoaded();
  setText (pParameter->getLongName() + (changed ? "*" : ""), changed ? 1 : 0);
}


void ParameterNameLabel::onParameterChanged (const Parameter *param)
{
  auto id = param->getID();
  if(id == 243 || id == 244 || id == 245 || id == 246) {
    handleMCParameterName(param);
  } else {
    handleParameterName(param);
  }

}

void ParameterNameLabel::setFontColor (FrameBuffer &fb) const
{
  if (isHighlight ())
    fb.setColor (FrameBuffer::Colors::C255);
  else
    fb.setColor (FrameBuffer::Colors::C128);
}

shared_ptr<Font> ParameterNameLabel::getFont () const
{
  return Oleds::get().getFont ("Emphase_9_Bold", getFontHeight ());
}

void ParameterNameLabel::drawSuffix (FrameBuffer &fb, int fullWidth, const Glib::ustring &firstPart, const Glib::ustring &secondPart)
{
  auto pos = getPosition ();
  auto font = getFont ();
  int offset = (pos.getHeight () - getFontHeight ()) / 2;

  int firstPartWidth = font->getStringWidth (firstPart);

  fb.setColor(FrameBuffer::C103);

  auto left = pos.getX () + getXOffset () + firstPartWidth;

  switch (getJustification ())
  {
    case Font::Justification::Center:
      font->draw (secondPart, left + (pos.getWidth () - fullWidth) / 2, pos.getBottom () - offset + getYOffset ());
      break;

    case Font::Justification::Left:
      font->draw (secondPart, left, pos.getBottom () - offset + getYOffset ());
      break;

    case Font::Justification::Right:
      font->draw (secondPart, left + (pos.getWidth () - fullWidth), pos.getBottom () - offset + getYOffset ());
      break;
  }
}

int ParameterNameLabel::getFontHeight () const
{
  return 9;
}


