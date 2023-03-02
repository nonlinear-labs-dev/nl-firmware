#include "ModulateableParameter.h"
#include "MacroControlParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "use-cases/ModParameterUseCases.h"
#include "ParameterFactory.h"
#include <libundo/undo/Transaction.h>
#include <device-settings/DebugLevel.h>
#include <xml/Writer.h>
#include <presets/ParameterGroupSet.h>
#include <presets/PresetParameter.h>
#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>
#include <proxies/audio-engine/AudioEngineProxy.h>
#include <parameters/messaging/ParameterMessageFactory.h>
#include <presets/recall/RecallParameter.h>
#include <proxies/hwui/HWUI.h>
#include <presets/PresetManager.h>
#include <http/UndoScope.h>

ModulateableParameter::ModulateableParameter(ParameterGroup *group, const ParameterId &id)
    : Parameter(group, id)
    , m_modulationAmount(0)
    , m_modulationBase(0)
    , m_modSource(MacroControls::NONE)
    , m_modulationAmountScaleConverter { ScaleConverter::getByEnum(
          ParameterDB::getModulationAmountDisplayScalingType(id)) }
{
}

ModulateableParameter::~ModulateableParameter() = default;

size_t ModulateableParameter::getHash() const
{
  size_t hash = super::getHash();
  hash_combine(hash, m_modulationAmount);
  hash_combine(hash, m_modulationBase);
  hash_combine(hash, (int) m_modSource);
  return hash;
}

tDisplayValue ModulateableParameter::getModulationBase() const
{
  return m_modulationBase;
}

void ModulateableParameter::setModulationBase(tDisplayValue v)
{
  m_modulationBase = v;
}

tDisplayValue ModulateableParameter::getModulationAmount() const
{
  return m_modulationAmount;
}

template <typename T> T clamp(T v, T min, T max)
{
  return std::max<T>(min, std::min<T>(v, max));
}

void ModulateableParameter::setModulationAmount(UNDO::Transaction *transaction, const tControlPositionValue &amount)
{
  auto clampedAmount = clamp<tControlPositionValue>(amount, -1.0, 1.0);

  if(differs(m_modulationAmount, clampedAmount))
  {
    auto swapData = UNDO::createSwapData(clampedAmount);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      swapData->swapWith(m_modulationAmount);
      updateModulationBase();
      getValue().resetSaturation();
      DebugLevel::gassy("mod amount set to", m_modulationAmount);
      invalidate();
      sendToAudioEngine();
    });
  }
}

MacroControls ModulateableParameter::getModulationSource() const
{
  return m_modSource;
}

void ModulateableParameter::copyFrom(UNDO::Transaction *transaction, const PresetParameter *other)
{
  if(!isLocked())
  {
    super::copyFrom(transaction, other);
    setModulationSource(transaction, other->getModulationSource());
    setModulationAmount(transaction, other->getModulationAmount());
  }
}

void ModulateableParameter::copyTo(UNDO::Transaction *transaction, PresetParameter *other) const
{
  super::copyTo(transaction, other);
  other->setField(transaction, PresetParameter::Fields::ModSource, to_string(getModulationSource()));
  other->setField(transaction, PresetParameter::Fields::ModAmount, to_string(getModulationAmount()));
}

void ModulateableParameter::setModulationSource(UNDO::Transaction *transaction, MacroControls src)
{
  if(m_modSource != src)
  {
    auto swapData = UNDO::createSwapData(src);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      if(auto groups = dynamic_cast<ParameterGroupSet *>(getParentGroup()->getParent()))
      {
        if(m_modSource != MacroControls::NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              groups->findParameterByID(ParameterFactory::modSrcToParamId(m_modSource)));
          modSrc->unregisterTarget(this);
        }

        swapData->swapWith(m_modSource);

        if(m_modSource != MacroControls::NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              groups->findParameterByID(ParameterFactory::modSrcToParamId(m_modSource)));
          modSrc->registerTarget(this);
        }

        getValue().resetSaturation();
        sendToAudioEngine();
      }
      else
      {
        swapData->swapWith(m_modSource);
        getValue().resetSaturation();
      }
      updateModulationBase();
      invalidate();
    });
  }
}

