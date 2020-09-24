#pragma once

#include <proxies/hwui/panel-unit/boled/setup/SetupLabel.h>

class PedalType;

class PedalView : public SetupLabel
{
 private:
  typedef SetupLabel base_type;

 public:
  PedalView(std::shared_ptr<PedalType> s);
  virtual ~PedalView();
  bool redraw(FrameBuffer& fb) override;

 private:
  void onSettingChanged(const Setting* s);
};
