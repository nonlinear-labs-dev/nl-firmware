#pragma once
#include "Enable14BitSupport.h"
#include <device-settings/EnumSetting.h>

template <typename tEnum> class MappingSetting : public EnumSetting<tEnum>
{
 public:
  MappingSetting(UpdateDocumentContributor& s, Enable14BitSupport& enable14Bit, tEnum def);

 protected:
  [[nodiscard]] bool is14BitSupportEnabled() const;

 private:
  void on14BitSupportChanged(const Setting* s);
  bool m_cached14BitSupportEnabledFlag = false;
};

template <typename tEnum>
MappingSetting<tEnum>::MappingSetting(UpdateDocumentContributor& s, Enable14BitSupport& enable14Bit, tEnum def)
    : EnumSetting<tEnum>(s, def)
{
  m_cached14BitSupportEnabledFlag = enable14Bit.get();
  enable14Bit.onChange(sigc::mem_fun(this, &MappingSetting<tEnum>::on14BitSupportChanged));
}

template <typename tEnum> void MappingSetting<tEnum>::on14BitSupportChanged(const Setting* s)
{
  if(auto enable = dynamic_cast<const Enable14BitSupport*>(s))
  {
    auto newState = enable->get();
    if(newState != m_cached14BitSupportEnabledFlag)
    {
      m_cached14BitSupportEnabledFlag = newState;
      EnumSetting<tEnum>::notify();
    }
  }
}

template <typename tEnum> bool MappingSetting<tEnum>::is14BitSupportEnabled() const
{
  return m_cached14BitSupportEnabledFlag;
}
