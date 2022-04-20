#include "MockSettingsObject.h"

MockSettingsObject::MockSettingsObject(const Glib::ustring& file, UpdateDocumentMaster* m)
    : Settings(file, m)
{
}

UpdateDocumentContributor::tUpdateID MockSettingsObject::onChange(uint64_t flags)
{
  if(m_onChangeCB)
    return m_onChangeCB(flags);

  return Settings::onChange(flags);
}

void MockSettingsObject::setOnChangeCB(MockSettingsObject::tOnChangeCB&& cb)
{
  m_onChangeCB = cb;
}
