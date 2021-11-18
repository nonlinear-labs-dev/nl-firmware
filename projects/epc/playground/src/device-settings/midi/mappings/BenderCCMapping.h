#pragma once
#include <device-settings/midi/mappings/MappingSetting.h>
#include <nltools/Types.h>

class Enable14BitSupport;

class BenderCCMapping : public MappingSetting<BenderCC>
{
 public:
  BenderCCMapping(UpdateDocumentContributor& u, Enable14BitSupport& enable14Bit);
  const std::vector<Glib::ustring>& enumToString() const override;
  const std::vector<Glib::ustring>& enumToDisplayString() const override;
};
