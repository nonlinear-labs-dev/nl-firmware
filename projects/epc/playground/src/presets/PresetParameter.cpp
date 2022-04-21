#include "PresetParameter.h"
#include "PresetParameterGroup.h"
#include <libundo/undo/Transaction.h>
#include <nltools/Assert.h>
#include <parameters/Parameter.h>
#include <xml/Writer.h>
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/scale-converters/LinearBipolar100PercentScaleConverter.h>
#include <groups/ParameterGroup.h>
#include <libundo/undo/Scope.h>
#include <xml/Attribute.h>
#include <parameter_declarations.h>
#include <parameters/SplitPointParameter.h>
#include <parameters/scale-converters/LinearCountScaleConverter.h>
#include <parameters/scale-converters/dimension/VoicesDimension.h>

PresetParameter::PresetParameter(const ParameterId &id)
    : m_id { id }
{
}

PresetParameter::PresetParameter(const ::Parameter &other)
    : PresetParameter(other.getID())
{
  auto trash = UNDO::Scope::startTrashTransaction();
  other.copyTo(trash->getTransaction(), this);
}

PresetParameter::PresetParameter(const PresetParameter &other)
    : PresetParameter(other.m_id)
{
  m_value = other.m_value;
  m_fields = other.m_fields;
}

tControlPositionValue PresetParameter::getValue() const
{
  return m_value;
}

void PresetParameter::setValue(UNDO::Transaction *transaction, tControlPositionValue v)
{
  v = ScaleConverter::getControlPositionRangeBipolar().clip(v);
  transaction->addUndoSwap(m_value, v);
}

void PresetParameter::setField(UNDO::Transaction *transaction, Fields field, const std::string &value)
{
  auto cp = m_fields;
  bool found = false;

  for(auto &f : cp)
  {
    if(std::get<0>(f) == field)
    {
      std::get<1>(f) = value;
      found = true;
      break;
    }
  }

  if(!found)
    cp.push_back({ field, value });

  transaction->addUndoSwap(m_fields, cp);
}

void PresetParameter::copyFrom(UNDO::Transaction *transaction, const std::unique_ptr<PresetParameter> &other)
{
  copyFrom(transaction, other.get());
}

void PresetParameter::copyFrom(UNDO::Transaction *transaction, const PresetParameter *other)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  nltools_assertAlways(m_id.getNumber() == other->m_id.getNumber());
  transaction->addUndoSwap(eb, m_value, other->m_value);
  transaction->addUndoSwap(eb, m_fields, other->m_fields);
}

void PresetParameter::copyFrom(UNDO::Transaction *transaction, const ::Parameter *other)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  nltools_assertAlways(m_id.getNumber() == other->getID().getNumber());
  transaction->addUndoSwap(eb, m_fields, {});
  other->copyTo(transaction, this);
}

void PresetParameter::assignVoiceGroup(VoiceGroup vg)
{
  m_id = { m_id.getNumber(), vg };
}

