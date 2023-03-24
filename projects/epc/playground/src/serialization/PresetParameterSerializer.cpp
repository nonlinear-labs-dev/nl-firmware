#include "PresetParameterSerializer.h"
#include <presets/PresetParameter.h>
#include <parameters/ParameterImportConversions.h>
#include <parameters/ModulateableParameter.h>
#include <parameters/RibbonParameter.h>
#include <parameters/PedalParameter.h>
#include <parameter_declarations.h>
#include <nltools/logging/Log.h>
#include <nltools/Assert.h>
#include <utility>
#include <parameters/scale-converters/SplitPointScaleConverter.h>

PresetParameterSerializer::PresetParameterSerializer(std::vector<PresetParameter *> param, SoundType type)
    : Serializer(getTagName())
    , m_param(std::move(param))
    , m_type(type)
{
}

Glib::ustring PresetParameterSerializer::getTagName()
{
  return "parameter";
}

void PresetParameterSerializer::writeTagContent(Writer &writer) const
{
  nltools_assertAlways(m_param.size() == 1);

  auto parameter = m_param[0];
  writer.writeTextElement("value", to_string(parameter->m_value));

  for(auto &f : parameter->m_fields)
    writer.writeTextElement(paramFieldToString(std::get<0>(f)), std::to_string(std::get<1>(f)));
}

void PresetParameterSerializer::readTagContent(Reader &reader) const
{
  reader.onTextElement("value", [&](const Glib::ustring &text, const Attributes &attr) mutable {
    auto v = std::stod(text);
    for(auto &p : m_param)
    {
      auto &conversion = ParameterImportConversions::get();
      auto converted = conversion.convert(p->m_id, v, reader.getFileVersion(), m_type);
      p->setValue(reader.getTransaction(), converted);
    }
  });

  reader.onTextElement("modAmount", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    auto v = std::stod(text);
    for(auto &p : m_param)
    {
      v = ParameterImportConversions::get().convertMCAmount(p->m_id, v, reader.getFileVersion());
      p->setField(reader.getTransaction(), PresetParameter::Fields::ModAmount, to_string(v));
    }
  });

  reader.onTextElement("modSrc", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::ModSource, text);
    }
  });

  reader.onTextElement("givenName", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::GivenName, text);
    }
  });

  reader.onTextElement("info", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::Info, text);
    }
  });

  reader.onTextElement("ribbon-touch-behaviour", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::RibbonTouchBehaviour, text);
    }
  });

  reader.onTextElement("ribbon-return-mode", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::RibbonReturnMode, text);
    }
  });

  reader.onTextElement("pedalMode", [=, &reader](const Glib::ustring &text, const Attributes &attr) {
    for(auto &p : m_param)
    {
      p->setField(reader.getTransaction(), PresetParameter::Fields::PedalMode, text);
    }
  });
}
