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
    registerConverter(C15::PID::Shp_A_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerConverter(C15::PID::Shp_B_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerConverter(C15::PID::Out_Mix_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerConverter(C15::PID::FB_Mix_Drive, 3, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(C15::PID::Shp_A_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(C15::PID::Shp_B_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(C15::PID::Out_Mix_Drive, 2, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerMCAmountConverter(C15::PID::FB_Mix_Drive, 3, [=](auto v, auto, auto) { return driveV2ToV3(v); });

    registerConverter(C15::PID::Env_A_Att, 2, [=](auto v, auto, auto) { return attackV2ToV3(v); });

    registerConverter(C15::PID::Env_B_Att, 2, [=](auto v, auto, auto) { return attackV2ToV3(v); });

    registerConverter(C15::PID::Env_C_Att, 2, [=](auto v, auto, auto) { return attackV2ToV3(v); });

    registerConverter(C15::PID::Env_A_Dec_1, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_A_Dec_2, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_B_Dec_1, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_B_Dec_2, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_C_Dec_1, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_C_Dec_2, 2, [=](auto v, auto, auto) { return decayV2ToV3(v); });

    registerConverter(C15::PID::Env_A_Rel, 2, [=](auto v, auto, auto) { return releaseV2ToV3(v); });

    registerConverter(C15::PID::Env_B_Rel, 2, [=](auto v, auto, auto) { return releaseV2ToV3(v); });

    registerConverter(C15::PID::Env_C_Rel, 2, [=](auto v, auto, auto) { return releaseV2ToV3(v); });

    registerConverter(C15::PID::Comb_Flt_AP_Res, 4, [=](auto v, auto, auto) { return combFltResonAPV4ToV5(v); });

    registerMCAmountConverter(C15::PID::Comb_Flt_AP_Res, 4, [=](auto v, auto, auto) { return v / 2.0; });

    registerConverter(C15::PID::Reverb_Color, 4, [=](auto v, auto, auto) { return reverbColorV4ToV5(v); });

    registerConverter(C15::PID::Reverb_Chorus, 4, [=](auto v, auto, auto) { return 0.25 + v * 0.75; });

    registerConverter(C15::PID::Unison_Voices, 5, [=](auto v, auto, auto) { return voicesV5ToV6(v); });
    registerConverter(C15::PID::Unison_Voices, 7,
                      [=](auto v, auto soundType, auto) { return voicesV7ToV8(v, soundType); });

    registerConverter(C15::PID::FB_Mix_Drive, 5, [=](auto v, auto, auto) { return driveV5ToV6(v); });

    registerConverter(C15::PID::Split_Split_Point, 8, [=](auto v, auto, auto) { return splitV8ToV9(v); });
    registerConverter(C15::PID::Split_Split_Point, 9, [=](auto v, auto, auto vg) { return splitIIV9ToV10(v, vg); });

    registerConverter(C15::PID::MC_Time_A, 5, [=](auto v, auto, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_B, 5, [=](auto v, auto, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_C, 5, [=](auto v, auto, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_D, 5, [=](auto v, auto, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_E, 5, [=](auto v, auto, auto) { return 0.442; });
    registerConverter(C15::PID::MC_Time_F, 5, [=](auto v, auto, auto) { return 0.442; });

    registerConverter(C15::PID::Out_Mix_Key_Pan, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Osc_A_Pitch_KT, 11, [=](auto v, auto, auto) { return pitchKTV11ToV12(v); });
    registerConverter(C15::PID::Osc_B_Pitch_KT, 11, [=](auto v, auto, auto) { return pitchKTV11ToV12(v); });
    registerConverter(C15::PID::Comb_Flt_Pitch_KT, 11, [=](auto v, auto, auto) { return pitchKTV11ToV12(v); });
    registerConverter(C15::PID::Comb_Flt_AP_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Comb_Flt_LP_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::SV_Flt_Cut_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Unison_Detune, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerMCAmountConverter(C15::PID::Unison_Detune, 11, [=](auto v, auto, auto) { return v * 0.5; });

    registerConverter(C15::PID::Env_A_Att_Vel, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Env_B_Att_Vel, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Env_C_Att_Vel, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Env_A_Time_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Env_B_Time_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Env_C_Time_KT, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Shp_A_FB_Env_C, 11, [=](auto v, auto, auto) { return v * 0.5; });
    registerConverter(C15::PID::Shp_B_FB_Env_C, 11, [=](auto v, auto, auto) { return v * 0.5; });

    registerConverter(C15::PID::Env_A_Att_Vel, 12, [=](auto v, auto, auto) { return -v; });
    registerConverter(C15::PID::Env_B_Att_Vel, 12, [=](auto v, auto, auto) { return -v; });
    registerConverter(C15::PID::Env_C_Att_Vel, 12, [=](auto v, auto, auto) { return -v; });
    registerConverter(C15::PID::Env_A_Rel_Vel, 12, [=](auto v, auto, auto) { return 1 - v; });
    registerConverter(C15::PID::Env_B_Rel_Vel, 12, [=](auto v, auto, auto) { return 1 - v; });
    registerConverter(C15::PID::Env_C_Rel_Vel, 12, [=](auto v, auto, auto) { return 1 - v; });
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
  if(auto it = m_converters.find(parameterID.getNumber()); it != m_converters.end())
  {
    return it->second.convert(in, inVersion, type, parameterID.getVoiceGroup());
  }

  return in;
}

tControlPositionValue ParameterImportConversions::convertMCAmount(const ParameterId& parameterID,
                                                                  const tControlPositionValue in,
                                                                  const tFileVersion inVersion) const
{
  auto it = m_mcAmountConverters.find(parameterID.getNumber());

  if(it != m_mcAmountConverters.end())
  {
    return it->second.convert(in, inVersion, SoundType::Invalid, parameterID.getVoiceGroup());
  }

  return in;
}

tControlPositionValue ParameterImportConversions::ConvertersBySourceFileVersion::convert(const tControlPositionValue in,
                                                                                         const tFileVersion inVersion,
                                                                                         SoundType type,
                                                                                         VoiceGroup vg) const
{
  auto it = from.lower_bound(inVersion);

  if(it != from.end())
  {
    const tControlPositionValue convertedValue = it->second(in, type, vg);
    return convert(convertedValue, it->first + 1, type, vg);
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

tControlPositionValue ParameterImportConversions::splitIIV9ToV10(tControlPositionValue d, VoiceGroup vg) const
{
  constexpr auto step = (1.0 / 60.0);
  if(vg == VoiceGroup::II)
    return d + step;
  else
    return d;
}

tControlPositionValue ParameterImportConversions::combFltResonAPV4ToV5(tControlPositionValue d) const
{
  if(d > 0.995)
    return 1.0;

  return 0.5 + d / 1.99;
}

tControlPositionValue ParameterImportConversions::reverbColorV4ToV5(tControlPositionValue d) const
{
  if(d > 0.5)
    return d;

  return d * d * d * 4.0;
}

tControlPositionValue ParameterImportConversions::pitchKTV11ToV12(tControlPositionValue v) const
{
  auto displayValueIn = v * 105.0;
  return displayValueIn / 200.0;
}
