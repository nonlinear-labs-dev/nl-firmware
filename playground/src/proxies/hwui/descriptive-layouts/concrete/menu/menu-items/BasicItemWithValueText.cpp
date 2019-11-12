#include "BasicItemWithValueText.h"

BasicItemWithValueText::BasicItemWithValueText(const Glib::ustring &label, const Glib::ustring &value, const Rect &r)
    : BasicItem(label, r)
{
  auto rightHalf = r;
  rightHalf.setWidth(rightHalf.getWidth() / 2);
  rightHalf.setLeft(r.getLeft() + rightHalf.getWidth());
  addControl(new CaptionLabel<LeftAlignedLabel>(value, rightHalf, true));
}
