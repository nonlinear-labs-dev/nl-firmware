#include "Oleds.h"
#include "OLEDProxy.h"
#include "Application.h"
#include <proxies/hwui/Font.h>
#include <proxies/hwui/FrameBuffer.h>
#include <tools/PerformanceTimer.h>
#include <CompileTimeOptions.h>

Oleds::Oleds()
    : m_throttler(std::chrono::milliseconds(20))
{
}

Oleds::~Oleds()
{
  m_throttler.cancel();
}

void Oleds::setDirty()
{
  if(!m_throttler.isPending())
  {
    static Settings* lastSettingsAtSetDirty = Application::get().getSettings();
    lastSettingsAtSetDirty = Application::get().getSettings();
    m_throttler.doTask([this, &lastSettingsAtSetDirty=lastSettingsAtSetDirty] {
                         auto app = &Application::get();
                         auto settingAtRedraw = app->getSettings();
                         syncRedraw(false);
                       });
  }
}

bool Oleds::isDirty() const
{
  return m_throttler.isPending();
}

void Oleds::deInit()
{
  m_proxies.clear();

  FrameBuffer::get().clear();
  FrameBuffer::get().swapBuffers();
}

void Oleds::registerProxy(OLEDProxy *proxy)
{
  m_proxies.push_back(proxy);
}

void Oleds::syncRedraw(bool force)
{
  for(auto proxy : m_proxies)
    proxy->redraw();

  FrameBuffer::get().swapBuffers(force);
}

Fonts &Fonts::get()
{
  static Fonts f;
  return f;
}

Fonts::tFont Fonts::getFont(const Glib::ustring &name, int height)
{
  tKey key(name, height);

  auto it = m_fonts.find(key);

  if(it != m_fonts.end())
    return it->second;

  Glib::ustring path = getResourcesDir() + "/" + name + ".ttf";
  auto font = std::make_shared<Font>(path, height);
  m_fonts[key] = font;
  return font;
}
