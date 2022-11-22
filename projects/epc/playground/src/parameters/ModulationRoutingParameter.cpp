#include "ModulationRoutingParameter.h"
#include "PhysicalControlParameter.h"
#include "RibbonParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>
#include <groups/MacroControlMappingGroup.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulationRouterParameterLayouts.h>
#include <cmath>
#include <Application.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/audio-engine/AudioEngineProxy.h>

ModulationRoutingParameter::ModulationRoutingParameter(ParameterGroup *group, const ParameterId &id,
                                                       tSrcParameterPtr srcParam, tMCParameterPtr tgtParam)
    : super(group, id)
    , m_tgtParameter(tgtParam)
    , m_srcParameter(srcParam)
{
  srcParam->registerTarget(this);
}

ModulationRoutingParameter::~ModulationRoutingParameter() = default;

void ModulationRoutingParameter::setCpValue(UNDO::Transaction *transaction, Initiator initiator,
                                            tControlPositionValue value, bool dosendToPlaycontroller)
{
  const auto oldValue = getControlPositionValue();
  Parameter::setCpValue(transaction, initiator, value, dosendToPlaycontroller);
  const auto newValue = getControlPositionValue();

  if(oldValue == 0.0 && newValue != 0)
  {
    if(auto p = dynamic_cast<RibbonParameter *>(getSourceParameter()))
    {
      if(p->getRibbonReturnMode() == RibbonReturnMode::STAY)
      {
        auto *parent = dynamic_cast<MacroControlMappingGroup *>(getParentGroup());
        auto otherRouters = parent->getModulationRoutingParametersFor(getSourceParameter());

        for(auto router : otherRouters)
        {
          if(router != this)
          {
            router->onExclusiveRoutingLost(transaction);
          }
        }

        p->boundToMacroControl(getTargetParameter()->getControlPositionValue());
      }
    }
  }
}

void ModulationRoutingParameter::onExclusiveRoutingLost(UNDO::Transaction *transaction)
{
  setIndirect(transaction, 0);

  transaction->addSimpleCommand(
      [=](auto s)
      {
        onChange();
        invalidate();
        sendToAudioEngine();
      });
}

void ModulationRoutingParameter::applyPlaycontrollerPhysicalControl(tControlPositionValue diff)
{
  auto amount = getControlPositionValue();

  if(std::abs(amount) > std::numeric_limits<tDisplayValue>::epsilon())
  {
    tControlPositionValue result = diff * amount;

    if(std::abs(result) > std::numeric_limits<tDisplayValue>::epsilon())
      m_tgtParameter->applyPlaycontrollerPhysicalControl(result);
  }
}

void ModulationRoutingParameter::applyAbsolutePlaycontrollerPhysicalControl(tControlPositionValue v)
{
  auto amount = getControlPositionValue();

  if(std::abs(amount) > std::numeric_limits<tDisplayValue>::epsilon())
  {
    m_tgtParameter->applyAbsolutePlaycontrollerPhysicalControl(v);
  }
}

ModulationRoutingParameter::tMCParameterPtr ModulationRoutingParameter::getTargetParameter() const
{
  return m_tgtParameter;
}

ModulationRoutingParameter::tSrcParameterPtr ModulationRoutingParameter::getSourceParameter() const
{
  return m_srcParameter;
}

bool ModulationRoutingParameter::routes(const PhysicalControlParameter *p) const
{
  return m_srcParameter == p;
}

Glib::ustring ModulationRoutingParameter::getDisplayString() const
{
  if(getSourceParameter()->getReturnMode() == ReturnMode::None)
  {
    if(getValue().getDisplayValue() != 0.0)
    {
      return "On";
    }
    else
    {
      return "Off";
    }
  }

  return super::getDisplayString();
}

tControlPositionValue ModulationRoutingParameter::getControlPositionValue() const
{
  if(getSourceParameter()->getReturnMode() == ReturnMode::None)
  {
    if(getValue().getDisplayValue() != 0.0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  return super::getControlPositionValue();
}

Layout *ModulationRoutingParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();

    case UIMode::Edit:
      return new ModulationRouterParameterEditLayout2();

    case UIMode::Select:
    default:
      return new ModulationRouterParameterSelectLayout2();
  }

  g_return_val_if_reached(nullptr);
}

void ModulationRoutingParameter::undoableRandomize(UNDO::Transaction *transaction, Initiator initiator, double amount)
{
}