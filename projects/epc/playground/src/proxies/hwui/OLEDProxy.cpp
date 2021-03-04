#include <device-settings/DebugLevel.h>
#include <proxies/hwui/Layout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/OLEDProxy.h>
#include <glib.h>
#include <proxies/hwui/Oleds.h>

#include <utility>

OLEDProxy::OLEDProxy(const Rect &posInFrameBuffer)
    : m_posInFrameBuffer(posInFrameBuffer)
{
  Oleds::get().registerProxy(this);
}

OLEDProxy::~OLEDProxy() = default;

const Rect &OLEDProxy::getPosInFrameBuffer() const
{
  return m_posInFrameBuffer;
}

void OLEDProxy::invalidate()
{
  if(auto l = std::dynamic_pointer_cast<Layout>(getLayout()))
    l->setDirty();
  else
    DebugLevel::warning("Oled proxy has NO screen set !??");
}

OLEDProxy::tLayoutPtr OLEDProxy::getLayout() const
{
  if(m_overlay)
    return m_overlay;

  return m_layout;
}

OLEDProxy::tLayoutPtr OLEDProxy::getBaseLayout() const
{
  return m_layout;
}

OLEDProxy::tLayoutPtr OLEDProxy::getOverlay() const
{
  return m_overlay;
}

void OLEDProxy::reset(Layout *layout)
{
  tLayoutPtr s(layout);
  reset(s);
}

void OLEDProxy::reset(tLayoutPtr layout)
{
  resetOverlay();

  if(m_layout)
    m_layout->removeButtonRepeat();

  m_layout = layout;

  if(!layout->isInitialized())
    layout->init();

  if(m_onLayoutInstalledOnce)
  {
    m_onLayoutInstalledOnce(layout.get());
    m_onLayoutInstalledOnce = nullptr;
  }
  
  m_sigLayoutInstalled.emit(layout.get());

  DebugLevel::info(G_STRLOC, typeid(layout.get()).name());
  invalidate();
}

void OLEDProxy::setOverlay(Layout *layout)
{
  tLayoutPtr s(layout);
  setOverlay(s);
}

void OLEDProxy::setOverlay(tLayoutPtr layout)
{
  if(m_layout)
    m_layout->removeButtonRepeat();

  m_overlay = layout;

  if(!layout->isInitialized())
    layout->init();

  DebugLevel::info(G_STRLOC, typeid(layout.get()).name());
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
    auto &fb = FrameBuffer::get();
    auto clip = fb.clip(m_posInFrameBuffer);
    auto offset = fb.offset(m_posInFrameBuffer.getPosition());
    ret = l->redrawLayout();
  }

  return ret;
}

void OLEDProxy::clear()
{
  auto &fb = FrameBuffer::get();
  fb.setColor(FrameBufferColors::C43);
  fb.fillRect(Rect(0, 0, m_posInFrameBuffer.getWidth(), m_posInFrameBuffer.getHeight()));
}

void OLEDProxy::onLayoutInstalled(const sigc::slot<void, Layout *> &slot)
{
  m_sigLayoutInstalled.connect(slot);
}

void OLEDProxy::onLayoutInstalledDoOnce(std::function<void(Layout *)> cb)
{
  if(m_onLayoutInstalledOnce != nullptr)
  {
    nltools::Log::warning("removing non called onLayoutInstalledOnce Callback!", __LINE__, __PRETTY_FUNCTION__,
                          __FILE__);
  }

  m_onLayoutInstalledOnce = std::move(cb);
}
