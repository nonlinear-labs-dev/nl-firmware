#pragma once
#include "BasicItem.h"

class BasicItemWithValueText : public BasicItem
{
 public:
  BasicItemWithValueText(const Glib::ustring& label, const Glib::ustring& value, const Rect& r);
};
