#include <Application.h>
#include "ScopedLock.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"

ScopedLock::ScopedLock(UNDO::Transaction* trans)
    : m_transaction { trans }
{
}

ScopedLock::~ScopedLock()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  for(auto [parameterId, oldLockState] : m_oldLockStates)
  {
    if(!oldLockState)
    {
      if(auto param = eb->findParameterByID(parameterId))
      {
        param->undoableUnlock(m_transaction);
      }
    }
  }
}

ScopedLock::ScopedLock(UNDO::Transaction* trans, std::vector<ParameterId>& ids)
    : ScopedLock(trans)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  for(const auto& id : ids)
  {
    addLock(id);
  }
}

void ScopedLock::addLock(const ParameterId& id)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto param = eb->findParameterByID(id))
  {
    addLock(param);
  }
}

void ScopedLock::addLock(Parameter* param)
{
  if(param)
  {
    m_oldLockStates[param->getID()] = param->isLocked();
    param->undoableLock(m_transaction);
  }
}

void ScopedLock::addGroupLock(const GroupId& id)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  if(auto group = eb->getParameterGroupByID(id))
  {
    for(auto& p : group->getParameters())
    {
      addLock(p);
    }
  }
}

void ScopedLock::addLocks(const std::vector<Parameter*>& params)
{
  for(auto p : params)
  {
    addLock(p);
  }
}
