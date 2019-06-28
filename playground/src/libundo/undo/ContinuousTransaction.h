#pragma once

#include "Transaction.h"
#include <glibmm/timeval.h>
#include <memory>
#include <chrono>

namespace UNDO
{
  using std::chrono::steady_clock;

  class ContinuousTransaction : public UNDO::Transaction
  {
    typedef UNDO::Transaction super;

   public:
    ContinuousTransaction(Scope &scope, void *id, const Glib::ustring &name, size_t depth);
    virtual ~ContinuousTransaction();

    std::chrono::steady_clock::duration getAge() const;
    void *getID() const;
    void setIsNested();

    void setClosingCommand(std::unique_ptr<ContinuousTransaction> command);
    UNDO::ContinuousTransaction *getClosingCommand() const;

   protected:
    virtual void implUndoAction() const;
    virtual void implRedoAction() const;

    virtual void onImplUndoActionStart() const;
    virtual void onImplRedoActionFinished() const;

   private:
    void *m_id = nullptr;
    steady_clock::time_point m_creationTimestamp;
    std::unique_ptr<ContinuousTransaction> m_closingCommand = nullptr;
    bool m_isNested = false;
  };

} /* namespace UNDO */
