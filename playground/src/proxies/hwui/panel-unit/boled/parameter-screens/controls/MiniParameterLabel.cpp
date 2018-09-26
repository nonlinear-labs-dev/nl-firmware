#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include "MiniParameterLabel.h"

MiniParameterLabel::MiniParameterLabel(const Parameter* parameter, const Glib::ustring &text, const Rect &pos)
    : super(text, pos)
    , m_selected(false)
    , m_originalText(text)
    , m_parameter(parameter)
{
  Application::get().getPresetManager()->getEditBuffer()->onChange(sigc::mem_fun(this, &MiniParameterLabel::onEditbufferChanged));
  onEditbufferChanged();
}

MiniParameterLabel::~MiniParameterLabel()
{
}



void MiniParameterLabel::onEditbufferChanged() {
  auto[displayString, len] = prepareDisplayString();
  setText(displayString, len);
}

void MiniParameterLabel::setSelected(bool selected)
{
  if(m_selected != selected)
  {
    m_selected = selected;
    setDirty();
  }
}

bool MiniParameterLabel::isSelected() const
{
  return m_selected;
}

bool MiniParameterLabel::redraw(FrameBuffer &fb)
{
  auto r = getPosition();

  if(m_selected)
  {
    fb.setColor(FrameBuffer::Colors::C103);
    fb.fillRect(r.getX(), r.getY(), r.getWidth(), r.getHeight());
  }

  super::redraw(fb);
  return true;
}

void MiniParameterLabel::setFontColor(FrameBuffer &fb) const
{
  if(m_selected)
    if(isHighlight())
      fb.setColor(FrameBuffer::Colors::C255);
    else
      fb.setColor(FrameBuffer::Colors::C128);
  else
    fb.setColor(FrameBuffer::Colors::C128);
}

const std::pair<Glib::ustring, short> MiniParameterLabel::prepareDisplayString() const {
  auto changed = m_parameter->isChangedFromLoaded();
  auto text = changed ? m_originalText + "*" : m_originalText;
  auto suffixLen = changed ? 1 : 0;
  return std::make_pair(text, suffixLen);
}
