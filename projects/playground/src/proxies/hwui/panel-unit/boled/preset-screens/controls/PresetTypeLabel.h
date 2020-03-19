#pragma once

#include "PresetLabel.h"
#include <nltools/Types.h>
#include <presets/Preset.h>

class SinglePresetTypeLabel : public PresetLabel
{
 public:
  explicit SinglePresetTypeLabel(const Rect& r);
  void drawBackground(FrameBuffer& fb) override;

  void update(const SoundType& type, bool selected, bool loaded);
};

class DualPresetTypeLabel : public Control
{
 public:
  explicit DualPresetTypeLabel(const Rect& r);
  bool redraw(FrameBuffer& fb) override;
  bool drawLayer(FrameBuffer& buffer);
  bool drawSplit(FrameBuffer& buffer);
  void update(const Preset* selected);

 private:
  bool m_inidcateI = true;
};

class PresetTypeLabel : public Control
{
 public:
  explicit PresetTypeLabel(const Rect& pos);
  void update(const Preset* newSelection);

  void drawBackground(FrameBuffer& fb) override;

  bool redraw(FrameBuffer& fb) override;

  std::unique_ptr<Control> m_currentControl;
};
