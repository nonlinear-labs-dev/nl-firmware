#include "EditBufferNotifier.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

OnEditBufferChangedNotifierBase::OnEditBufferChangedNotifierBase()
{
  m_connection = Application::get().getPresetManager()->getEditBuffer()->onChange(
      sigc::mem_fun(this, &OnEditBufferChangedNotifierBase::onEditBufferChanged));
}

EditBuffer *OnEditBufferChangedNotifierBase::getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer();
}
