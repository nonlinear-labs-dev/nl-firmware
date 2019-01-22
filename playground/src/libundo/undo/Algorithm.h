#pragma once

#include "Transaction.h"

namespace UNDO
{

  class Algorithm
  {
   public:
    Algorithm();
    virtual ~Algorithm();

    static Transaction* getCommonRoot(Transaction* one, Transaction* other);
    static void traverse(Transaction* from, Transaction* to);
    static std::list<Transaction*> getPathAsList(Transaction* target, const Transaction* stop);
    static Transaction* find(Transaction* root, const Glib::ustring& targetID);
    static Transaction* find(Transaction* root, const void* targetID);
  };

} /* namespace UNDO */
