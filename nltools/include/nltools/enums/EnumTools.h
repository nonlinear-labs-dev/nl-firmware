#pragma once

#include <string>
#include <map>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <nltools/ExceptionTools.h>

namespace EnumTools
{
  template <typename enum_type, typename base_type, typename struct_type>
  std::map<enum_type, std::string> createMap(const std::string &def)
  {
    std::map<enum_type, std::string> ret;

    auto invalidValue = (base_type) -1;
    auto numEnums = sizeof(struct_type) / sizeof(base_type);
    base_type plain[numEnums];

    for(size_t i = 0; i < numEnums; i++)
      plain[i] = invalidValue;

    new(plain) struct_type;
    base_type currentValue = 0;

    for(size_t i = 0; i < numEnums; i++)
    {
      if(plain[i] == invalidValue)
        plain[i] = currentValue;
      else
        currentValue = plain[i];
      currentValue++;
    }

    std::vector<std::string> strs;
    boost::split(strs, def, boost::is_any_of(","));

    g_assert(strs.size() == numEnums);

    for(size_t idx = 0; idx < numEnums; idx++)
    {
      std::string key = strs[idx];
      key = boost::trim_copy(key);
      auto pos = key.find_first_of(" =,;\n\r");
      if(pos != std::string::npos)
        key = key.substr(0, pos);

      ret[(enum_type) plain[idx]] = key;
    }

    return ret;
  }

  template <typename enum_type> std::map<std::string, enum_type> reverse(const std::map<enum_type, std::string> &m)
  {
    std::map<std::string, enum_type> ret;
    for(auto &a : m)
      ret[a.second] = a.first;

    return ret;
  }
}

#define ENUM(enumName, type, enums...)                                                                                 \
  enum class enumName : type                                                                                           \
  {                                                                                                                    \
    enums                                                                                                              \
  };                                                                                                                   \
  struct __attribute__((packed)) ValuesOf##enumName                                                                    \
  {                                                                                                                    \
    type enums;                                                                                                        \
  };                                                                                                                   \
  template <typename E> const std::map<E, std::string> &getMap();                                                      \
  template <> inline const std::map<enumName, std::string> &getMap<enumName>()                                         \
  {                                                                                                                    \
    static auto m = EnumTools::createMap<enumName, type, ValuesOf##enumName>(#enums);                                  \
    return m;                                                                                                          \
  }                                                                                                                    \
  template <typename E> E to(const std::string &e);                                                                    \
  template <> inline enumName to<enumName>(const std::string &e)                                                       \
  {                                                                                                                    \
    static auto map = EnumTools::reverse(getMap<enumName>());                                                          \
    auto it = map.find(e);                                                                                             \
    if(it != map.end())                                                                                                \
      return it->second;                                                                                               \
    nltools::throwException("Could not find value", e, "in enum map for", #enumName);                                  \
    return static_cast<enumName>(0);                                                                                   \
  }                                                                                                                    \
  inline std::string toString(const enumName &e)                                                                       \
  {                                                                                                                    \
    auto it = getMap<enumName>().find(e);                                                                              \
    if(it != getMap<enumName>().end())                                                                                 \
      return it->second;                                                                                               \
    nltools::throwException("Could not find value", static_cast<int>(e), "in enum map for", #enumName);                \
    return "";                                                                                                         \
  }                                                                                                                    \
  template <typename E> void forEachValue(std::function<void(E v)> cb);                                                \
  template <> inline void forEachValue<enumName>(std::function<void(enumName v)> cb)                                   \
  {                                                                                                                    \
    for(auto &a : getMap<enumName>())                                                                                  \
      cb(a.first);                                                                                                     \
  }                                                                                                                    \
  template <typename E> void forEachString(std::function<void(const std::string &v)> cb);                              \
  template <> inline void forEachString<enumName>(std::function<void(const std::string &v)> cb)                        \
  {                                                                                                                    \
    for(auto &a : getMap<enumName>())                                                                                  \
      cb(a.second);                                                                                                    \
  }                                                                                                                    \
  template <typename E> std::vector<std::string> getAllStrings();                                                      \
  template <> inline std::vector<std::string> getAllStrings<enumName>()                                                \
  {                                                                                                                    \
    std::vector<std::string> ret;                                                                                      \
    for(auto &a : getMap<enumName>())                                                                                  \
      ret.push_back(a.second);                                                                                         \
    return ret;                                                                                                        \
  }                                                                                                                    \
  template <typename E> std::vector<E> getAllValues();                                                                 \
  template <> inline std::vector<enumName> getAllValues<enumName>()                                                    \
  {                                                                                                                    \
    std::vector<enumName> ret;                                                                                         \
    for(auto &a : getMap<enumName>())                                                                                  \
      ret.push_back(a.first);                                                                                          \
    return ret;                                                                                                        \
  }
