#include <Application.h>
#include "EditBufferEditLayout.h"
#include "EditBufferInfoLayout.h"
#include <proxies/hwui/HWUI.h>
#include "proxies/hwui/TextEditUsageMode.h"

EditBufferEditLayout::EditBufferEditLayout() : EditPresetInfoLayout() {
  m_currentPreset = Application::get().getPresetManager()->getEditBuffer();
}

void EditBufferEditLayout::commit (const Glib::ustring &comment)
{
  if (m_currentPreset)
  {
    UNDO::Scope::tTransactionScopePtr scope = m_currentPreset->getUndoScope ().startTransaction ("Set EditBuffer Comment");
    m_currentPreset->undoableSetAttribute(scope->getTransaction(), "Comment", comment);
  }
}

bool EditBufferEditLayout::onButton (int i, bool down, ButtonModifiers modifiers)
{
  switch (i)
  {
    case BUTTON_A:
      if (down)
      {
        cancel ();
        Application::get ().getHWUI ()->getPanelUnit ().getEditPanel ().getBoled ().reset (new EditBufferInfoLayout());
      }
      return true;
    case BUTTON_D:
    case BUTTON_ENTER:
      if (down)
      {
        commit (m_textUsageMode->getText ());
        Application::get ().getHWUI ()->getPanelUnit ().getEditPanel ().getBoled ().reset (new EditBufferInfoLayout());
      }
      return true;
  }

  return EditPresetInfoLayout::onButton (i, down, modifiers);
}