void ModulateableParameter::applyMacroControl(tDisplayValue mcValue, Initiator initiator)
{
  auto range = getValue().getScaleConverter()->getControlPositionRange().getRangeWidth();
  auto newValue = m_modulationBase + m_modulationAmount * mcValue * range;
  getValue().setRawValue(initiator, newValue);
}

void ModulateableParameter::undoableSelectModSource(UNDO::Transaction *transaction, MacroControls src)
{
  setModulationSource(transaction, src);
}

void ModulateableParameter::undoableSetModAmount(UNDO::Transaction *transaction, double amount)
{
  setModulationAmount(transaction, amount);
}

void *ModulateableParameter::getAmountCookie()
{
  return &m_modulationAmount;
}

void ModulateableParameter::undoableSetMCAmountToDefault()
{
  tDisplayValue def = 0.0;

  if(differs(m_modulationAmount, def))
  {
    ModParameterUseCases useCase(this);
    useCase.setModulationAmount(def);
  }
}

int ModulateableParameter::getModAmountDenominator(const ButtonModifiers &modifiers) const
{
  auto denom = modifiers[FINE] ? getModulationAmountFineDenominator() : getModulationAmountCoarseDenominator();
  return static_cast<int>(denom);
}

void ModulateableParameter::updateModulationBase()
{
  auto calcModulationBase = [this] {
    if(auto macroParam = getMacroControl())
    {
      auto range = getValue().getScaleConverter()->getControlPositionRange().getRangeWidth();
      auto modAmount = getModulationAmount() * range;
      auto curValue = getValue().getRawValue();
      return curValue - modAmount * macroParam->getValue().getQuantizedClipped();
    }
    return 0.0;
  };

  setModulationBase(calcModulationBase());
}

void ModulateableParameter::writeDocProperties(Writer &writer, tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);

  writer.writeTextElement("modAmount", to_string(m_modulationAmount));
  writer.writeTextElement("modBase", to_string(m_modulationBase));
  writer.writeTextElement("modSrc", to_string(static_cast<int>(m_modSource)));

  if(shouldWriteDocProperties(knownRevision))
  {
    writer.writeTextElement("mod-amount-coarse", to_string(getModulationAmountCoarseDenominator()));
    writer.writeTextElement("mod-amount-fine", to_string(getModulationAmountFineDenominator()));
    writer.writeTextElement("mod-amount-stringizer", m_modulationAmountScaleConverter->controlPositionToDisplayJS());
  }
}

void ModulateableParameter::loadDefault(UNDO::Transaction *transaction, Defaults mode)
{
  undoableSelectModSource(transaction, MacroControls::NONE);
  undoableSetModAmount(transaction, 0.0);
  super::loadDefault(transaction, mode);
}

double ModulateableParameter::getModulationAmountFineDenominator() const
{
  return ParameterDB::getFineModulationDenominator(getID());
}

double ModulateableParameter::getModulationAmountCoarseDenominator() const
{
  return ParameterDB::getCourseModulationDenominator(getID());
}

Glib::ustring ModulateableParameter::stringizeModulationAmount() const
{
  return stringizeModulationAmount(getModulationAmount());
}

Glib::ustring ModulateableParameter::stringizeModulationAmount(tControlPositionValue amount) const
{
  const auto conv = m_modulationAmountScaleConverter;
  return conv->getDimension().stringize(conv->controlPositionToDisplay(amount));
}

Layout *ModulateableParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();

    case UIMode::Edit:
      return new ModulateableParameterEditLayout2();

    case UIMode::Select:
    default:
      return new ModulateableParameterSelectLayout2();
  }

  g_return_val_if_reached(nullptr);
}

