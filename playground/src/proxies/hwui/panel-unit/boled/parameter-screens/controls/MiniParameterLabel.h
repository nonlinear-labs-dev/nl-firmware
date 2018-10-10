#pragma once

#include "proxies/hwui/controls/LabelRegular8.h"

class Parameter;

class MiniParameterLabel : public LabelRegular8
{
 private:
  typedef LabelRegular8 super;

 public:
  MiniParameterLabel(Parameter* parameter, const Rect& pos);
  virtual ~MiniParameterLabel();

  void setSelected(bool selected);
  bool isSelected() const;

 private:
  MiniParameterLabel(const MiniParameterLabel& other);
  MiniParameterLabel& operator=(const MiniParameterLabel&);

  void onParameterChanged(const Parameter* p);
  const std::pair<Glib::ustring, short> prepareDisplayString(const Parameter* p) const;

  bool redraw(FrameBuffer& fb) override;
  void setFontColor(FrameBuffer& fb) const override;

  bool m_selected;
};
