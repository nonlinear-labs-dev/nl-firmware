#pragma once
#include <proxies/hwui/controls/LabelRegular8.h>

class SplitPointOverlapIndicator : public LabelRegular8
{
 public:
  explicit SplitPointOverlapIndicator(const Rect& pos);
  ~SplitPointOverlapIndicator() override;
  int getZPriority() const override;

 private:
  void onSplitIChanged(const Parameter* splitI);
  void onSplitIIChanged(const Parameter* splitII);

  double m_splitICP;
  double m_splitIICP;

  sigc::connection splitIConnection;
  sigc::connection splitIIConnection;
};
