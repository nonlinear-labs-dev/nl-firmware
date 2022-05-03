#pragma once

#include "proxies/hwui/controls/Label.h"
#include <ParameterId.h>

class PhysicalControlParameter;
class Application;

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

 protected:
  int getXOffset() const override;

 private:
  Glib::ustring crop(const Glib::ustring &text) const;
  Glib::ustring binarySearchLength(const Glib::ustring &text, unsigned long min, unsigned long max) const;

  using tRibbonEnables = std::pair<bool, bool>;
  tRibbonEnables getRibbonEnabledStates() const;

  std::vector<sigc::connection> m_signalsChanged;
  ParameterId m_parameterID;
  PhysicalControlParameter *m_parameter;
};
