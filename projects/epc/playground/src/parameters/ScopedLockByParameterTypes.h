#pragma once
#include "ScopedLock.h"
#include <parameter_declarations.h>

class EditBuffer;

class ScopedLockByParameterTypes : public ScopedLock
{
 public:
  ScopedLockByParameterTypes(UNDO::Transaction* transaction, const std::vector<C15::Descriptors::ParameterType>& types, EditBuffer& eb);
};

class ScopedMonophonicParameterLock : public ScopedLockByParameterTypes
{
 public:
  ScopedMonophonicParameterLock(UNDO::Transaction* transaction, EditBuffer& eb);
};
