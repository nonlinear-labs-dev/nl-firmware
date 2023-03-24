#pragma once

#include <proxies/hwui/OLEDProxy.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/InfoLayout.h>

class MultiLineLabel;
class HWUI;

class SplashLayout : public InfoLayout
{
  typedef InfoLayout super;

 public:
  SplashLayout();
  ~SplashLayout() override;

  void setMessage(const std::string &txt);
  void addMessage(const std::string &txt);

 private:
  void addHeadline() override;
  void addModuleCaption() override;
  void addInfoLabel() override;
  Scrollable *createScrollableContent() override;
  Rect getScrollableAreaRect() const override;

  MultiLineLabel *m_message = nullptr;
  std::string m_text;
  HWUI *m_hwui;
};
