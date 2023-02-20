#pragma once

#include "playground.h"
#include "GroupId.h"
#include "http/UpdateDocumentContributor.h"
#include "tools/IntrusiveList.h"
#include "sync/SyncedItem.h"
#include "tools/Signal.h"
#include "ParameterId.h"
#include "parameter_descriptor.h"

namespace UNDO
{
  class Transaction;
}

class PresetParameterGroup;
class ParameterGroupSet;
class Parameter;

class ParameterGroup : public UpdateDocumentContributor, public IntrusiveListItem<ParameterGroup *>, public SyncedItem
{
 private:
  typedef UpdateDocumentContributor super;

 public:
  ParameterGroup(ParameterGroupSet *parent, C15::ParameterGroupDescriptor desc, VoiceGroup vg);
  ~ParameterGroup() override;

  virtual void init();

  Glib::ustring getShortName() const;
  Glib::ustring getLongName() const;

  typedef Parameter *tParameterPtr;

  tParameterPtr getParameterByID(const ParameterId &id) const;

  tParameterPtr findParameterByID(const ParameterId &id) const;

  template <typename tParameterPtrT> inline tParameterPtrT *findAndCastParameterByID(const ParameterId &id) const
  {
    if(auto p = getParameterByID(id))
      return dynamic_cast<tParameterPtrT *>(p);
    return nullptr;
  }

  void forEachParameter(const std::function<void(Parameter *p)> &cb);
  const IntrusiveList<tParameterPtr> &getParameters() const;
  IntrusiveList<tParameterPtr> &getParameters();

  void undoableReset(UNDO::Transaction *transaction, Initiator initiator);

  void undoableLock(UNDO::Transaction *transaction);
  void undoableUnlock(UNDO::Transaction *transaction);
  void undoableToggleLock(UNDO::Transaction *transaction);
  bool areAllParametersLocked() const;
  bool isAnyParameterLocked() const;
  bool isAnyParameterChanged() const;

  virtual void undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount);
  void undoableSetDefaultValues(UNDO::Transaction *transaction, const PresetParameterGroup *values);

  GroupId getID() const;
  size_t getHash() const;

  virtual void copyFrom(UNDO::Transaction *transaction, const PresetParameterGroup *other);
  virtual void copyFrom(UNDO::Transaction *transaction, const ParameterGroup *other);

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

  virtual tUpdateID onChange(uint64_t flags = UpdateDocumentContributor::ChangeFlags::Generic) override;

  // CALLBACKS
  sigc::connection onGroupChanged(const sigc::slot<void> &slot);

  VoiceGroup getVoiceGroup() const;

  void undoableLoadDefault(UNDO::Transaction *transaction, Defaults mode);

  bool isPolyphonic() const;

 protected:
  template <typename T> inline T *appendParameter(T *p)
  {
    m_parameters.append(p);
    return p;
  }

  nlohmann::json serialize() const override;

 private:
  const GroupId m_id;
  const char *m_shortName;
  const char *m_longName;
  const char *m_webUIName;
  IntrusiveList<tParameterPtr> m_parameters;
  Signal<void> m_signalGroupChanged;
};
