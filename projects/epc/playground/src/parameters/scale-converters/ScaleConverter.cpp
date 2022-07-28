#include "ScaleConverter.h"
#include "BaseKeyScaleConverter.h"
#include "BipolarParabolic100PercentScaleConverter.h"
#include "CombDecayBipolarMsScaleConverter.h"
#include "EnvelopeAttackDecayTimeMSScaleConverter.h"
#include "EnvelopeReleaseTimeMSScaleConverter.h"
#include "Fine105PercentScaleConverter.h"
#include "Fine12STScaleConverter.h"
#include "FineBipolar1200CTScaleConverter.h"
#include "FineBipolar12STScaleConverter.h"
#include "FineBipolar80StScaleConverter.h"
#include "KeyScaleConverter.h"
#include "LegatoScaleConverter.h"
#include "Linear0To140StScaleConverter.h"
#include "Linear100PercentScaleConverter.h"
#include "Linear12CountScaleConverter.h"
#include "Linear180DegreeScaleConverter.h"
#include "Linear20To100StScaleConverter.h"
#include "Linear20To140StScaleConverter.h"
#include "Linear24StScaleConverter.h"
#include "Linear24To120StScaleConverter.h"
#include "Linear25DbScaleConverter.h"
#include "Linear360DegreeScaleConverter.h"
#include "Linear40To140StScaleConverter.h"
#include "Linear50DbScaleConverter.h"
#include "Linear60DbScaleConverter.h"
#include "Linear60KeyScaleConverter.h"
#include "Linear60StScaleConverter.h"
#include "Linear60To140StScaleConverter.h"
#include "Linear70DbScaleConverter.h"
#include "Linear80To140StScaleConverter.h"
#include "Linear96StScaleConverter.h"
#include "LinearBipolar100PercentScaleConverter.h"
#include "LinearBipolar100StScaleConverter.h"
#include "LinearBipolar120StScaleConverter.h"
#include "LinearBipolar140StScaleConverter.h"
#include "LinearBipolar160StScaleConverter.h"
#include "LinearBipolar1DbstScaleConverter.h"
#include "LinearBipolar200PercentScaleConverter.h"
#include "LinearBipolar24DbScaleConverter.h"
#include "LinearBipolar33PercentScaleConverter.h"
#include "LinearBipolar36StScaleConverter.h"
#include "LinearBipolar48DbScaleConverter.h"
#include "LinearBipolar48StScaleConverter.h"
#include "LinearBipolar50DbScaleConverter.h"
#include "LinearBipolar50PercentScaleConverter.h"
#include "LinearBipolar60DbScaleConverter.h"
#include "LinearBipolar60StScaleConverter.h"
#include "LinearBipolar66PercentScaleConverter.h"
#include "LinearBipolar70DbScaleConverter.h"
#include "LinearBipolar80StScaleConverter.h"
#include "LinearBipolar96StScaleConverterCoarse.h"
#include "LinearBipolar96StScaleConverterFine.h"
#include "LinearBipolarInverted60DBTScaleConverter.h"
#include "LinearMinus50To0DbScaleConverter.h"
#include "MonoPriorityScaleConverter.h"
#include "OnOffScaleConverter.h"
#include "Parabolic10HzScaleConverter.h"
#include "Parabolic2000MsScaleConverter.h"
#include "Parabolic50MsScaleConverter.h"
#include "ParabolicGainDbScaleConverter.h"
#include "PhaseBipolar180DegreeScaleConverter.h"
#include "PhaseBipolar360DegreeScaleConverter.h"
#include "PitchCombLinearStModulationScaleConverter.h"
#include "PitchCombLinearStScaleConverter.h"
#include "PitchOscLinearStScaleConverter.h"
#include "PitchOscLinearStScaleModulationConverter.h"
#include "SplitPointScaleConverter.h"
#include "dimension/VoicesDimension.h"
#include "nltools/Assert.h"
#include <parameters/value/QuantizedValue.h>

