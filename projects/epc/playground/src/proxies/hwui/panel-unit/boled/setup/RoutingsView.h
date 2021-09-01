#pragma once
#include "SetupLabel.h"
#include <device-settings/midi/RoutingSettings.h>

class RoutingsView : public SetupLabel
{
 public:
  typedef RoutingSettings::tRoutingIndex tID;
  typedef RoutingSettings::tAspectIndex tAspect;
  typedef std::initializer_list<tAspect> tAspectList;

  explicit RoutingsView(tID id);
  bool redraw(FrameBuffer& fb) override;

 private:
  void onSettingChanged(const Setting* s);

  constexpr static auto size = static_cast<int>(RoutingSettings::tAspectIndex::LENGTH);
  std::array<bool, size> m_data {false};
  const tID m_id;
  const tAspectList m_aspectList;
};