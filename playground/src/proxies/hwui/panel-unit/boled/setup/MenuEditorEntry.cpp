#include <proxies/hwui/panel-unit/boled/setup/MenuEditorEntry.h>
#include <proxies/hwui/FrameBuffer.h>

MenuEditorEntry::MenuEditorEntry()
    : super(Rect(0, 0, 0, 0))
{
}

MenuEditorEntry::~MenuEditorEntry()
{
}

void MenuEditorEntry::assign(const std::vector<Glib::ustring> &entries, int idx, bool selected)
{
  if(idx < 0 || idx >= entries.size())
    setText("");
  else
    setText({ entries[idx] });

  setHighlight(selected);
}

void MenuEditorEntry::setFontColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C255);
  else
    super::setFontColor(fb);
}

void MenuEditorEntry::setBackgroundColor(FrameBuffer &fb) const
{
  if(isHighlight())
    fb.setColor(FrameBufferColors::C103);
  else
    super::setBackgroundColor(fb);
}

Font::Justification MenuEditorEntry::getJustification() const
{
  return Font::Justification::Center;
}
