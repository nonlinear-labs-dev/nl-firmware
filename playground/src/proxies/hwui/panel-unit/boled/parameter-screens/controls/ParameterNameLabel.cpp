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

const Glib::ustring &ParameterNameLabel::truncateMCName(const bool changed, const Glib::ustring &name) const {
  Glib::ustring ret = name;

  if(getFont()->getStringWidth(ret + ".." + (changed ? "*" : "")) > getWidth()) {
    if(changed)
      ret = removeLastChar(ret);

    while(getFont()->getStringWidth(ustring(ret + ".." + (changed ? "*" : ""))) > getWidth()) {
      ret = removeLastChar(ret);
    }

    ret += "..";

    if(changed)
      ret = addStar(ret);
  }
  return ret;
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

int ParameterNameLabel::getFontHeight () const
{
  return 9;
}

void ParameterNameLabel::setSuffixFontColor(FrameBuffer &fb) const {
  fb.setColor(FrameBuffer::C103);
}


