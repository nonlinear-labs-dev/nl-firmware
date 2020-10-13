#include <xml/VersionAttribute.h>

VersionAttribute::VersionAttribute(int v)
    : Attribute("version", v)
{
}

VersionAttribute::~VersionAttribute()
{
}

VersionAttribute &VersionAttribute::get()
{
  static VersionAttribute currentFileVersion(getCurrentFileVersion());
  return currentFileVersion;
}

int VersionAttribute::getCurrentFileVersion()
{
  enum class History
  {
    Reaktor = 5,
    Version1_5 = 6,
    NAMM_2020 = 7,
    FIX_UNISON_VOICES = 8,
    DUAL_SPLIT_POINT = 9
  };

  return static_cast<int>(History::DUAL_SPLIT_POINT);
}
