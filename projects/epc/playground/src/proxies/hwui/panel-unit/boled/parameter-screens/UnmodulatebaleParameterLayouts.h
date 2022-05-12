#pragma once

#include "ParameterLayout.h"

class UnmodulateableParameterLayout2 : public virtual ParameterLayout2
{
 public:
  typedef ParameterLayout2 super;
  UnmodulateableParameterLayout2();

 protected:
  void init() override;
  virtual void addButtons();
};

class UnmodulateableParameterSelectLayout2 : public ParameterSelectLayout2, public UnmodulateableParameterLayout2
{
 public:
  typedef ParameterSelectLayout2 super1;
  typedef UnmodulateableParameterLayout2 super2;
  UnmodulateableParameterSelectLayout2();

 protected:
  void init() override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

 private:
  void onParameterSelectionChanged(Parameter* oldP, Parameter* newP);
  void onScaleGroupChanged();
  void updateResetButton();
  bool resetEnabled();
  void resetScaleGroup();

  bool m_isScaleParameter = false;
  sigc::connection m_signalParameterSelectionChanged = {};
  sigc::connection m_signalScaleChanged = {};
  Button* m_resetButton = nullptr;
};

class UnmodulateableParameterEditLayout2 : public ParameterEditLayout2, public UnmodulateableParameterLayout2
{
 public:
  typedef ParameterEditLayout2 super1;
  typedef UnmodulateableParameterLayout2 super2;

  UnmodulateableParameterEditLayout2();
  void init() override;

 protected:
  ButtonMenu *createMenu(const Rect &rect) override;
  bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;
};
