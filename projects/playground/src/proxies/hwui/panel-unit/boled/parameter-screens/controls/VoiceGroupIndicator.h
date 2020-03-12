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
  void onVoiceGroupChanged(VoiceGroup newVoiceGroup);

  void onParameterSelectionChanged(const Parameter* oldParam, const Parameter* newParam);

  bool drawSplit(FrameBuffer& fb);
  bool drawLayer(FrameBuffer& fb);

  VoiceGroup m_selectedVoiceGroup{};
  SoundType m_currentSoundType{};

  sigc::connection m_voiceGroupChanged;
  sigc::connection m_soundTypeChanged;
  sigc::connection m_parameterSelectionChanged;

  bool m_shouldDraw = true;
};
