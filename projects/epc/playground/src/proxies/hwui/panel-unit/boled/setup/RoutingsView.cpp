#include "RoutingsView.h"
#include "RoutingsSettingsHelper.h"
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/labels/LabelStyleable.h>

RoutingsView::RoutingsView(tID id)
    : SetupLabel(Rect(0, 0, 0, 0))
    , m_id(id)
    , m_aspectList(getAspectsForIndex(id))
{
  getSetting()->onChange(sigc::mem_fun(this, &RoutingsView::onSettingChanged));
}

inline auto getShortName(RoutingsView::tAspect a)
{
  typedef nltools::msg::Setting::MidiSettingsMessage::RoutingAspect tAspect;
  switch(a)
  {
    case tAspect::SEND_PRIMARY:
      return "SP";
    case tAspect::RECEIVE_PRIMARY:
      return "RP";
    case tAspect::SEND_SPLIT:
      return "SS";
    case tAspect::RECEIVE_SPLIT:
      return "RS";
    case tAspect::LOCAL:
      return "L";
    default:
    case tAspect::LENGTH:
      return "";
  }
}

bool RoutingsView::redraw(FrameBuffer& fb)
{
  const auto pos = getPosition();
  const auto width = pos.getWidth();
  const auto height = pos.getHeight();
  const auto boxWidth = width / size;
  const int y = pos.getTop();

  int x = pos.getLeft();

  LabelStyleable text(pos);
  text.setLabelStyle({.size = FontSize::Size7, .decoration = FontDecoration::Regular});

  int idx = 0;
  for(auto& a : m_aspectList)
  {
    auto state = m_data[idx++];
    idx %= size;
    //Draw Box in color with labels
    auto bgColor = (not state) ? FrameBufferColors::C77 : FrameBufferColors::C103;
    fb.setColor(bgColor);

    Rect boxPos = { x, y, boxWidth, height };
    fb.fillRect(boxPos);
    text.setPosition(boxPos);
    text.setText(getShortName(a));
    text.redraw(fb);

    x += boxWidth;
  }
  return SetupLabel::redraw(fb);
}

void RoutingsView::onSettingChanged(const Setting* s)
{
  if(auto r = dynamic_cast<const RoutingSettings*>(s))
  {
    auto id = 0;
    for(const auto& a : m_aspectList)
    {
      m_data[id++] = r->getState(m_id, a);
      id %= size;
    }
    setDirty();
  }
}