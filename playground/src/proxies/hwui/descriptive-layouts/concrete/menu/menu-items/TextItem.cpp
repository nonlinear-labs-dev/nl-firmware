#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include "TextItem.h"

TextItem::TextItem(const std::string& text, const Rect& r)
    : BasicItem(text, r)
{
}

void TextItem::doAction()
{
}

bool TextItem::canEnter()
{
  return false;
}
