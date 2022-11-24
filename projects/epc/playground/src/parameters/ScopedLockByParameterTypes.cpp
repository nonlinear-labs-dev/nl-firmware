#include "ScopedLockByParameterTypes.h"
#include <presets/EditBuffer.h>

ScopedLockByParameterTypes::ScopedLockByParameterTypes(UNDO::Transaction* transaction,
                                                       const std::vector<C15::Descriptors::ParameterType>& types,
                                                       EditBuffer& eb)
    : ScopedLock(transaction)
{

  for(auto& t : types)
  {
    for(auto& p : eb.findAllParametersOfType(t))
    {
      addLock(p);
    }
  }
}
