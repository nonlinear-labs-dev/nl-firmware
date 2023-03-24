#pragma once

#include "ParameterLayout.h"
#include "UnmodulatebaleParameterLayouts.h"

class PhysicalControlSlider;

class PlayControlParameterLayout2 : public virtual ParameterLayout2
{
 public:
  typedef ParameterLayout2 super;

  PlayControlParameterLayout2();
  ~PlayControlParameterLayout2() override;

  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
  bool onRotary(int inc, ButtonModifiers modifiers) override;

 protected:
  Control* createParameterValueControl() override;

  void onInit() override;

  enum Mode : uint8_t
  {
    ValueEdit = 0,
    Select = 1,
    Behaviour = 2,
    Touch = 3
  };

  virtual void toggleMode(uint8_t desiredMode);
  virtual bool isModeSupported(uint8_t desiredMode) const;
  virtual void setMode(uint8_t desiredMode);

  uint8_t getMode() const;

 protected:
  bool isSendParameter() const;
  bool supportsBehaviour() const;
  bool supportsMode() const;

  static uint8_t s_mode;
  static int s_instanceCount;

  Control* m_currentValueDisplay = nullptr;
};

class PlayControlParameterEditLayout2 : public ParameterEditLayout2, public UnmodulateableParameterLayout2
{
 public:
  typedef ParameterEditLayout2 super1;
  typedef UnmodulateableParameterLayout2 super2;
  PlayControlParameterEditLayout2();
  void init() override;

 protected:
  ButtonMenu* createMenu(const Rect& rect) override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
};