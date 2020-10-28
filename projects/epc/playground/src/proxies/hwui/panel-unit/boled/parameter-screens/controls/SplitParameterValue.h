#pragma once
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/controls/PNGControl.h>

class SplitParameterValue : public Label
{
 public:
  explicit SplitParameterValue(const Rect& pos);
  SplitParameterValue(const StringAndSuffix& text, const Rect& pos);

  bool redraw(FrameBuffer& fb) override;
  StringAndSuffix getText() const override;

 private:
  void init();
  void drawParts(FrameBuffer& fb, const std::vector<Glib::ustring>& parts);
  std::shared_ptr<PNGControl> overlapIndicator;

  void onSplitIChanged(const Parameter* splitI);
  void onSplitIIChanged(const Parameter* splitII);
  void onSplitValuesChanged();
  void onSyncSettingChanged(const Setting* s);

  double m_splitICP;
  double m_splitIICP;

  bool m_splitParametersHaveOverlap = false;
  bool m_syncSettingState = false;

 protected:
  virtual FrameBufferColors getColorForSplit(int i);
};