std::pair<tControlPositionValue, tControlPositionValue> ModulateableParameter::getModulationRange(bool clipped) const
{
  const auto converter = getValue().getScaleConverter();
  auto range = converter->getControlPositionRange().getRangeWidth();
  double modLeft = m_modulationBase;
  double modRight = m_modulationBase + m_modulationAmount * range;

  if(clipped)
  {
    modLeft = getValue().getFineQuantizedClippedValue(modLeft);
    modRight = getValue().getFineQuantizedClippedValue(modRight);
  }
  else
  {
    modLeft = getValue().getQuantizedValue(modLeft, true);
    modRight = getValue().getQuantizedValue(modRight, true);
  }
  return std::make_pair(modLeft, modRight);
}

std::pair<Glib::ustring, Glib::ustring> ModulateableParameter::getModRangeAsDisplayValues() const
{
  auto range = getModulationRange(false);
  auto first = modulationValueToDisplayString(range.first);
  auto second = modulationValueToDisplayString(range.second);
  return std::make_pair(first, second);
}

void ModulateableParameter::onValueChanged(Initiator initiator, tControlPositionValue oldValue,
                                           tControlPositionValue newValue)
{
  if(initiator != Initiator::MODULATION)
    updateModulationBase();

  super::onValueChanged(initiator, oldValue, newValue);
}

Glib::ustring ModulateableParameter::modulationValueToDisplayString(tControlPositionValue v) const
{
  auto scaleConverter = getValue().getScaleConverter();
  auto clipped = scaleConverter->getControlPositionRange().clip(v);
  auto displayValue = scaleConverter->controlPositionToDisplay(clipped);
  auto ret = scaleConverter->getDimension().stringize(displayValue);

  if(differs(clipped, v))
    ret = "! " + ret;

  return ret;
}

bool ModulateableParameter::isChangedFromLoaded() const
{
  return isAnyModChanged() || Parameter::isChangedFromLoaded();
}

bool ModulateableParameter::isAnyModChanged() const
{
  return isModSourceChanged() || isModAmountChanged();
}

bool ModulateableParameter::isModAmountChanged() const
{
  if(getModulationSource() == MacroControls::NONE)
    return false;

  auto original = getOriginalParameter();
  const int denominator = static_cast<const int>(getValue().getFineDenominator());
  const int roundedNow = static_cast<const int>(getModulationAmount() * denominator);
  const int roundedOG = static_cast<const int>(original->getRecallModulationAmount() * denominator);
  return roundedOG != roundedNow;
}

bool ModulateableParameter::isModSourceChanged() const
{
  auto original = getOriginalParameter();
  return original->getRecallModSource() != getModulationSource();
}

bool ModulateableParameter::isMacroControlAssignedAndChanged() const
{
  if(getModulationSource() == MacroControls::NONE)
    return false;

  if(auto myCurrMC = getMacroControl())
    return myCurrMC->isChangedFromLoaded();

  return false;
}

MacroControlParameter *ModulateableParameter::getMacroControl() const
{
  auto src = getModulationSource();
  if(src != MacroControls::NONE)
  {
    auto myMCID = ParameterFactory::modSrcToParamId(src);
    return dynamic_cast<MacroControlParameter *>(
        Application::get().getPresetManager()->getEditBuffer()->findParameterByID(myMCID));
  }
  return nullptr;
}

void ModulateableParameter::copyFrom(UNDO::Transaction *transaction, const Parameter *other)
{
  Parameter::copyFrom(transaction, other);

  if(!isLocked())
  {
    if(auto otherMod = dynamic_cast<const ModulateableParameter *>(other))
    {
      setModulationSource(transaction, otherMod->getModulationSource());
      setModulationAmount(transaction, otherMod->getModulationAmount());
    }
  }
}

bool ModulateableParameter::isDefaultLoaded() const
{
  auto valSame = Parameter::isDefaultLoaded();
  auto modSrcSame = getModulationSource() == MacroControls::NONE;
  auto modAmtSame = getModulationAmount() == 0;
  return valSame && modSrcSame && modAmtSame;
}
