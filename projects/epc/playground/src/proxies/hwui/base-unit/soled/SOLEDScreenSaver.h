#pragma once
#include <proxies/hwui/Layout.h>
#include <proxies/hwui/controls/Label.h>
#include <sigc++/connection.h>

class SOLEDScreenSaver : public Layout
{
 public:
  explicit SOLEDScreenSaver(OLEDProxy& oled);
  bool redraw(FrameBuffer& fb) override;
  void init() override;

 private:
  std::pair<int, int> velocity = { 1, 1 };
  Label* m_scrollingLabel = nullptr;
  sigc::connection m_animationConnection;
  sigc::connection m_editbufferConnection;

  void destroy();
  bool animate();
};