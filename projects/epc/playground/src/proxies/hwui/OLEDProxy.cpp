#include <device-settings/DebugLevel.h>
#include <proxies/hwui/Layout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/OLEDProxy.h>
#include <glib.h>
#include <proxies/hwui/Oleds.h>

#include <utility>
#include <Application.h>
#include <proxies/hwui/HWUI.h>

OLEDProxy::OLEDProxy(const Rect &posInFrameBuffer, Oleds &oleds)
    : m_fb(oleds.getFrameBuffer())
    , m_posInFrameBuffer(posInFrameBuffer)
{
  oleds.registerProxy(this);
}

FrameBuffer &OLEDProxy::getFrameBuffer()
{
  return m_fb;
}

OLEDProxy::~OLEDProxy() = default;

const Rect &OLEDProxy::getPosInFrameBuffer() const
{
  return m_posInFrameBuffer;
}

void OLEDProxy::invalidate()
{
  if(auto l = dynamic_cast<Layout*>(getLayout()))
    l->setDirty();
  else
    DebugLevel::warning("Oled proxy has NO screen set !??");
}

Layout* OLEDProxy::getLayout() const
{
  if(m_screenSaver)
    return m_screenSaver.get();

  if(m_overlay)
    return m_overlay.get();

  return m_layout.get();
}

Layout* OLEDProxy::getBaseLayout() const
{
  return m_layout.get();
}

Layout* OLEDProxy::getOverlay() const
{
  return m_overlay.get();
}

void OLEDProxy::reset(Layout *layout)
{
  resetOverlay();

  if(m_layout)
    m_layout->removeButtonRepeat();

  tLayoutPtr oldLayout(m_layout.release());
  m_layout.reset(layout);

  if(!layout->isInitialized())
    layout->init();

  if(oldLayout)
  {
    layout->copyFrom(oldLayout.get());
  }

  m_sigLayoutInstalled.emit(m_layout.get());

  DebugLevel::info(G_STRLOC, typeid(m_layout.get()).name());
  invalidate();
}

void OLEDProxy::setOverlay(Layout *layout)
{
  if(m_layout)
    m_layout->removeButtonRepeat();

  m_overlay.reset(layout);

  if(!layout->isInitialized())
    layout->init();

  DebugLevel::info(G_STRLOC, typeid(layout).name());
  invalidate();
}

void OLEDProxy::resetOverlay()
{
  m_overlay.reset();
  invalidate();
}

bool OLEDProxy::redraw()
{
  bool ret = false;

  if(auto l = getLayout())
  {
    auto clip = m_fb.clip(m_posInFrameBuffer);
    auto offset = m_fb.offset(m_posInFrameBuffer.getPosition());
    ret = l->redrawLayout();
  }

  return ret;
}

void OLEDProxy::clear()
{
  m_fb.setColor(FrameBufferColors::C43);
  m_fb.fillRect(Rect(0, 0, m_posInFrameBuffer.getWidth(), m_posInFrameBuffer.getHeight()));
}

sigc::connection OLEDProxy::onLayoutInstalled(const sigc::slot<void, Layout *> &slot)
{
  return m_sigLayoutInstalled.connect(slot);
}

void OLEDProxy::installScreenSaver(Layout *l)
{
  m_screenSaver.reset(l);
  if(!m_screenSaver->isInitialized())
    m_screenSaver->init();
}

void OLEDProxy::removeScreenSaver()
{
  m_screenSaver.reset();
  invalidate();
}

Layout *OLEDProxy::getScreenSaver()
{
  return m_screenSaver.get();
}
