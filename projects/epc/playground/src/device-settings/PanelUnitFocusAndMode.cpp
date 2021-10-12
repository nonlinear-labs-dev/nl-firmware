#include "PanelUnitFocusAndMode.h"

void PanelUnitFocusAndMode::load(const Glib::ustring& text, Initiator initiator)
{
  auto newFocus = FocusAndMode::decode(text);
  if(newFocus != m_focusAndMode)
  {
    m_focusAndMode = newFocus;
    notify();
  }
}

Glib::ustring PanelUnitFocusAndMode::save() const
{
  return m_focusAndMode.encode();
}

Glib::ustring PanelUnitFocusAndMode::getDisplayString() const
{
  return "unused";
}

void PanelUnitFocusAndMode::updateFromHWUI(FocusAndMode fam)
{
  if(fam != m_focusAndMode)
  {
    m_focusAndMode = fam;
    notify();
  }
}

FocusAndMode PanelUnitFocusAndMode::getState() const
{
  return m_focusAndMode;
}

bool PanelUnitFocusAndMode::persistent() const
{
  return true;
}
