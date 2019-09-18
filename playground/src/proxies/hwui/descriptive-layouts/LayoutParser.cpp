#include <fstream>
#include <Application.h>
#include <Options.h>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"
#include "ConditionRegistry.h"
#include "JsonHelpers.h"
#include <boost/algorithm/string.hpp>
#include <tools/StringTools.h>
#include <proxies/hwui/descriptive-layouts/Primitives/Text.h>
#include <boost/lexical_cast.hpp>
#include <proxies/hwui/descriptive-layouts/Primitives/Bar.h>
#include <nltools/StringTools.h>
#include <tools/ExceptionTools.h>

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  using tConditionList = std::list<ConditionBase*>;

  std::list<Selector> toSelectors(const json& selector)
  {
    std::list<Selector> selectors;

    if(!JSONTools::readFieldsFromJson<UIFocus>(selector, "UIFocus", toUIFocus, selectors))
      selectors.emplace_back(UIFocus::Any);

    if(!JSONTools::readFieldsFromJson<UIMode>(selector, "UIMode", toUIMode, selectors))
      selectors.emplace_back(UIMode::Any);

    if(!JSONTools::readFieldsFromJson<UIDetail>(selector, "UIDetail", toUIDetail, selectors))
      selectors.emplace_back(UIDetail::Any);

    return selectors;
  }

  Point toPoint(const json& pt)
  {
    if(pt.is_string())
    {
      std::string compact = pt;
      if(nltools::startsWith(compact, "BUTTON_"))
      {
 #warning"adlerauge -> take points from Button.h"
        if(compact == "BUTTON_A")
        {
          return Point(3, 51);
        }
        else if(compact == "BUTTON_B")
        {
          return Point(67, 51);
        }
        else if(compact == "BUTTON_C")
        {
          return Point(131, 51);
        }
        else if(compact == "BUTTON_D")
        {
          return Point(195, 51);
        }
        else
        {
          return Point(0, 0);
        }
      }
      else
      {
        std::vector<std::string> splits;
        boost::split(splits, compact, boost::is_any_of(","));
        return Point(std::stoi(splits[0]), std::stoi(splits[1]));
      }
    }
    else
    {
      auto x = pt.at("X");
      auto y = pt.at("Y");
      return Point(x, y);
    }
  }

  std::string removeSpaces(std::string str)
  {
    str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
  }

  std::string removeLastCharacter(std::string str)
  {
    str.pop_back();
    return str;
  }

  std::experimental::any getAnyFromProperty(const json& value, const PrimitiveProperty& eventTargetProperty)
  {
    std::experimental::any ret{};
    switch(eventTargetProperty)
    {
      case PrimitiveProperty::ControlPosition:
        return value.template get<double>();
      case PrimitiveProperty::Range:
      {
        auto values = StringTools::splitStringOnStringDelimiter(value.template get<std::string>(), ",");
        return Bar::Range(std::stof(values[0]), std::stof(values[1]));
      }
      case PrimitiveProperty::Text:
        return Text::DisplayString(value.template get<std::string>(), 0);
      case PrimitiveProperty::Highlight:
      case PrimitiveProperty::Visibility:
      {
        bool val{};
        std::istringstream(value.template get<std::string>()) >> std::boolalpha >> val;
        return val;
      }
      default:
      case PrimitiveProperty::None:
        break;
    }
    return ret;
  }

  ControlInstance::StaticInitList parseInit(const json& j)
  {
    ControlInstance::StaticInitList ret{};

    JSONTools::forEachJsonInTag(j, "Init", [&](const auto& init) {
      JSONTools::forEachJsonChild(init, [&](const auto& obj) {
        auto key = obj.key();
        auto value = obj.value();

        auto parts = StringTools::splitStringOnStringDelimiter(key, "[");
        auto eventTargetObject = removeSpaces(parts[0]);
        auto eventTargetProperty = toPrimitiveProperty(removeSpaces(removeLastCharacter(parts[1])));

        try
        {
          auto v = getAnyFromProperty(value, eventTargetProperty);
          ret.addToList(eventTargetObject, eventTargetProperty, v);
        }
        catch(...)
        {
          DebugLevel::warning("Could not create value from init:", key, ":", value.template get<std::string>());
        }
      });
    });

    return ret;
  }

  ControlInstance::EventConnections parseEventConnections(const json& j)
  {
    ControlInstance::EventConnections ret;
    JSONTools::forEachJsonInTag(j, "Events", [&](const auto& entry) {
      std::string str = entry;
      auto connections = StringTools::splitStringOnAnyDelimiter(str, ',');
      for(auto& connection : connections)
      {

        if(connection.find("=>") == Glib::ustring::npos)
          DebugLevel::throwException("Event Routing syntax error: missing \"=>\"", connection);

        auto parts = StringTools::splitStringOnStringDelimiter(connection, "=>");
        auto eventTargetParts = StringTools::splitStringOnAnyDelimiter(parts[1], '[');

        auto eventSource = toEventSources(removeSpaces(parts[0]));
        auto eventTargetObject = removeSpaces(eventTargetParts[0]);
        auto eventTargetProperty = toPrimitiveProperty(removeSpaces(removeLastCharacter(eventTargetParts[1])));

        ret.push_back({ eventSource, eventTargetObject, eventTargetProperty });
      }
    });

    return ret;
  }

  LayoutClass::ControlInstanceList toControlInstanceList(const json& j)
  {
    LayoutClass::ControlInstanceList l{};
    JSONTools::forEachJsonChild(j, [&](const auto& obj) {
      auto key = obj.key();
      auto value = obj.value();
      auto controlInstances = key;
      auto controlClasses = value.at("Class");
      auto point = toPoint(value.at("Position"));
      auto staticInit = parseInit(value);
      auto eventConnections = parseEventConnections(value);
      l.emplace_back(controlInstances, controlClasses, point, eventConnections, staticInit);
    });
    return l;
  }

  LayoutClass::EventSinkList toEventSinkList(const json& j)
  {
    LayoutClass::EventSinkList l{};
    JSONTools::forEachJsonChild(j, [&](const auto& obj) {
      auto button = std::to_string(obj.key());
      auto target = obj.value();

      if(button.find("+") != Glib::ustring::npos)
      {
        auto buttonOnly = button.substr(0, button.size() - 1);
        l.push_back(EventSinkMapping(toButtons(buttonOnly), toEventSinks(target), ButtonEvents::Down, true));
      }
      else
      {
        l.push_back(EventSinkMapping(toButtons(button), toEventSinks(target)));
      }
    });
    return l;
  }

  tConditionList toConditions(const json& j)
  {
    tConditionList ret;

    JSONTools::forEachJsonArrayElementChild(
        j, [&](const auto& conditionString) { ret.push_back(ConditionRegistry::get().getLambda(conditionString)); });
    return ret;
  }

  template <class T, typename tToT, typename tTag>
  T parseTFromTag(tToT toT, const nlohmann::json& obj, const tTag& tag, bool optional = true)
  {
    try
    {
      return toT(obj.at(tag));
    }
    catch(nlohmann::json::out_of_range& notFoundException)
    {
      auto desc = ExceptionTools::handle_eptr(std::current_exception());
      nltools::Log::error(desc);

      if(!optional)
        std::rethrow_exception(std::current_exception());
    }
    catch(nlohmann::json::invalid_iterator& conversionError)
    {
      auto desc = ExceptionTools::handle_eptr(std::current_exception());
      nltools::Log::error(desc);
      std::rethrow_exception(std::current_exception());
    }
    catch(...)
    {
      auto desc = ExceptionTools::handle_eptr(std::current_exception());
      nltools::Log::error(desc);
      std::rethrow_exception(std::current_exception());
    }
    return {};
  }

  void parseLayout(const json& j)
  {
    JSONTools::forEachJsonChild(j, [&](const auto& obj) {
      auto name = obj.key();
      DebugLevel::gassy("importing layout", name);
      const auto& id = name;
      try
      {
        auto selectionConditions = parseTFromTag<tConditionList>(toConditions, obj.value(), "Conditions");
        auto sinkMappings = parseTFromTag<LayoutClass::EventSinkList>(toEventSinkList, obj.value(), "EventSinks");
        auto selectors = parseTFromTag<std::list<Selector>>(toSelectors, obj.value(), "Selector", false);
        auto controls
            = parseTFromTag<LayoutClass::ControlInstanceList>(toControlInstanceList, obj.value(), "Controls", false);
        BoledLayoutFactory::get().registerLayout(id, selectors, controls, sinkMappings, selectionConditions);
      }
      catch(...)
      {
        auto desc = ExceptionTools::handle_eptr(std::current_exception());
        nltools::Log::error("Error Parsing:", name, ":", desc);
      }
    });
  }

  void importLayout(const std::string& fileName)
  {
    DebugLevel::gassy("importing layouts from file", fileName);
    std::ifstream i(fileName);
    json j;
    i >> j;

    auto it = j.find("layouts");
    if(it != j.end())
    {
      json layouts = *it;
      parseLayout(layouts);
    }

    BoledLayoutFactory::get().sortByPriority();
  }
}
