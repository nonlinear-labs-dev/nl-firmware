#include "ValueRange.h"

#include <parameters/ParameterImportConversions.h>
#include <parameter_declarations.h>

#include <utility>

ParameterImportConversions& ParameterImportConversions::get()
{
  static ParameterImportConversions conversions(true);
  return conversions;
}

ParameterImportConversions::ParameterImportConversions(bool registerDefaults)
{
  if(registerDefaults)
  {
    registerConverter(71, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerConverter(101, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerConverter(181, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerConverter(164, 3, [=](auto v, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(71, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(101, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(181, 2, [=](auto v, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(164, 3, [=](auto v, auto) { return driveV2ToV3(v); });

    registerConverter(0, 2, [=](auto v, auto) { return attackV2ToV3(v); });

    registerConverter(19, 2, [=](auto v, auto) { return attackV2ToV3(v); });

    registerConverter(38, 2, [=](auto v, auto) { return attackV2ToV3(v); });

    registerConverter(2, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(6, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(21, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(25, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(40, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(44, 2, [=](auto v, auto) { return decayV2ToV3(v); });

    registerConverter(10, 2, [=](auto v, auto) { return releaseV2ToV3(v); });

    registerConverter(29, 2, [=](auto v, auto) { return releaseV2ToV3(v); });

    registerConverter(46, 2, [=](auto v, auto) { return releaseV2ToV3(v); });

    registerConverter(127, 4,
                      [=](auto v, auto)
                      {
                        if(v > 0.995)
                          return 1.0;

                        return 0.5 + v / 1.99;
                      });

    registerMCAmountConverter(C15::PID::Comb_Flt_AP_Res, 4, [=](auto v, auto) { return v / 2.0; });

    registerConverter(C15::PID::Reverb_Color, 4,
                      [=](auto v, auto)
                      {
                        if(v > 0.5)
                          return v;

                        return v * v * v * 4.0;
                      });

    registerConverter(C15::PID::Reverb_Chorus, 4, [=](auto v, auto) { return 0.25 + v * 0.75; });

    registerConverter(C15::PID::Unison_Voices, 5, [=](auto v, auto) { return voicesV5ToV6(v); });
    registerConverter(C15::PID::Unison_Voices, 7, [=](auto v, auto soundType) { return voicesV7ToV8(v, soundType); });

    registerConverter(C15::PID::FB_Mix_Drive, 5, [=](auto v, auto) { return driveV5ToV6(v); });

    registerConverter(C15::PID::Split_Split_Point, 8, [=](auto v, auto) { return splitV8ToV9(v); });
    registerExplicitConverter({ C15::PID::Split_Split_Point, VoiceGroup::II }, 8,
                              [=](auto v, auto) { return splitIIV9ToV10(v); });

    registerConverter(C15::PID::MC_Time_A, 5, [=](auto v, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_B, 5, [=](auto v, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_C, 5, [=](auto v, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_D, 5, [=](auto v, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_E, 5, [=](auto v, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_F, 5, [=](auto v, auto) { return 0.442; });
  }
}

ParameterImportConversions::~ParameterImportConversions() = default;

tControlPositionValue ParameterImportConversions::attackV2ToV3(tControlPositionValue in) const
{
  if(in < 0.01)
    return 0.01 / 1.13 + (0.01 - 0.01 / 1.13) * (in / 0.01);

  return in;
}

tControlPositionValue ParameterImportConversions::decayV2ToV3(tControlPositionValue in) const
{
  if(in > 0.8078)
    return 1.0;

  return in + 0.1922;
}

tControlPositionValue ParameterImportConversions::releaseV2ToV3(tControlPositionValue in) const
{
  if(in > 0.8078)
    return 100.0 / 101;

  return (in + 0.1922) * 100.0 / 101;
}

tControlPositionValue ParameterImportConversions::driveV2ToV3(tControlPositionValue in) const
{
  // 0 ...25 -> 0 .. 50
  return in / 2;
}

tControlPositionValue ParameterImportConversions::driveV5ToV6(tControlPositionValue in) const
{
  // 0 .. 50 -> 0 .. 70
  return std::min((5.0 / 7.0) * in + (2.0 / 7.0), 1.0);
}

void ParameterImportConversions::registerExplicitConverter(const ParameterId& id,
                                                           ParameterImportConversions::tFileVersion srcVersion,
                                                           ParameterImportConversions::tConverter c)
{
  m_explicitConverters[id].from[srcVersion] = std::move(c);
}

void ParameterImportConversions::registerConverter(const tParameterNumber parameterID, const tFileVersion srcVersion,
                                                   tConverter c)
{
  m_converters[parameterID].from[srcVersion] = std::move(c);
}

void ParameterImportConversions::registerMCAmountConverter(const tParameterNumber parameterID,
                                                           const tFileVersion srcVersion, tConverter c)
{
  m_mcAmountConverters[parameterID].from[srcVersion] = std::move(c);
}

tControlPositionValue ParameterImportConversions::convert(const ParameterId& parameterID,
                                                          const tControlPositionValue in, const tFileVersion inVersion,
                                                          SoundType type) const
{
  if(auto itExplicit = m_explicitConverters.find(parameterID); itExplicit != m_explicitConverters.end())
  {
    return itExplicit->second.convert(in, inVersion, type);
  }

  if(auto it = m_converters.find(parameterID.getNumber()); it != m_converters.end())
  {
    return it->second.convert(in, inVersion, type);
  }

  return in;
}

tControlPositionValue ParameterImportConversions::convertMCAmount(const tParameterNumber parameterID,
                                                                  const tControlPositionValue in,
                                                                  const tFileVersion inVersion) const
{
  auto it = m_mcAmountConverters.find(parameterID);

  if(it != m_mcAmountConverters.end())
  {
    return it->second.convert(in, inVersion, SoundType::Invalid);
  }

  return in;
}

tControlPositionValue ParameterImportConversions::ConvertersBySourceFileVersion::convert(const tControlPositionValue in,
                                                                                         const tFileVersion inVersion,
                                                                                         SoundType type) const
{
  auto it = from.lower_bound(inVersion);

  if(it != from.end())
  {
    const tControlPositionValue convertedValue = it->second(in, type);
    return convert(convertedValue, it->first + 1, type);
  }

  return in;
}

tControlPositionValue ParameterImportConversions::voicesV5ToV6(tControlPositionValue in) const
{
  auto numVoicesIn = in * 11.0;
  auto res = numVoicesIn / 23.0;
  return res;
}

tControlPositionValue ParameterImportConversions::voicesV7ToV8(tControlPositionValue v, SoundType type) const
{
  if(type != SoundType::Single)
    return voicesV5ToV6(v);

  return v;
}

tControlPositionValue ParameterImportConversions::splitV8ToV9(tControlPositionValue split) const
{
  auto note = split * 59.0;
  return note / 60.0;
}

tControlPositionValue ParameterImportConversions::splitIIV9ToV10(tControlPositionValue d) const
{
  constexpr auto step = (1.0 / 60.0);
  return d + step;
}
