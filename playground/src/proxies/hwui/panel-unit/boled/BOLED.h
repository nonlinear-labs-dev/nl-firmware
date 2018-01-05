#pragma once

#include "proxies/hwui/OLEDProxy.h"

class Application;
class Parameter;
class Setting;

class BOLED: public OLEDProxy, public sigc::trackable
{
  public:
    BOLED();
    virtual ~BOLED();

    void init();

    bool onButtonPressed (gint32 buttonID, ButtonModifiers modifiers, bool state);
    void onRotary(signed char i);
    void setupFocusAndMode (FocusAndMode focusAndMode);
    void showUndoScreen ();

  private:
    void setupSoundScreen (FocusAndMode focusAndMode);
    void setupParameterScreen (FocusAndMode focusAndMode);
    void setupPresetScreen (FocusAndMode focusAndMode);
    void setupBankScreen (FocusAndMode focusAndMode);
};
