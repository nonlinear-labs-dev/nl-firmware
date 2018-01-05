#pragma once

#include "ModulationModeButton.h"

class Application;

class DimmingModulationModeButton : public ModulationModeButton
{
  private:
    typedef ModulationModeButton super;

  public:
    DimmingModulationModeButton (const Glib::ustring &text, int id);
    virtual ~DimmingModulationModeButton ();
};

