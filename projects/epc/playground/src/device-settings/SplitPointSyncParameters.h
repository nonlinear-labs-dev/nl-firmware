#pragma once
#include <libundo/undo/Transaction.h>
#include <presets/EditBuffer.h>
#include "Setting.h"

class Preset;

class SplitPointSyncParameters : public Setting, public sigc::trackable
{
 public:
  using tSlot = sigc::slot<void, const Setting*, UNDO::Transaction*>;

  explicit SplitPointSyncParameters(Settings& s);

  void load(const Glib::ustring& text, Initiator initiator) override;
  Glib::ustring save() const override;
  Glib::ustring getDisplayString() const override;

  bool get() const;
  void setState(bool s);
  void init() override;

  void undoableSet(UNDO::Transaction* transaction, bool newState);

 private:
  void onSoundConverted(SoundType type);
  bool m_state = true;
};
