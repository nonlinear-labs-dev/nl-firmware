#pragma once
#include "ModuleCaption.h"

class FixedTextModuleCaption : public ModuleCaption
{
 public:
  FixedTextModuleCaption(const Rect& r, const Glib::ustring& text);
};
