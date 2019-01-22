#pragma once

namespace UNDO
{
  class Transaction;

  class TransactionCreationScope
  {
   public:
    TransactionCreationScope(Transaction* transaction, bool ownTransaction = false);
    virtual ~TransactionCreationScope();

    Transaction* getTransaction() const;

   private:
    Transaction* m_transaction = nullptr;
    bool m_transactionOwned = false;
  };

} /* namespace UNDO */
