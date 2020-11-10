#pragma once
#include <libundo/undo/Transaction.h>
#include "Setting.h"

class SplitPointSyncParameters : public Setting
{
 public:
  using tSlot = sigc::slot<void, const Setting*, UNDO::Transaction*>;

  explicit SplitPointSyncParameters(Settings& p);

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

  sigc::connection onChangeWithTransaction(const tSlot& cb);

  bool get() const;
  void setState(bool s);
  void setState(bool s, UNDO::Transaction* t);

 private:
  sigc::signal<void, const Setting*, UNDO::Transaction*> m_signalWithTransaction;
  bool m_state = true;
};
