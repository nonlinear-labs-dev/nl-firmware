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
  void onVoiceGroupChanged(const VoiceGroup& voiceGroup);
  void onParameterChanged(const Parameter* parameter);
  void onParameterSelectionChanged(const Parameter* old, const Parameter* newParameter);

  bool drawSplit(FrameBuffer& fb);
  bool drawLayer(FrameBuffer& fb);

  VoiceGroup m_selectedVoiceGroup {};
  SoundType m_currentSoundType {};

  sigc::connection m_soundTypeChanged;
  sigc::connection m_parameterSelectionChanged;
  sigc::connection m_parameterChanged;
  sigc::connection m_voiceGroupChanged;

  bool m_shouldDraw = true;
};
