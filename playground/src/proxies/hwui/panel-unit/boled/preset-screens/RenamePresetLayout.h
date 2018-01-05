#pragma once

#include "RenameLayout.h"
#include <libundo/undo/Transaction.h>

class Preset;

class RenamePresetLayout : public RenameLayout
{
  private:
    typedef RenameLayout super;

  public:
    typedef function<void (const Glib::ustring &)> tCommitCB;
    RenamePresetLayout (tCommitCB commitCB);

  protected:
    virtual void commit (const Glib::ustring &newName) override;
    virtual void cancel () override;
    virtual Glib::ustring getInitialText () const override;

  private:
    shared_ptr<Preset> m_currentPreset;
    tCommitCB m_commitCB;
};
