#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include "TextItem.h"

TextItem::TextItem(const std::string &text)
    : BasicItem(text)
{
}

void TextItem::doAction()
{
}

bool TextItem::canEnter()
{
  return false;
}
