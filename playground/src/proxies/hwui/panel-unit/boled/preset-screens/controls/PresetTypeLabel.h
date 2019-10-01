#pragma once
#include "PresetLabel.h"

class PresetTypeLabel : public PresetLabel
{
 public:
  explicit PresetTypeLabel(const Rect& pos);
  void update(const SoundType& type, bool selected, bool loaded);

  void drawBackground(FrameBuffer &fb) override;

  std::string typeToString(const SoundType& type);
};
