#pragma once
#include <proxies/hwui/Layout.h>
#include <sigc++/connection.h>

class BOLEDScreenSaver : public Layout
{
 public:
  BOLEDScreenSaver(OLEDProxy& oled);
  ~BOLEDScreenSaver() override;

 private:
  bool animate();
  std::pair<int, int> m_velC = { 1, 1 };
  std::pair<int, int> m_velN = { -1, -1 };
  sigc::connection m_animation;
  Control* m_logoNL;
  Control* m_logoC15;
};
