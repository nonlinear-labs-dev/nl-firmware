#include "BaseUnitPlayMode.h"
#include "Application.h"
#include "device-settings/Settings.h"
#include "device-settings/BaseUnitUIMode.h"
#include "proxies/hwui/HWUI.h"
#include "proxies/lpc/LPCProxy.h"
#include "groups/HardwareSourcesGroup.h"
#include "presets/PresetSettings.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include <parameters/RibbonParameter.h>
#include "presets/PresetSettings.h"
#include <proxies/hwui/buttons.h>

BaseUnitPlayMode::BaseUnitPlayMode ()
{
}

BaseUnitPlayMode::~BaseUnitPlayMode ()
{
}

void BaseUnitPlayMode::setup ()
{
  setupBaseUnitUIModeButton ();
  setupBaseUnitMinusButton ();
  setupBaseUnitPlusButton ();

  setupButtonConnection (BUTTON_FUNCTION, [ = ] (gint32 buttonID, ButtonModifiers modifiers, bool state)
  {
    if (state)
    toggleTouchBehaviour();

    return true;
  });
}

void BaseUnitPlayMode::toggleTouchBehaviour ()
{
  if (auto pm = Application::get ().getPresetManager ())
  {
    auto trans = pm->getUndoScope ().startTransaction ("Set ribbon mode");
    int id = Application::get ().getLPCProxy ()->getLastTouchedRibbonParameterID ();

    if (auto ribbonParam = dynamic_cast<RibbonParameter*> (pm->getEditBuffer ()->findParameterByID (id)))
    {
      ribbonParam->undoableIncRibbonTouchBehaviour (trans->getTransaction ());
    }
  }
}

void BaseUnitPlayMode::setupBaseUnitUIModeButton ()
{
  setupButtonConnection (BUTTON_MODE, [ = ] (gint32 buttonID, ButtonModifiers modifiers, bool state)
  {
    if (state)
    Application::get ().getSettings ()->getSetting<BaseUnitUIMode> ()->inc (1, true);

    return true;
  });
}

void BaseUnitPlayMode::setupBaseUnitMinusButton ()
{
  setupButtonConnection (BUTTON_MINUS, [ = ] (gint32 buttonID, ButtonModifiers modifiers, bool state)
  {
    if (state)
    m_noteShiftState.traverse (NOTE_SHIFT_EVENT_MINUS_PRESSED);
    else
    m_noteShiftState.traverse (NOTE_SHIFT_EVENT_MINUS_RELEASED);

    return true;
  });
}

void BaseUnitPlayMode::setupBaseUnitPlusButton ()
{
  setupButtonConnection (BUTTON_PLUS, [ = ] (gint32 buttonID, ButtonModifiers modifiers, bool state)
  {
    if (state)
    m_noteShiftState.traverse (NOTE_SHIFT_EVENT_PLUS_PRESSED);
    else
    m_noteShiftState.traverse (NOTE_SHIFT_EVENT_PLUS_RELEASED);

    return true;
  });
}
