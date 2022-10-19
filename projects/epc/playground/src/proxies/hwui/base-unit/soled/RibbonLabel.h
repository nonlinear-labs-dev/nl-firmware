#pragma once

#include "proxies/hwui/controls/Label.h"
#include <ParameterId.h>

class PhysicalControlParameter;
class Application;
class Setting;

class RibbonLabel : public Label
{
 private:
  typedef Label super;

 public:
  RibbonLabel(const ParameterId &paramID, const Rect &rect);
  ~RibbonLabel() override;

  StringAndSuffix getText() const override;

  RibbonLabel(const RibbonLabel &other) = delete;
  RibbonLabel &operator=(const RibbonLabel &) = delete;

  std::shared_ptr<Font> getFont() const override;

 protected:
  int getXOffset() const override;
  void onRibbonSelectionChanged(const Setting *s);
  int getFontHeight() const override;

 private:
  Glib::ustring crop(const Glib::ustring &text) const;

  using tRibbonEnables = std::tuple<bool, bool, bool, bool>;
  tRibbonEnables getRibbonEnabledStates() const;

  std::vector<sigc::connection> m_signalsChanged;
  ParameterId m_parameterID;
  PhysicalControlParameter *m_parameter;
};
