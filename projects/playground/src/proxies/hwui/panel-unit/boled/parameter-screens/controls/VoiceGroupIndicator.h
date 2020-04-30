#pragma once
#include <proxies/hwui/controls/Control.h>

class VoiceGroupIndicator : public Control
{
 public:
  explicit VoiceGroupIndicator(const Rect& r);
  ~VoiceGroupIndicator() override;
  bool redraw(FrameBuffer& fb) override;

 private:
  void onSoundTypeChanged();
  void onParameterChanged(const Parameter* parameter);
  void onParameterSelectionChanged(const Parameter* old, const Parameter* newParam);
  void onVoiceGroupSelectionChanged(VoiceGroup vg);

  bool drawSplit(FrameBuffer& fb);
  bool drawLayer(FrameBuffer& fb);

  VoiceGroup m_selectedVoiceGroup {};
  SoundType m_currentSoundType {};

  sigc::connection m_parameterChanged;

  bool m_shouldDraw = true;
};
