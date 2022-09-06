#include "FixedTextModuleCaption.h"

FixedTextModuleCaption::FixedTextModuleCaption(const Rect& r, const Glib::ustring& text)
    : ModuleCaption(r, false)
{
  setText({ text, 0 });
}
