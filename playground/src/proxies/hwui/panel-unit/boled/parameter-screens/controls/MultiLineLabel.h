#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/Oleds.h>

class MultiLineLabel : public ControlWithChildren
{
 private:
  typedef ControlWithChildren super;

 public:
  MultiLineLabel(const Glib::ustring &text);

  bool setText(const Glib::ustring &text, FrameBuffer::Colors c);
  const Glib::ustring& getText() const;
  void setColor(FrameBuffer::Colors c);

  void drawBackground(FrameBuffer &fb) override;
  bool redraw(FrameBuffer &fb) override;

  void setForceUpdateLine(bool update);
  virtual void setPosition(const Rect &rect) override;

 protected:
  virtual Oleds::tFont getFont();

 private:
  bool m_forceUpdateOnSetPos = false;
  void updateLines();

  FrameBuffer::Colors m_color = FrameBuffer::Colors::C103;
  Glib::ustring m_text;
};
