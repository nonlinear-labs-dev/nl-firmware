#pragma once

#include "EnumSetting.h"

enum class PedalTypes
{
  PotTipActive = 0,
  PotRingActive = 1,
  SwitchClosing = 2,
  SwitchOpening = 3,
};

class PedalType : public EnumSetting<PedalTypes>
{
 private:
  typedef EnumSetting<PedalTypes> super;

 public:
  PedalType(UpdateDocumentContributor &settings, uint16_t lpcKey);
  virtual ~PedalType();

  const std::vector<Glib::ustring> &enumToDisplayString() const override;

 private:
  void sendToLPC() const override;
  const std::vector<Glib::ustring> &enumToString() const override;

  uint16_t m_lpcKey;
};