void PresetParameter::writeDiff(Writer &writer, ParameterId parameterID, const PresetParameter *other) const
{
  auto ebParam = Application::get().getPresetManager()->getEditBuffer()->findParameterByID(parameterID);

  bool aFound = ebParam != nullptr;
  bool bFound = other != nullptr;
  auto name = aFound ? std::string(ebParam->getLongName()) : getID().toString();

  writer.writeTag(
      "parameter", Attribute("name", name), Attribute("afound", aFound ? "true" : "false"),
      Attribute("bfound", bFound ? "true" : "false"), Attribute("id-num", getID().getNumber()), [&] {
        if(aFound && bFound)
        {
          auto sc = ebParam->getValue().getScaleConverter();
          auto myString = sc->getDimension().stringize(sc->controlPositionToDisplay(m_value));
          auto otherString = sc->getDimension().stringize(sc->controlPositionToDisplay(other->m_value));

          if(ebParam->getID().getNumber() == C15::PID::Unison_Voices)
          {
            //we always use the single converter as unison voices get saved in 24 voices format
            auto singleConverter = ScaleConverter::get<LinearCountScaleConverter<24, VoicesDimension>>();

            auto stringize
                = [](auto sc, auto v) { return sc->getDimension().stringize(sc->controlPositionToDisplay(v)); };

            myString = stringize(singleConverter, m_value);
            otherString = stringize(singleConverter, other->m_value);
          }

          if(myString != otherString)
            writer.writeTextElement("value", "", Attribute("a", myString), Attribute("b", otherString));

          if(getModulationSource() != other->getModulationSource())
          {
            writer.writeTextElement("mc-select", "", Attribute("a", getModulationSource()),
                                    Attribute("b", other->getModulationSource()));
          }

          if(differs(getModulationAmount(), other->getModulationAmount()))
          {
            auto c = ScaleConverter::get<LinearBipolar100PercentScaleConverter>();
            auto currentParameter = c->getDimension().stringize(c->controlPositionToDisplay(getModulationAmount()));
            auto otherParameter
                = c->getDimension().stringize(c->controlPositionToDisplay(other->getModulationAmount()));
            writer.writeTextElement("mc-amount", "", Attribute("a", currentParameter), Attribute("b", otherParameter));
          }

          if(getGivenName() != other->getGivenName())
          {
            writer.writeTextElement("name", "", Attribute("a", getGivenName()), Attribute("b", other->getGivenName()));
          }

          if(getInfo() != other->getInfo())
          {
            writer.writeTextElement("info", "", Attribute("a", "changed"), Attribute("b", "changed"));
          }

          if(getRibbonReturnMode() != other->getRibbonReturnMode())
          {
            writer.writeTextElement("return-mode", "", Attribute("a", static_cast<int>(getRibbonReturnMode())),
                                    Attribute("b", static_cast<int>(other->getRibbonReturnMode())));
          }

          if(getRibbonTouchBehaviour() != other->getRibbonTouchBehaviour())
          {
            writer.writeTextElement("behaviour", "", Attribute("a", getRibbonTouchBehaviour()),
                                    Attribute("b", other->getRibbonTouchBehaviour()));
          }

          if(getPedalMode() != other->getPedalMode())
          {
            writer.writeTextElement("return-mode", "", Attribute("a", static_cast<int>(getPedalMode())),
                                    Attribute("b", static_cast<int>(other->getPedalMode())));
          }
        }
      });
}

MacroControls PresetParameter::getModulationSource() const
{
  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::ModSource && !std::get<1>(f).empty())
      return static_cast<MacroControls>(std::stoi(std::get<1>(f)));

  return MacroControls::NONE;
}

double PresetParameter::getModulationAmount() const
{
  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::ModAmount && !std::get<1>(f).empty())
      return std::stod(std::get<1>(f));

  return 0.0;
}

std::string PresetParameter::getGivenName() const
{
  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::GivenName)
      return std::get<1>(f);

  return "";
}

std::string PresetParameter::getInfo() const
{
  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::Info)
      return std::get<1>(f);

  return "";
}

const ParameterId PresetParameter::getID() const
{
  return m_id;
}

enum RibbonReturnMode PresetParameter::getRibbonReturnMode() const
{
  using RRM = enum RibbonReturnMode;

  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::RibbonReturnMode && !std::get<1>(f).empty())
      return static_cast<RRM>(std::stoi(std::get<1>(f)));

  return RRM::RETURN;
}

enum RibbonTouchBehaviour PresetParameter::getRibbonTouchBehaviour() const
{
  using RTB = enum RibbonTouchBehaviour;

  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::RibbonTouchBehaviour && !std::get<1>(f).empty())
      return static_cast<RTB>(std::stoi(std::get<1>(f)));

  return RTB::ABSOLUTE;
}

enum PedalModes PresetParameter::getPedalMode() const
{
  for(const auto &f : m_fields)
    if(std::get<0>(f) == Fields::PedalMode && !std::get<1>(f).empty())
      return static_cast<enum PedalModes>(std::stoi(std::get<1>(f)));

  return PedalModes::STAY;
}

void PresetParameter::writeDocument(Writer &writer) const
{
  writer.writeTag("param",
                  { Attribute { "id", m_id.toString() }, Attribute { "value", to_string(m_value) },
                    Attribute { "mod-src", to_string(static_cast<int>(getModulationSource())) },
                    Attribute { "mod-amt", to_string(getModulationAmount()) } },
                  []() {});
}

std::string paramFieldToString(PresetParameter::Fields f)
{
  switch(f)
  {
    case PresetParameter::Fields::ModAmount:
      return "modAmount";

    case PresetParameter::Fields::ModSource:
      return "modSrc";

    case PresetParameter::Fields::GivenName:
      return "givenName";

    case PresetParameter::Fields::Info:
      return "info";

    case PresetParameter::Fields::RibbonTouchBehaviour:
      return "ribbon-touch-behaviour";

    case PresetParameter::Fields::RibbonReturnMode:
      return "ribbon-return-mode";

    case PresetParameter::Fields::PedalMode:
      return "pedalMode";
  }
  return "";
}
