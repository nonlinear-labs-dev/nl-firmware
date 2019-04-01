#include "ModulateableParameter.h"
#include "MacroControlParameter.h"
#include "groups/MacroControlsGroup.h"
#include "proxies/lpc/MessageComposer.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include "testing/TestDriver.h"
#include "http/UpdateDocumentMaster.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "scale-converters/LinearBipolar100PercentScaleConverter.h"
#include <libundo/undo/Transaction.h>
#include <device-settings/DebugLevel.h>
#include <xml/Writer.h>
#include <presets/ParameterGroupSet.h>
#include <presets/PresetParameter.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <presets/Preset.h>

static TestDriver<ModulateableParameter> tests;

ModulateableParameter::ModulateableParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                             tDisplayValue def, tControlPositionValue coarseDenominator,
                                             tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
    , m_modulationAmount(0)
    , m_modSource(ModulationSource::NONE)
{
}

ModulateableParameter::~ModulateableParameter()
{
}

size_t ModulateableParameter::getHash() const
{
  size_t hash = super::getHash();
  hash_combine(hash, m_modulationAmount);
  hash_combine(hash, (int) m_modSource);
  return hash;
}

tDisplayValue ModulateableParameter::getModulationAmount() const
{
  return m_modulationAmount;
}

void ModulateableParameter::writeToLPC(MessageComposer &cmp) const
{
  Parameter::writeToLPC(cmp);
  cmp << getModulationSourceAndAmountPacked();
}

uint16_t ModulateableParameter::getModulationSourceAndAmountPacked() const
{
  if(getModulationSource() == ModulationSource::NONE)
    return 0;

  auto scaled = static_cast<gint16>(round(m_modulationAmount * getModulationAmountFineDenominator()));
  auto abs = (scaled < 0) ? -scaled : scaled;
  gint16 src = static_cast<gint16>(getModulationSource());

  g_assert(src > 0);
  src--;

  gint16 sign = static_cast<gint16>((scaled < 0) ? 1 : 0);
  uint16_t toSend = static_cast<uint16_t>((src << 14) | (sign << 13) | (abs));

  return toSend;
}

void ModulateableParameter::setModulationAmount(UNDO::Transaction *transaction, const tDisplayValue &amount)
{
  auto clampedAmount = CLAMP(amount, -1.0, 1.0);

  if(m_modulationAmount != clampedAmount)
  {
    auto swapData = UNDO::createSwapData(clampedAmount);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      swapData->swapWith(m_modulationAmount);
      getValue().resetSaturation();
      DebugLevel::gassy("mod amount set to", m_modulationAmount);
      invalidate();
      sendToLpc();
    });
  }
}

ModulationSource ModulateableParameter::getModulationSource() const
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

void ModulateableParameter::setModulationSource(UNDO::Transaction *transaction, ModulationSource src)
{
  if(m_modSource != src)
  {
    auto swapData = UNDO::createSwapData(src);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      if(auto groups = static_cast<ParameterGroupSet *>(getParentGroup()->getParent()))
      {
        if(m_modSource != ModulationSource::NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              groups->findParameterByID(MacroControlsGroup::modSrcToParamID(m_modSource)));
          modSrc->unregisterTarget(this);
        }

        swapData->swapWith(m_modSource);

        if(m_modSource != ModulationSource::NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              groups->findParameterByID(MacroControlsGroup::modSrcToParamID(m_modSource)));
          modSrc->registerTarget(this);
        }

        getValue().resetSaturation();
        sendToLpc();
      }
      else
      {
        swapData->swapWith(m_modSource);
        getValue().resetSaturation();
      }

      invalidate();
    });
  }
}

void ModulateableParameter::applyLpcMacroControl(tDisplayValue diff)
{
  if(isBiPolar())
    diff *= 2;

  getValue().changeRawValue(Initiator::EXPLICIT_LPC, diff * m_modulationAmount);
}

void ModulateableParameter::undoableSelectModSource(UNDO::Transaction *transaction, ModulationSource src)
{
  setModulationSource(transaction, src);
}

void ModulateableParameter::undoableSetModAmount(UNDO::Transaction *transaction, double amount)
{
  setModulationAmount(transaction, amount);
}

void ModulateableParameter::undoableIncrementMCSelect(int inc)
{
  auto scope = getUndoScope().startTransaction("Set MC Select for '%0'", getLongName());
  undoableIncrementMCSelect(scope->getTransaction(), inc);
}

void ModulateableParameter::undoableIncrementMCAmount(int inc)
{
  auto scope = getUndoScope().startContinuousTransaction(getAmountCookie(), "Set MC Amount for '%0'",
                                                         getGroupAndParameterName());
  undoableIncrementMCAmount(scope->getTransaction(), inc, ButtonModifiers());
}

void *ModulateableParameter::getAmountCookie()
{
  return &m_modulationAmount;
}

void ModulateableParameter::undoableSetMCAmountToDefault()
{
  tDisplayValue def = 0.0;

  if(m_modulationAmount != def)
  {
    auto scope = getUndoScope().startContinuousTransaction(getAmountCookie(), "Set MC Amount for '%0'",
                                                           getGroupAndParameterName());
    setModulationAmount(scope->getTransaction(), def);
  }
}

