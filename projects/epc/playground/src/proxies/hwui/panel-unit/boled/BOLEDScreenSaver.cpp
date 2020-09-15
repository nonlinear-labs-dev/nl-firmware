#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/OLEDProxy.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include "BOLEDScreenSaver.h"
#include <glibmm/main.h>
#include <chrono>

BOLEDScreenSaver::BOLEDScreenSaver(OLEDProxy& oled)
    : Layout(oled)
{
  m_logoNL = addControl(new Label({ "NL", 0 }, { 128, 40, 16, 16 }));
  m_logoC15 = addControl(new Label({ "C15", 0 }, { 128 + 16, 40, 20, 16 }));
  
  m_animation = Application::get().getMainContext()->signal_timeout().connect(
      sigc::mem_fun(this, &BOLEDScreenSaver::animate), 50);

  m_editBufferConnection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &BOLEDScreenSaver::destroy), false);
}

BOLEDScreenSaver::~BOLEDScreenSaver()
{
  m_animation.disconnect();
}

bool BOLEDScreenSaver::animate()
{
  for(auto& p : std::vector<std::pair<Control*, std::pair<int, int>&>> { { m_logoC15, m_velC }, { m_logoNL, m_velN } })
  {
    auto old = p.first->getPosition();

    if(old.getRight() > 256)
    {
      p.second.first = -1;
    }

    if(old.getLeft() < 0)
    {
      p.second.first = 1;
    }

    if(old.getBottom() > 64)
    {
      p.second.second = -1;
    }

    if(old.getTop() < 0)
    {
      p.second.second = 1;
    }

    old.setLeft(old.getLeft() + p.second.first);
    old.setTop(old.getTop() + p.second.second);

    p.first->setPosition(old);
  }

  setDirty();
  return true;
}

void BOLEDScreenSaver::destroy()
{
  getOLEDProxy().resetOverlay();
}
