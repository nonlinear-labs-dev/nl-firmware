#include <proxies/hwui/panel-unit/boled/InfoContent.h>

InfoContent::InfoContent() :
    super(Rect(0, 0, 0, 64))
{

}

InfoContent::~InfoContent()
{

}

void InfoContent::setPosition(const Rect &rect)
{
  super::setPosition(rect);
}

const Rect &InfoContent::getPosition() const
{
  return super::getPosition();
}

void InfoContent::setDirty()
{
  super::setDirty();
  notifyDirty(true);
}

InfoContent::InfoField* InfoContent::addInfoField(std::string lineIdentifier, Label* label, Control* field)
{
  addControl(label);
  addControl(field);
  infoFields[lineIdentifier] = new InfoField(label, field);
  return infoFields[lineIdentifier];
}
