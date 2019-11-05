#include "MultiLineInfoContent.h"

MultiLineInfoContent::MultiLineInfoContent()
    : MultiLineLabel("---")
{
}

void MultiLineInfoContent::setPosition(const Rect& rect)
{
  auto re = rect;
  re.moveBy(0, 2);
  MultiLineLabel::setPosition(re);
}