void ModulateableParameter::undoableIncrementMCSelect(UNDO::Transaction *transaction, int inc)
{
  auto src = (int) getModulationSource();
  auto numChoices = static_cast<int>(ModulationSource::NUM_CHOICES);
  src += inc;

  while(src < 0)
    src += numChoices;

  while(src >= numChoices)
    src -= numChoices;

  setModulationSource(std::move(transaction), (ModulationSource) src);
}

void ModulateableParameter::undoableIncrementMCAmount(UNDO::Transaction *transaction, int inc,
                                                      ButtonModifiers modifiers)
{
  tDisplayValue controlVal = getModulationAmount();
  double denominator = getModAmountDenominator(modifiers);
  int rasterized = round(controlVal * denominator);
  controlVal = ScaleConverter::getControlPositionRangeBipolar().clip((rasterized + inc) / denominator);
  setModulationAmount(transaction, controlVal);
}

int ModulateableParameter::getModAmountDenominator(const ButtonModifiers &modifiers) const
{
  return modifiers[FINE] ? getModulationAmountFineDenominator() : getModulationAmountCoarseDenominator();
}

void ModulateableParameter::writeDocProperties(Writer &writer, tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);

  writer.writeTextElement("modAmount", to_string(m_modulationAmount));
  writer.writeTextElement("modSrc", to_string(static_cast<int>(m_modSource)));
  writer.writeTextElement("og-modAmount", to_string(getOriginalModulationAmount()));
  writer.writeTextElement("og-modSrc", to_string(static_cast<int>(getOriginalModulationSource())));

  if(shouldWriteDocProperties(knownRevision))
  {
    writer.writeTextElement("mod-amount-coarse", to_string(getModulationAmountCoarseDenominator()));
    writer.writeTextElement("mod-amount-fine", to_string(getModulationAmountFineDenominator()));
  }
}

void ModulateableParameter::loadDefault(UNDO::Transaction *transaction)
{
  undoableSelectModSource(transaction, ModulationSource::NONE);
  undoableSetModAmount(transaction, 0.0);
  super::loadDefault(transaction);
}

void ModulateableParameter::undoableLoadPackedModulationInfo(UNDO::Transaction *transaction,
                                                             const Glib::ustring &packedModulationInfo)
{
  auto bits = stoul(packedModulationInfo);
  auto modSrc = (bits & 0xC000) >> 14;
  auto modAmount = bits & 0x1FFF;
  auto negative = bits & 0x2000;

  if(negative && modAmount == 0)
  {
    undoableSelectModSource(transaction, ModulationSource::NONE);
    undoableSetModAmount(transaction, 0.0);
  }
  else
  {
    auto iModSrc = static_cast<ModulationSource>(modSrc + 1);
    undoableSelectModSource(transaction, iModSrc);

    auto fModAmount = (negative ? -1.0 : 1.0) * modAmount / getModulationAmountFineDenominator();
    undoableSetModAmount(transaction, fModAmount);
  }
}

double ModulateableParameter::getModulationAmountFineDenominator() const
{
  auto fineDenominator = getValue().getFineDenominator();

  while(fineDenominator > 8000)
    fineDenominator /= 2;

  return fineDenominator;
}

double ModulateableParameter::getModulationAmountCoarseDenominator() const
{
  return getValue().getCoarseDenominator();
}

void ModulateableParameter::exportReaktorParameter(stringstream &target) const
{
  super::exportReaktorParameter(target);
  auto packedModulationInfo = getModulationSourceAndAmountPacked();

  if(m_modSource == ModulationSource::NONE)
    packedModulationInfo = 0x2000;

  target << packedModulationInfo << endl;
}

Glib::ustring ModulateableParameter::stringizeModulationAmount() const
{
  return stringizeModulationAmount(getModulationAmount());
}

Glib::ustring ModulateableParameter::stringizeModulationAmount(tControlPositionValue amount) const
{
  auto converter = getValue().getScaleConverter();
  if(!converter->isBiPolar())
  {
    DebugLevel::warning("Using LinearBipolar100PercentScaleConverter For Parameter:", getLongName(), "because ValueScaleConverter is not bipolar");
    converter = ScaleConverter::get<LinearBipolar100PercentScaleConverter>();
  }

  return converter->getDimension().stringize(converter->controlPositionToDisplay(amount));
}

