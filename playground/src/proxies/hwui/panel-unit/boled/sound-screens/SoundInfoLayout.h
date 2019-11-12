#pragma once
#include <proxies/hwui/panel-unit/boled/parameter-screens/InfoLayout.h>

class SoundInfoLayout : public InfoLayout
{
 public:
  SoundInfoLayout();

 protected:
  void addModuleCaption() override;
  void addHeadline() override;
  void addInfoLabel() override;
  Scrollable *createScrollableContent() override;
};