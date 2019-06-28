#pragma once

#include "playground.h"
#include <proxies/hwui/HWUIEnums.h>
#include <tools/Uncopyable.h>
#include <map>
#include <set>

class Application;

class UsageMode : public Uncopyable
{
 public:
  UsageMode();
  virtual ~UsageMode();

  virtual bool onButtonPressed(gint32 buttonID, ButtonModifiers modifiers, bool state);

  virtual void setup() = 0;
  virtual void setupFocusAndMode(FocusAndMode focusAndMode);
  virtual void bruteForceUpdateLeds();

 protected:
  typedef std::function<bool(gint32 button, ButtonModifiers modifiers, bool state)> tAction;

  void setupButtonConnection(gint32 buttonID, tAction action);

  Application &getApp();
  const Application &getApp() const;

#if _TESTS
  std::set<gint32> assignedAudioIDs;
#endif

 private:
  static const int num_buttons = 128;

  std::map<int, tAction> m_actions;
};
