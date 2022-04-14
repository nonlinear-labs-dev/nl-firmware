#pragma once

#include "proxies/hwui/controls/LabelRegular8.h"

class Parameter;

class MiniParameterLabel : public LabelRegular8
{
 private:
  typedef LabelRegular8 super;

 public:
  MiniParameterLabel(Parameter* parameter, const Rect& pos);
  ~MiniParameterLabel() override;

  MiniParameterLabel(const MiniParameterLabel& other) = delete;
  MiniParameterLabel& operator=(const MiniParameterLabel&) = delete;

  void setSelected(bool selected);
  bool isSelected() const;
  void setParameterNameTransformer(std::function<Glib::ustring(Glib::ustring)> transformer);
  void setInfix(Glib::ustring infix);
  Glib::ustring getInfix() const;

 private:
  void onParameterChanged(const Parameter* p);
  const std::pair<Glib::ustring, size_t> prepareDisplayString(const Parameter* p) const;

  bool redraw(FrameBuffer& fb) override;
  void setFontColor(FrameBuffer& fb) const override;
  Glib::ustring cleanupName(Glib::ustring in) const;

  bool m_selected;
  std::optional<Glib::ustring> m_infix = std::nullopt;
  std::function<Glib::ustring(Glib::ustring)> m_nameTransformer;
};
