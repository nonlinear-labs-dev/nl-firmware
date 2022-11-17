#include "MockSettingsObject.h"

static HardwareFeatures s_hwFeatures;

MockSettingsObject::MockSettingsObject(const Glib::ustring& file, UpdateDocumentMaster* m)
    : Settings(file, m, s_hwFeatures)
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
