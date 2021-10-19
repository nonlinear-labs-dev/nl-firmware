#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <functional>

class Label;
class PresetListEntry;
class Preset;
class Bank;

class PresetListContent : public ControlWithChildren
{
 private:
  typedef ControlWithChildren super;

 public:
  explicit PresetListContent(const Rect &pos);
  ~PresetListContent() override;

  void setup(const Bank *bank, size_t focussedPresetPos);
  bool animateSomePreset(const Preset* preset, std::function<void()> cb);

 protected:
  bool isTransparent() const override;

 private:
  Preset *getPresetAtPosition(const Bank *bank, int pos) const;

  PresetListEntry *m_firstPreset = nullptr;
  PresetListEntry *m_secondPreset = nullptr;
  PresetListEntry *m_thirdPreset = nullptr;
  Label *m_emptyLabel = nullptr;
};