ScaleConverter::ScaleConverter(const Dimension &dim) : m_dimension(dim) {}

ScaleConverter::~ScaleConverter() {}

const Dimension &ScaleConverter::getDimension() const { return m_dimension; }

size_t ScaleConverter::hash() const {
  return m_dimension.hash() ^ typeid(*this).hash_code();
}

const ScaleConverter::tControlPositionRange &
ScaleConverter::getControlPositionRangeUnipolar() {
  static tControlPositionRange s(0, 1);
  return s;
}

const ScaleConverter::tControlPositionRange &
ScaleConverter::getControlPositionRangeBipolar() {
  static tControlPositionRange s(-1, 1);
  return s;
}

ScaleConverter::tTcdRange ScaleConverter::getTcdRange() const {
  ScaleConverter::tControlPositionRange cpRange = getControlPositionRange();
  return tTcdRange(controlPositionToTcd(cpRange.getMin()),
                   controlPositionToTcd(cpRange.getMax()));
}

tControlPositionValue
ScaleConverter::getCoarseDenominator(const QuantizedValue &v) const {
  return v.getCoarseDenominator();
}

tControlPositionValue
ScaleConverter::getFineDenominator(const QuantizedValue &v) const {
  return v.getFineDenominator();
}

ScaleConverter::tControlPositionRange
ScaleConverter::getControlPositionRange() const {
  return isBiPolar() ? getControlPositionRangeBipolar()
                     : getControlPositionRangeUnipolar();
}

