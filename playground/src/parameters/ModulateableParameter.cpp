#include "ModulateableParameter.h"
#include "MacroControlParameter.h"
#include "groups/MacroControlsGroup.h"
#include "presets/EditBuffer.h"
#include "proxies/lpc/MessageComposer.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include "testing/TestDriver.h"
#include "http/UpdateDocumentMaster.h"
#include "parameters/scale-converters/Linear100PercentScaleConverter.h"
#include "scale-converters/LinearBipolar100PercentScaleConverter.h"

static TestDriver<ModulateableParameter> tests;

ModulateableParameter::ModulateableParameter(ParameterGroup *group, uint16_t id, const ScaleConverter *scaling,
                                             tDisplayValue def, tControlPositionValue coarseDenominator,
                                             tControlPositionValue fineDenominator)
    : Parameter(group, id, scaling, def, coarseDenominator, fineDenominator)
    , m_modulationAmount(0)
    , m_modSource(NONE)
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
  if(getModulationSource() == NONE)
    return 0;

  auto scaled = static_cast<gint16>(round(m_modulationAmount * getModulationAmountFineDenominator()));
  auto abs = (scaled < 0) ? -scaled : scaled;
  gint16 src = getModulationSource();

  g_assert(src > 0);
  src--;

  gint16 sign = static_cast<gint16>((scaled < 0) ? 1 : 0);
  uint16_t toSend = static_cast<uint16_t>((src << 14) | (sign << 13) | (abs));

  return toSend;
}

void ModulateableParameter::setModulationAmount(UNDO::Scope::tTransactionPtr transaction, const tDisplayValue &amount)
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

ModulateableParameter::ModulationSource ModulateableParameter::getModulationSource() const
{
  return m_modSource;
}

void ModulateableParameter::copyFrom(UNDO::Scope::tTransactionPtr transaction, Parameter *other)
{
  if(!isLocked())
  {
    super::copyFrom(transaction, other);

    if(auto p = dynamic_cast<ModulateableParameter *>(other))
    {
      setModulationSource(transaction, p->getModulationSource());
      setModulationAmount(transaction, p->getModulationAmount());
    }
  }
}

void ModulateableParameter::setModulationSource(UNDO::Scope::tTransactionPtr transaction, ModulationSource src)
{
  if(m_modSource != src)
  {
    auto swapData = UNDO::createSwapData(src);

    transaction->addSimpleCommand([=](UNDO::Command::State) mutable {
      if(EditBuffer *edit = dynamic_cast<EditBuffer *>(getParentGroup()->getParent()))
      {
        if(m_modSource != NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              edit->findParameterByID(MacroControlsGroup::modSrcToParamID(m_modSource)));
          modSrc->unregisterTarget(this);
        }

        swapData->swapWith(m_modSource);

        if(m_modSource != NONE)
        {
          auto modSrc = dynamic_cast<MacroControlParameter *>(
              edit->findParameterByID(MacroControlsGroup::modSrcToParamID(m_modSource)));
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

void ModulateableParameter::undoableSelectModSource(UNDO::Scope::tTransactionPtr transaction, int src)
{
  ModulationSource modSrc = (ModulationSource) src;
  setModulationSource(transaction, modSrc);
}

void ModulateableParameter::undoableSetModAmount(UNDO::Scope::tTransactionPtr transaction, double amount)
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

void ModulateableParameter::undoableIncrementMCSelect(UNDO::Scope::tTransactionPtr transaction, int inc)
{
  auto src = (int) getModulationSource();
  src += inc;

  while(src < 0)
    src += NUM_CHOICES;

  while(src >= NUM_CHOICES)
    src -= NUM_CHOICES;

  setModulationSource(std::move(transaction), (ModulationSource) src);
}

void ModulateableParameter::undoableIncrementMCAmount(UNDO::Scope::tTransactionPtr transaction, int inc,
                                                      ButtonModifiers modifiers)
{
  tDisplayValue controlVal = getModulationAmount();
  auto bipolarRange = ScaleConverter::getControlPositionRangeBipolar();

  if(modifiers[ButtonModifier::SHIFT]) {
    controlVal = inc < 0 ? bipolarRange.getMin() : bipolarRange.getMax();
  } else {
    double denominator = modifiers[ButtonModifier::FINE] ? 1000 : 100;
    auto rasterized = static_cast<int>(round(controlVal * denominator));
    controlVal = bipolarRange.clip((rasterized + inc) / denominator);
  }
  setModulationAmount(std::move(transaction), controlVal);
}

void ModulateableParameter::writeDocProperties(Writer &writer, tUpdateID knownRevision) const
{
  Parameter::writeDocProperties(writer, knownRevision);
  writer.writeTextElement("modAmount", to_string(m_modulationAmount));
  writer.writeTextElement("modSrc", to_string(m_modSource));
}

void ModulateableParameter::writeDifferences(Writer &writer, Parameter *other) const
{
  Parameter::writeDifferences(writer, other);
  auto *pOther = static_cast<ModulateableParameter*>(other);

  if(getModulationAmount() != pOther->getModulationAmount())
  {
    auto c = ScaleConverter::get<LinearBipolar100PercentScaleConverter>();
    auto currentParameter = c->getDimension().stringize(c->controlPositionToDisplay(getModulationAmount()));
    auto otherParameter = c->getDimension().stringize(c->controlPositionToDisplay(pOther->getModulationAmount()));

    writer.writeTextElement("mc-amount", "", Attribute("a", currentParameter), Attribute("b", otherParameter));
  }

  if(getModulationSource() != pOther->getModulationSource())
  {
    writer.writeTextElement("mc-select", "", Attribute("a", getModulationSource()),
                            Attribute("b", pOther->getModulationSource()));
  }
}

void ModulateableParameter::loadDefault(UNDO::Scope::tTransactionPtr transaction)
{
  undoableSelectModSource(transaction, 0);
  undoableSetModAmount(transaction, 0.0);
  super::loadDefault(transaction);
}

void ModulateableParameter::undoableLoadPackedModulationInfo(UNDO::Scope::tTransactionPtr transaction,
                                                             const Glib::ustring &packedModulationInfo)
{
  auto bits = stoul(packedModulationInfo);
  auto modSrc = (bits & 0xC000) >> 14;
  auto modAmount = bits & 0x1FFF;
  auto negative = bits & 0x2000;

  if(negative && modAmount == 0)
  {
    undoableSelectModSource(transaction, 0);
    undoableSetModAmount(transaction, 0.0);
  }
  else
  {
    auto iModSrc = modSrc + 1;
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

  if(m_modSource == ModulateableParameter::NONE)
    packedModulationInfo = 0x2000;

  target << packedModulationInfo << endl;
}

Glib::ustring ModulateableParameter::stringizeModulationAmount() const
{
  auto amount = getModulationAmount();
  LinearBipolar100PercentScaleConverter converter;
  return converter.getDimension().stringize(converter.controlPositionToDisplay(amount));
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
    ret = "!" + ret;

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
    peter.m_modSource = MC1;
    uint16_t packed = peter.getModulationSourceAndAmountPacked();
    packed &= 0x3FFF;
    g_assert(packed == 14);
  });
}
