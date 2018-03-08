#include "AnyParameterLockedIndicator.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/panel-unit/boled/BOLED.h"

AnyParameterLockedIndicator::AnyParameterLockedIndicator(const Rect &pos) :
    super(pos),
    m_calcHasLocks(1, std::bind(&AnyParameterLockedIndicator::calcHasLocks, this))
{
  Application::get().getPresetManager()->getEditBuffer()->onChange(sigc::mem_fun(this, &AnyParameterLockedIndicator::update));
  update();
}

AnyParameterLockedIndicator::~AnyParameterLockedIndicator()
{
}

bool AnyParameterLockedIndicator::isVisible () const
{
  return m_hasLocks == true;
}

void AnyParameterLockedIndicator::calcHasLocks()
{
  auto hasLocks = Application::get().getPresetManager()->getEditBuffer()->hasLocks();

  if(hasLocks != m_hasLocks)
  {
    m_hasLocks = hasLocks;
    setText(hasLocks ? "L" : "");
  }
}

void AnyParameterLockedIndicator::update()
{
  m_calcHasLocks.trigger();
}


