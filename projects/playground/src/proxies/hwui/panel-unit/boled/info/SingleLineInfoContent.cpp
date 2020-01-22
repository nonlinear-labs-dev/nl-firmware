#include "SingleLineInfoContent.h"

SingleLineInfoContent::SingleLineInfoContent(Glib::ustring name)
    : LeftAlignedLabel(name, Rect(0, 0, 0, 0))
{
}

SingleLineInfoContent::SingleLineInfoContent()
    : LeftAlignedLabel("---", Rect(0, 0, 0, 0))
{
}
