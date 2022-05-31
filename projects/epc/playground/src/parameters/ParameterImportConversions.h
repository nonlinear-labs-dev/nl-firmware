#pragma once

#include "playground.h"
#include <nltools/Types.h>
#include <functional>
#include <map>
#include <ParameterId.h>

class ParameterImportConversions
{
 public:
  static ParameterImportConversions &get();

  typedef std::function<tControlPositionValue(tControlPositionValue, SoundType, VoiceGroup)> tConverter;
  typedef int tFileVersion;
  typedef uint16_t tParameterNumber;

  [[nodiscard]] tControlPositionValue convert(const ParameterId& parameterID, tControlPositionValue in,
                                              tFileVersion inVersion, SoundType type) const;
  [[nodiscard]] tControlPositionValue convertMCAmount(const ParameterId& parameterID, const tControlPositionValue in,
                                                      const tFileVersion inVersion) const;

  virtual ~ParameterImportConversions();


 private:
  explicit ParameterImportConversions(bool registerDefaults);

  void registerExplicitConverter(const ParameterId& id, tFileVersion srcVersion, tConverter c);
  void registerConverter(tParameterNumber parameterID, tFileVersion srcVersion, tConverter c);
  void registerMCAmountConverter(tParameterNumber parameterID, tFileVersion srcVersion, tConverter c);

  [[nodiscard]] tControlPositionValue attackV2ToV3(tControlPositionValue in) const;
  [[nodiscard]] tControlPositionValue decayV2ToV3(tControlPositionValue in) const;
  [[nodiscard]] tControlPositionValue releaseV2ToV3(tControlPositionValue in) const;
  [[nodiscard]] tControlPositionValue driveV2ToV3(tControlPositionValue in) const;
  [[nodiscard]] tControlPositionValue driveV5ToV6(tControlPositionValue in) const;
  [[nodiscard]] tControlPositionValue voicesV5ToV6(tControlPositionValue unisonVoices) const;
  [[nodiscard]] tControlPositionValue voicesV7ToV8(tControlPositionValue unisonVoices, SoundType type) const;
  [[nodiscard]] tControlPositionValue splitV8ToV9(tControlPositionValue split) const;
  [[nodiscard]] tControlPositionValue splitIIV9ToV10(tControlPositionValue d, VoiceGroup vg) const;
  [[nodiscard]] tControlPositionValue combFltResonAPV4ToV5(tControlPositionValue d) const;
  [[nodiscard]] tControlPositionValue reverbColorV4ToV5(tControlPositionValue d) const;

  struct ConvertersBySourceFileVersion
  {
    [[nodiscard]] tControlPositionValue convert(tControlPositionValue in, tFileVersion inVersion, SoundType type, VoiceGroup vg) const;
    std::map<tFileVersion, tConverter> from;
  };

  std::map<tParameterNumber, ConvertersBySourceFileVersion> m_converters;
  std::map<tParameterNumber, ConvertersBySourceFileVersion> m_mcAmountConverters;

  friend class TestableImportConversions;
  tControlPositionValue pitchKTV11ToV12(tControlPositionValue v);
};
