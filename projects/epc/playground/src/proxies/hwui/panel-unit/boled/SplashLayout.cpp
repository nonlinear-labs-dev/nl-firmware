#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/Font.h>
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/InfoLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModuleCaption.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>

namespace DETAIL
{
  class Headline : public Label
  {
    typedef Label super;

   public:
    using super::super;

    std::shared_ptr<Font> getFont() const override
    {
      return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());
    }

    int getFontHeight() const override
    {
      return 9;
    }

    void setFontColor(FrameBuffer &fb) const override
    {
      fb.setColor(FrameBufferColors::C255);
    }
  };

  class Message : public MultiLineLabel, public Scrollable
  {
   private:
    typedef MultiLineLabel super;

   public:
    Message(ControlOwner *parent)
        : super("")
        , m_parent(parent)
    {
    }

    virtual void setPosition(const Rect &rect) override
    {
      super::setPosition(rect);
    }

    virtual const Rect &getPosition() const override
    {
      return super::getPosition();
    }

    virtual void setDirty() override
    {
      super::setDirty();
      notifyDirty(true);
    }

   protected:
    virtual Fonts::tFont getFont() override
    {
      return Fonts::get().getFont("Emphase-8-TXT-Regular", 8);
    }

   private:
    ControlOwner *m_parent;
  };
}

SplashLayout::SplashLayout(HWUI *hwui)
  : m_hwui(hwui)
{
  hwui->registerSplash(this);
  Application::get().stopWatchDog();
}

SplashLayout::~SplashLayout()
{
  m_hwui->unregisterSplash(this);
  Application::get().runWatchDog();
}

void SplashLayout::setMessage(const std::string &txt)
{
  m_text = txt;
  m_message->setText(m_text, FrameBufferColors::C179);
  Application::get().getHWUI()->getOleds().syncRedraw(true);
}

void SplashLayout::addMessage(const std::string &txt)
{
  if(!m_text.empty())
    m_text += "\n";

  m_text += txt;
  m_message->setText(m_text, FrameBufferColors::C179);
  scrollToMax();

  Application::get().getHWUI()->getOleds().syncRedraw(true);
}

void SplashLayout::addHeadline()
{
  addControl(new DETAIL::Headline(StringAndSuffix { "Nonlinear Labs - C15" }, Rect(0, 0, 256, 16)));
}

void SplashLayout::addInfoLabel()
{
}

void SplashLayout::addModuleCaption()
{
}

Scrollable *SplashLayout::createScrollableContent()
{
  auto ret = new DETAIL::Message(this);
  m_message = ret;
  return ret;
}

Rect SplashLayout::getScrollableAreaRect() const
{
  return {0, 16, 256, 64 - 16};
}
