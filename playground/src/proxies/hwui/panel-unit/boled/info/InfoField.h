#pragma once

#include <glibmm/ustring.h>
#include <proxies/hwui/FrameBuffer.h>

class SingleLineInfoContent;
class Control;

class InfoField
{
 public:
  InfoField(SingleLineInfoContent* label, Control* c);

  void setInfo(const Glib::ustring& text);
  void setInfo(const Glib::ustring& text, FrameBuffer::Colors c);
  void setPosition(int y);
  int format(int y);

 private:
  SingleLineInfoContent* m_label;
  Control* m_content;
};
