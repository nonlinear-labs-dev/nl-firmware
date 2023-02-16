#pragma once

#include <libundo/undo/Transaction.h>
#include <ParameterId.h>
#include <GroupId.h>

class Parameter;

class ScopedLock
{
 public:
  explicit ScopedLock(UNDO::Transaction* trans);
  ScopedLock(UNDO::Transaction* trans, const std::vector<Parameter*>& params);
  virtual ~ScopedLock();

  void addLock(const ParameterId& id);
  void addLock(Parameter* param);
  void addGroupLock(const GroupId& id);

  void addLocks(const std::vector<Parameter*>& params);
  void addLocks(const std::vector<ParameterId>& paramIds);

 private:
  UNDO::Transaction* m_transaction;
  std::vector<Parameter*> m_oldLockedParameters;
};