const ScaleConverter *
ScaleConverter::getByEnum(C15::Properties::DisplayScalingType e) {
  switch (e) {
  case C15::Properties::DisplayScalingType::None:
    return nullptr;
  case C15::Properties::DisplayScalingType::LINEAR_12_KEYS:
    return get<BaseKeyScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_61_KEYS:
    return get<SplitPointScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_0_60_KEY:
    return get<Linear60KeyScaleConverter>();
  case C15::Properties::DisplayScalingType::SWITCH_ON_OFF:
    return get<OnOffScaleConverter>();
  case C15::Properties::DisplayScalingType::SWITCH_PRIO:
    return get<MonoPriorityScaleConverter>();
  case C15::Properties::DisplayScalingType::MODES_LEGATO:
    return get<LegatoScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_1200_CENT:
    return get<KeyScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_2400_CENT:
    return get<FineBipolar1200CTScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_50_DB:
    return get<Linear50DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_50_DB:
    return get<LinearBipolar50DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_MINUS_50_0_DB:
    return get<LinearMinus50To0DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_24_DB:
    return get<LinearBipolar24DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_48_DB:
    return get<LinearBipolar48DbScaleConverter>();
  case C15::Properties::DisplayScalingType::PARABOLIC_GAIN_DB:
    return get<ParabolicGainDbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_70_DB:
    return get<Linear70DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_70_DB:
    return get<LinearBipolar70DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_60_DB:
    return get<LinearBipolar60DbScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_INVERTED_60_DBT:
    return get<LinearBipolarInverted60DbtScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_1_DBST:
    return get<LinearBipolar1DbstScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_180_DEG:
    return get<Linear180DegreeScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_360_DEG:
    return get<Linear360DegreeScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_360_DEG:
    return get<PhaseBipolar360DegreeScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_180_DEG:
    return get<PhaseBipolar180DegreeScaleConverter>();
  case C15::Properties::DisplayScalingType::PARABOLIC_10_HZ:
    return get<Parabolic10HzScaleConverter>();
  case C15::Properties::DisplayScalingType::COMB_DECAY_BIPOLAR_MS:
    return get<CombDecayBipolarMsScaleConverter>();
  case C15::Properties::DisplayScalingType::ENVELOPE_TIME_MS:
    return get<EnvelopeAttackDecayTimeMSScaleConverter>();
  case C15::Properties::DisplayScalingType::ENVELOPE_TIME_MS_INF:
    return get<EnvelopeReleaseTimeMSScaleConverter>();
  case C15::Properties::DisplayScalingType::PARABOLIC_50_MS:
    return get<Parabolic50MsScaleConverter>();
  case C15::Properties::DisplayScalingType::PARABOLIC_2000_MS:
    return get<Parabolic2000MsScaleConverter>();
  case C15::Properties::DisplayScalingType::FINE_BIPOLAR_48_ST:
    return get<LinearBipolar48StScaleConverter>();
  case C15::Properties::DisplayScalingType::FINE_BIPOLAR_96_ST:
    return get<LinearBipolar96StScaleConverterFine>();
  case C15::Properties::DisplayScalingType::FINE_BIPOLAR_80_ST:
    return get<FineBipolar80StScaleConverter>();
  case C15::Properties::DisplayScalingType::FINE_BIPOLAR_160_ST:
    return get<FineBipolar160StScaleConverter>(); // This ScaleConverter was
                                                  // new! This was never used!
                                                  // All mod-amounts of
                                                  // Parameters were using
                                                  // default % scale converter
  case C15::Properties::DisplayScalingType::INTEGRAL_BIPOLAR_60_ST:
    return get<LinearBipolar60StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_36_ST:
    return get<LinearBipolar36StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_60_ST:
    return get<LinearBipolar60StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_96_ST:
    return get<Linear96StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_96_ST:
    return get<LinearBipolar96StScaleConverterCoarse>();
  case C15::Properties::DisplayScalingType::LINEAR_0_140_ST:
    return get<Linear0To140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_140_ST:
    return get<LinearBipolar140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_20_100_ST:
    return get<Linear20To100StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_20_140_ST:
    return get<Linear20To140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_24_120_ST:
    return get<Linear24To120StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_40_140_ST:
    return get<Linear40To140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_60_140_ST:
    return get<Linear60To140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_80_140_ST:
    return get<Linear80To140StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_72_ST:
    return get<LinearBipolar72StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_80_ST:
    return get<FineBipolar80StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_100_ST:
    return get<LinearBipolar100StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_120_ST:
    return get<LinearBipolar120StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_160_ST:
    return get<LinearBipolar160StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_200_ST:
    return get<LinearBipolar100StScaleConverter>();
  case C15::Properties::DisplayScalingType::NUMERIC_120_ST:
    return get<PitchCombLinearStScaleConverter>();
  case C15::Properties::DisplayScalingType::NUMERIC_BIPOLAR_120_ST:
    return get<PitchCombLinearStModulationScaleConverter>();
  case C15::Properties::DisplayScalingType::NUMERIC_150_ST:
    return get<PitchOscLinearStScaleConverter>();
  case C15::Properties::DisplayScalingType::NUMERIC_BIPOLAR_150_ST:
    return get<PitchOscLinearStScaleModulationConverter>();
  case C15::Properties::DisplayScalingType::VERY_FINE_24_ST:
    return get<Fine24STScaleConverter>();
  case C15::Properties::DisplayScalingType::VERY_FINE_BIPOLAR_24_ST:
    return get<FineBipolar24STScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_0_60_ST:
    return get<Linear60StScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_24_COUNT:
    return get<LinearCountScaleConverter<24, VoicesDimension>>();
  case C15::Properties::DisplayScalingType::FINE_200_PERCENT:
    return get<Fine200PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_100_PERCENT:
    return get<Linear100PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_33_PERCENT:
    return get<LinearBipolar33PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_50_PERCENT:
    return get<LinearBipolar50PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_66_PERCENT:
    return get<LinearBipolar66PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::PARABOLIC_BIP_100_PERCENT:
    return get<BipolarParabolic100PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_200_PERCENT:
    return get<Linear200PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_100_PERCENT:
    return get<LinearBipolar100PercentScaleConverter>();
  case C15::Properties::DisplayScalingType::LINEAR_BIPOLAR_200_PERCENT:
    return get<LinearBipolar200PercentScaleConverter>();
  }
  return nullptr;
}
