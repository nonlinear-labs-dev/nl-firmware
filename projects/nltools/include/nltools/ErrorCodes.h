#pragma once
#include <nltools/enums/EnumTools.h>

namespace nltools
{
  ENUM(ErrorCode, int, UnknownOptionKeyError = 20, OptionConvertError = 21);
}