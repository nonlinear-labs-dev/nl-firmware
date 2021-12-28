#include "FocusAndModeSetting.h"

void FocusAndModeSetting::load(const Glib::ustring& text, Initiator initiator)
{
  auto newFocus = FocusAndMode::decode(text);
  set(newFocus);
}

Glib::ustring FocusAndModeSetting::save() const
{
  return m_focusAndMode.encode();
}

Glib::ustring FocusAndModeSetting::getDisplayString() const
{
  return "unused";
}

FocusAndMode FocusAndModeSetting::getState() const
{
  return m_focusAndMode;
}

FocusAndMode FocusAndModeSetting::getOldState() const
{
  return m_oldFocusAndMode;
}

bool FocusAndModeSetting::persistent() const
{
  return true;
}

void FocusAndModeSetting::setFocusAndModeFreeze(bool state)
{
  m_focusAndModeFrozen = state;
}

bool FocusAndModeSetting::isFocusAndModeFrozen() const
{
  return m_focusAndModeFrozen;
}

void FocusAndModeSetting::set(FocusAndMode fam)
{
  if(m_focusAndModeFrozen)
    return;

  fam.fixUnchanged(m_focusAndMode);
  restrictFocusAndMode(fam);

  if(fam != m_focusAndMode)
  {
    m_oldFocusAndMode = m_focusAndMode;
    m_focusAndMode = fam;
    notify();
  }
}

void FocusAndModeSetting::setUndoable(UNDO::Transaction* transaction, FocusAndMode focusAndMode)
{
  if(m_focusAndModeFrozen)
    return;

  if(focusAndMode.focus == UIFocus::Unchanged)
    focusAndMode.focus = m_focusAndMode.focus;

  if(focusAndMode.mode == UIMode::Unchanged)
    focusAndMode.mode = m_focusAndMode.mode;

  if(focusAndMode.focus != m_focusAndMode.focus)
    if(focusAndMode.mode == UIMode::Unchanged)
      focusAndMode.mode = UIMode::Select;

  auto swapData = UNDO::createSwapData(focusAndMode);
  auto oldSwap = UNDO::createSwapData(m_focusAndMode);

  transaction->addSimpleCommand(
      [=](UNDO::Command::State)
      {
        swapData->swapWith(m_focusAndMode);
        oldSwap->swapWith(m_oldFocusAndMode);
        notify();
      });
}

FocusAndMode FocusAndModeSetting::fixFocusAndModeWithAnys(FocusAndMode in)
{
  if(in.focus == UIFocus::Any)
    in.focus = UIFocus::Parameters;

  if(in.mode == UIMode::Any)
    in.mode = UIMode::Select;

  if(in.detail == UIDetail::Any)
    in.detail = UIDetail::Init;

  return in;
}

FocusAndMode FocusAndModeSetting::removeInvalidsFromSound(FocusAndMode in)
{
  if(in.focus == UIFocus::Sound)
  {
    if(in.mode == UIMode::Store || in.mode == UIMode::Info)
    {
      in.mode = UIMode::Select;
    }
  }

  return in;
}

FocusAndMode FocusAndModeSetting::removeEditOnFocusChange(FocusAndMode in) const
{
  auto focusAndMode = m_focusAndMode;
  const bool isDesiredParameter = (in.focus == UIFocus::Parameters);
  const bool isDesiredPresetManager = (in.focus == UIFocus::Banks) || (in.focus == UIFocus::Presets);
  const bool isDesiredSound = in.focus == UIFocus::Sound;

  const bool isCurrentPresetManager
      = (focusAndMode.focus == UIFocus::Banks) || (focusAndMode.focus == UIFocus::Presets);
  const bool isCurrentParameter = focusAndMode.focus == UIFocus::Parameters;
  const bool isCurrentSound = focusAndMode.focus == UIFocus::Sound;

  const bool switchFromPresetManagerToParameter = (isCurrentPresetManager && isDesiredParameter);
  const bool switchFromPresetManagerToSound = (isCurrentPresetManager && isDesiredSound);

  const bool switchFromParameterToSound = (isCurrentParameter && isDesiredSound);

  const bool switchFromSoundToPresetManager = (isCurrentSound && isDesiredPresetManager);
  const bool switchFromSoundToParameter = (isCurrentSound && isDesiredParameter);

  if(switchFromPresetManagerToParameter || switchFromSoundToPresetManager || switchFromSoundToParameter
     || switchFromPresetManagerToSound || switchFromParameterToSound)
  {
    if(focusAndMode.mode == UIMode::Edit)
    {
      in.mode = UIMode::Select;
    }
  }

  return in;
}

FocusAndMode FocusAndModeSetting::restrictFocusAndMode(FocusAndMode in) const
{
  in = fixFocusAndModeWithAnys(in);
  in = removeEditOnFocusChange(in);
  in = removeInvalidsFromSound(in);
  return in;
}
