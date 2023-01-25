#pragma once
#include <proxies/hwui/controls/Control.h>
#include "proxies/hwui/controls/PNGControl.h"

class VoiceGroupIndicator : public Control
{
 public:
  explicit VoiceGroupIndicator(const Rect& r, bool allowLoadToPart, bool alwaysDraw = false);
  ~VoiceGroupIndicator() override;
  bool redraw(FrameBuffer& fb) override;
  VoiceGroup getCurrentDisplayedVoiceGroup() const;

 private:
  void onSoundTypeChanged(SoundType type);
  void onParameterChanged(const Parameter* parameter);
  void onParameterSelectionChanged(const Parameter* old, const Parameter* newParam);
  void onVoiceGroupSelectionChanged(VoiceGroup vg);
  void onLoadModeChanged(bool loadModeActive);

  bool drawSplit(FrameBuffer& fb);
  bool drawLayer(FrameBuffer& fb);
  bool drawSingle(FrameBuffer& fb);

  bool isLayerPartMuted(VoiceGroup vg) const;
  bool isSingleFXNotActive(VoiceGroup vg) const;
  bool shouldDraw();

  VoiceGroup m_selectedVoiceGroup {};
  SoundType m_currentSoundType {};
  bool m_inLoadToPart = false;

  sigc::connection m_parameterChanged;
  const Parameter* m_param = nullptr;
  const bool m_allowLoadToPart;
  const bool m_alwaysDraw;
  void drawCrossForLayer(FrameBuffer& fb, int centerX, int centerY);
  void drawCrossForSingle(FrameBuffer& fb, int centerX, int centerY);

  PNGControl m_fxUnused;
};
