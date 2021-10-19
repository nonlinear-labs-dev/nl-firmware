#include "PanelUnitFocusAndMode.h"

void PanelUnitFocusAndMode::load(const Glib::ustring& text, Initiator initiator)
{
  auto newFocus = FocusAndMode::decode(text);
  set(newFocus);
}

Glib::ustring PanelUnitFocusAndMode::save() const
{
  return m_focusAndMode.encode();
}

Glib::ustring PanelUnitFocusAndMode::getDisplayString() const
{
  return "unused";
}

void PanelUnitFocusAndMode::set(FocusAndMode fam)
{
  if(fam != m_focusAndMode)
  {
    m_oldFocusAndMode = m_focusAndMode;
    m_focusAndMode = fam;
    notify();
  }
}

FocusAndMode PanelUnitFocusAndMode::getState() const
{
  return m_focusAndMode;
}

FocusAndMode PanelUnitFocusAndMode::getOldState() const
{
  return m_oldFocusAndMode;
}


bool PanelUnitFocusAndMode::persistent() const
{
  return true;
}
