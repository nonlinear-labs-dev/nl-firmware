#include <Application.h>
#include <playground.h>
#include <boost/tokenizer.hpp>
#include <parameters/names/ParameterDB.h>
#include <tools/StringTools.h>
#include <parameters/Parameter.h>
#include <parameter_list.h>
#include <placeholder.h>
#include <cassert>
#include <nltools/logging/Log.h>
#include <groups/MacroControlsGroup.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/MacroControlParameter.h>

namespace C15::Placeholder
{
  static Glib::ustring replaceGlobal(const Glib::ustring &_text, const SoundType &_st, const bool &_multiple = false)
  {
    Glib::ustring ret{ _text };
    std::size_t pos;
    while((pos = ret.find(Qualifier)) != std::string::npos)
    {
      for(const GlobalReplacer &placeholder : GlobalPlaceholders)
      {
        if((pos = ret.find(placeholder.m_qualifier)) == Glib::ustring::npos)
          continue;
        ret.replace(pos, strlen(placeholder.m_qualifier), placeholder.getReplacement(_st));
        if(!_multiple)
          return ret;
      }
    }
    return ret;
  }

  static Glib::ustring replaceLocal(const Glib::ustring &_text, const SoundType &_st, const VoiceGroup &_vg,
                                    const bool &_multiple = false)
  {
    Glib::ustring ret{ _text };
    std::size_t pos;
    while((pos = ret.find(Qualifier)) != std::string::npos)
    {
      for(const LocalReplacer &placeholder : LocalPlaceholders)
      {
        if((pos = ret.find(placeholder.m_qualifier)) == Glib::ustring::npos)
          continue;
        ret.replace(pos, strlen(placeholder.m_qualifier), placeholder.getReplacement(_st, _vg));
        if(!_multiple)
          return ret;
      }
    }
    return ret;
  }
}  // namespace C15

ParameterDB::ParameterDB(EditBuffer &eb)
    : m_editBuffer(eb)
{
}

ParameterDB::~ParameterDB() = default;

Glib::ustring ParameterDB::getLongName(const ParameterId &id) const
{
  auto num = id.getNumber();

  assert(num >= 0);
  assert(num < C15::Config::tcd_elements);

  auto d = C15::ParameterList[num];
  if(!d.m_pg.m_param_label_long)
  {
    nltools::Log::error("there is no long name entry in parameter list for parameter", num);
    return "MISSING!!!";
  }

  return replaceInDynamicLabels(d.m_pg.m_param_label_long, id, m_editBuffer.getType());
}

Glib::ustring ParameterDB::getShortName(const ParameterId &id) const
{
  auto num = id.getNumber();

  assert(num >= 0);
  assert(num < C15::Config::tcd_elements);

  auto d = C15::ParameterList[num];
  if(!d.m_pg.m_param_label_short)
  {
    nltools::Log::error("there is no short name entry in parameter list for parameter", num);
    return "MISSING!!!";
  }

  return replaceInDynamicLabels(d.m_pg.m_param_label_short, id, m_editBuffer.getType());
}

Glib::ustring ParameterDB::getDescription(const ParameterId &id) const
{
  auto num = id.getNumber();
  return getDescription(num);
}

std::optional<Glib::ustring> ParameterDB::getLongGroupName(const ParameterId &id) const
{
  auto d = C15::ParameterList[id.getNumber()];

  if(!d.m_pg.m_group_label_long)
    return {};

  return replaceInDynamicLabels(d.m_pg.m_group_label_long, id, m_editBuffer.getType());
}

Glib::ustring ParameterDB::getDescription(const int num) const
{
  assert(num >= 0);
  assert(num < C15::Config::tcd_elements);

  if(MacroControlsGroup::isMacroControl(num))
  {
    auto param = Application::get().getPresetManager()->getEditBuffer()->findParameterByID({ num, VoiceGroup::Global });
    if(auto mcParam = dynamic_cast<const MacroControlParameter *>(param))
    {
      return mcParam->getInfo();
    }
  }

  auto d = C15::ParameterList[num];
  if(!d.m_pg.m_param_info)
  {
    nltools::Log::error("there is no short name entry in parameter list for parameter", num);
    return "";
  }

  //TODO add replacement of infotexts
  return d.m_pg.m_param_info;
}

tControlPositionValue ParameterDB::getSignalPathIndication(int id) const
{
  assert(id >= 0);
  assert(id < C15::Config::tcd_elements);

  auto d = C15::ParameterList[id];
  return (d.m_pg.m_inactive_cp && strlen(d.m_pg.m_inactive_cp) > 0) ? std::stod(d.m_pg.m_inactive_cp)
                                                                    : getInvalidSignalPathIndication();
}

Glib::ustring ParameterDB::replaceInDynamicLabels(const Glib::ustring &name, const ParameterId &parameterID,
                                                  SoundType type) const
{
  auto paramType = C15::ParameterList[parameterID.getNumber()].m_param.m_type;
  using namespace C15::Descriptors;
  auto isGlobal = paramType != ParameterType::Local_Modulateable && paramType != ParameterType::Local_Unmodulateable;

  if(isGlobal)
    return C15::Placeholder::replaceGlobal(name, type);
  else
    return C15::Placeholder::replaceLocal(name, type, parameterID.getVoiceGroup());
}
