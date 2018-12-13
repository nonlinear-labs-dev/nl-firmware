#pragma once

#include <http/UpdateDocumentContributor.h>

namespace UNDO
{
  class Transaction;
}

class AttributesOwner : public UpdateDocumentContributor
{
 public:
  typedef std::map<std::string, std::string> tAttributes;

  AttributesOwner(UpdateDocumentContributor *parent);
  ~AttributesOwner() override;

  void setAttribute(UNDO::Transaction *transaction, const std::string &key, const ustring &value);
  void copyFrom(UNDO::Transaction *transaction, const AttributesOwner *other);
  void clear(UNDO::Transaction *transaction);

  Glib::ustring getAttribute(const Glib::ustring &key, const Glib::ustring &def) const;
  const tAttributes &getAttributes() const;
  size_t getHash() const;

  void writeDocument(Writer &writer, tUpdateID knownRevision) const override;

 private:
  tAttributes m_attributes;

  friend class AttributesOwnerSerializer;
};
