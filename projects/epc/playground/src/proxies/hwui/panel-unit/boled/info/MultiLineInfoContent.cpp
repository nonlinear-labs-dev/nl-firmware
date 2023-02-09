#include "MultiLineInfoContent.h"

MultiLineInfoContent::MultiLineInfoContent()
    : MultiLineLabel("---")
{
}

void MultiLineInfoContent::setPosition(const Rect& rect)
{
  auto re = rect;
  re.moveBy(0, getBottomPadding());
  MultiLineLabel::setPosition(re);
}

int MultiLineInfoContent::getBottomPadding()
{
  return 2;
}