DFBLayout *ModulateableParameter::createLayout(FocusAndMode focusAndMode) const
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
  double modLeft = 0;
  double modRight = 0;

  auto src = getModulationSource();
  uint16_t srcParamID = MacroControlsGroup::modSrcToParamID(src);
  auto groupSet = dynamic_cast<const ParameterGroupSet *>(getParentGroup()->getParent());

  if(auto srcParam = groupSet->findParameterByID(srcParamID))
  {
    auto modAmount = getModulationAmount();
    auto srcValue = srcParam->getValue().getRawValue();
    auto value = getValue().getRawValue();

    if(isBiPolar())
      modLeft = 0.5 * (value + 1.0) - modAmount * srcValue;
    else
      modLeft = value - modAmount * srcValue;

    modRight = modLeft + modAmount;
  }

  if(clipped)
  {
    modLeft = getValue().getScaleConverter()->getControlPositionRange().clip(modLeft);
    modRight = getValue().getScaleConverter()->getControlPositionRange().clip(modRight);
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

Glib::ustring ModulateableParameter::modulationValueToDisplayString(tControlPositionValue v) const
{
  if(isBiPolar())
  {
    v = v * 2 - 1;
  }

  auto scaleConverter = getValue().getScaleConverter();
  auto clipped = scaleConverter->getControlPositionRange().clip(v);
  auto displayValue = scaleConverter->controlPositionToDisplay(clipped);
  auto ret = scaleConverter->getDimension().stringize(displayValue);

  if(clipped != v)
    ret = "! " + ret;

  return ret;
}

void ModulateableParameter::registerTests()
{
  g_test_add_func("/ModulateableParameter/1.4pct-to-112lpc", []() {
    class Root : public UpdateDocumentMaster
    {
     public:
      tUpdateID onChange(bool force)
      {
        return 0;
      }

      virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const
      {
      }
    };

    Root root;

    class GroupSet : public ParameterGroupSet
    {
     public:
      GroupSet(Root *root)
          : ParameterGroupSet(root)
      {
      }

      virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const
      {
      }
    };

    GroupSet groupSet(&root);

    class Group : public ParameterGroup
    {
     public:
      Group(GroupSet *root)
          : ParameterGroup(root, "a", "b", "b", "b")
      {
      }

      void init()
      {
      }

      virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const
      {
      }
    };

    Group group(&groupSet);

    ModulateableParameter peter(&group, 1, ScaleConverter::get<Linear100PercentScaleConverter>(), 0, 100, 1000);
    peter.m_modulationAmount = 0.014;
    peter.m_modSource = ModulationSource::MC1;
    uint16_t packed = peter.getModulationSourceAndAmountPacked();
    packed &= 0x3FFF;
    g_assert(packed == 14);
  });
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
  if(getModulationSource() == ModulationSource::NONE)
    return false;

  if(auto original = getOriginalParameter())
  {
    return original->getModulationAmount() != getModulationAmount();
  }
  return false;
}

bool ModulateableParameter::isModSourceChanged() const
{
  if(auto original = getOriginalParameter())
  {
    return original->getModulationSource() != getModulationSource();
  }
  return false;
}

bool ModulateableParameter::isMacroControlAssignedAndChanged() const
{
  if(getModulationSource() == ModulationSource::NONE)
    return false;

  if(auto myCurrMC = getMacroControl())
    return myCurrMC->isChangedFromLoaded();

  return false;
}

PresetParameter *ModulateableParameter::getOriginalMC() const
{
  if(auto original = getOriginalParameter())
  {
    if(original->getModulationSource() == ModulationSource::NONE)
      return nullptr;

    auto originalMCID = MacroControlsGroup::modSrcToParamID(original->getModulationSource());
    if(auto origin = Application::get().getPresetManager()->getEditBuffer()->getOrigin())
    {
      return origin->findParameterByID(originalMCID);
    }
  }
  return nullptr;
}

Parameter *ModulateableParameter::getMacroControl() const
{
  auto myMCID = MacroControlsGroup::modSrcToParamID(getModulationSource());
  return Application::get().getPresetManager()->getEditBuffer()->findParameterByID(myMCID);
}

tControlPositionValue ModulateableParameter::getOriginalModulationAmount() const
{
  if(auto original = getOriginalParameter())
  {
    return original->getModulationAmount();
  }
  return 0.0;
}
ModulationSource ModulateableParameter::getOriginalModulationSource() const
{
  if(auto original = getOriginalParameter())
  {
    return original->getModulationSource();
  }
  return ModulationSource::NONE;
}

void ModulateableParameter::undoableRecallMCPos()
{

  if(auto mc = getMacroControl())
  {
    mc->undoableRecallFromPreset();
    onChange(ChangeFlags::Generic);
  }
}
void ModulateableParameter::undoableRecallMCSource()
{
  if(!isModSourceChanged())
    return;
  auto &scope = Application::get().getPresetManager()->getUndoScope();
  auto original = getOriginalParameter();
  auto transactionScope = scope.startTransaction("Recall MC Pos");
  auto transaction = transactionScope->getTransaction();
  if(original)
  {
    setModulationSource(transaction, original->getModulationSource());
  }
  onChange(ChangeFlags::Generic);
}
void ModulateableParameter::undoableRecallMCAmount()
{
  if(!isModAmountChanged())
    return;
  auto &scope = Application::get().getPresetManager()->getUndoScope();
  auto original = getOriginalParameter();
  auto transactionScope = scope.startTransaction("Recall MC Pos");
  auto transaction = transactionScope->getTransaction();
  if(original)
  {
    setModulationAmount(transaction, original->getModulationAmount());
  }
  onChange(ChangeFlags::Generic);
}