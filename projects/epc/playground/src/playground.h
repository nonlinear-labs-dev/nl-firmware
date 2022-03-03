#pragma once

#include <nltools/GlibFwd.h>
#include <stdlib.h>
#include <type_traits>
#include <string>
#include <memory>
#include "playground-helpers.h"
#include "nltools/logging/Log.h"
#include <glibmm/ustring.h>

void printLastFunctions();

typedef int32_t tAudioControlValue;

namespace std
{
  template <> struct hash<Glib::ustring> : public __hash_base<size_t, Glib::ustring>
  {
    size_t operator()(const Glib::ustring& p) const noexcept
    {
      std::hash<std::string> h;
      return h(p.raw());
    }
  };

  inline basic_istream<char>& getline(basic_istream<char>& in, Glib::ustring& target)
  {
    std::string str;
    auto& res = getline(in, str);
    target = str;
    return res;
  }

  template <typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true> std::string to_string(const T& e)
  {
    return to_string(static_cast<int>(e));
  }

  inline Glib::ustring to_string(const Glib::ustring& in)
  {
    return in;
  }
}

template <typename T, std::enable_if_t<std::is_enum<T>::value, bool> = true>
std::ostream& operator<<(std::ostream& stream, const T& e)
{
  wirteIntToStream(stream, static_cast<int>(e));
  return stream;
}

template <class T> inline void hash_combine(std::size_t& seed, const T& v)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T> inline std::string to_string(const T& t)
{
  return std::to_string(t);
}

template <> inline std::string to_string<std::string>(const std::string& t)
{
  return t;
}

typedef int16_t tTcdValue;
typedef double tDisplayValue;
typedef double tControlPositionValue;

enum class Initiator
{
  EXPLICIT_LOAD,
  EXPLICIT_WEBUI,
  EXPLICIT_HWUI,
  EXPLICIT_PLAYCONTROLLER,
  EXPLICIT_OTHER,
  EXPLICIT_MCVIEW,
  EXPLICIT_USECASE,
  INDIRECT_SPLIT_SYNC,
  INDIRECT,
  EXPLICIT_TCD,
  EXPLICIT_MIDI,
  INDIRECT_SETTING_CHANGED
};

inline std::string toString(Initiator initiator)
{
  switch(initiator)
  {
    case Initiator::EXPLICIT_LOAD:
      return "EXPLICIT_LOAD";
    case Initiator::EXPLICIT_WEBUI:
      return "EXPLICIT_WEBUI";
    case Initiator::EXPLICIT_HWUI:
      return "EXPLICIT_HWUI";
    case Initiator::EXPLICIT_PLAYCONTROLLER:
      return "EXPLICIT_PLAYCONTROLLER";
    case Initiator::EXPLICIT_OTHER:
      return "EXPLICIT_OTHER";
    case Initiator::EXPLICIT_MCVIEW:
      return "EXPLICIT_MCVIEW";
    case Initiator::EXPLICIT_USECASE:
      return "EXPLICIT_USECASE";
    case Initiator::INDIRECT_SPLIT_SYNC:
      return "INDIRECT_SPLIT_SYNC";
    case Initiator::INDIRECT:
      return "INDIRECT";
    case Initiator::EXPLICIT_TCD:
      return "EXPLICIT_TCD";
    case Initiator::EXPLICIT_MIDI:
      return "EXPLICIT_MIDI";
    case Initiator::INDIRECT_SETTING_CHANGED:
      return "INDIRECT_SETTING_CHANGED";
    default:
      return "";
  }
}

enum class PresetType : uint8_t
{
  Single,
  Morph,
  Multi
};

enum class SaveResult : uint8_t
{
  Nothing,
  Again,
  Finished
};

std::string getStackTrace(const std::string& prefix);
void printbacktrace(nltools::Log::Level level, int maxFrames = 500);

#ifdef _TESTS
#define FOR_TESTS(expression) expression
#else
#define FOR_TESTS(expression)
#endif
