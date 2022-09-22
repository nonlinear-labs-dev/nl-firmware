#include "MockSettingsObject.h"

MockSettingsObject::MockSettingsObject(const Glib::ustring& file, UpdateDocumentMaster* m)
    : Settings(file, m,
               [this]() -> HardwareFeatures&
               {
                 m_hwFeatures = std::make_unique<HardwareFeatures>();
                 return *m_hwFeatures;
               }())
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
