#include "InfoContent.h"
#include <nltools/Assert.h>

InfoContent::InfoContent()
    : super(Rect(0, 0, 0, 64))
{
}

void InfoContent::setPosition(const Rect& rect)
{
  super::setPosition(rect);
  fixLayout();
}

const Rect& InfoContent::getPosition() const
{
  return super::getPosition();
}

void InfoContent::setDirty()
{
  super::setDirty();
  notifyDirty(true);
}

InfoField* InfoContent::addInfoField(const std::string& lineIdentifier, const Glib::ustring& labelText, Control* field)
{
  infoOrder.emplace_back(lineIdentifier);
  auto label = addControl(new SingleLineInfoContent(labelText));
  addControl(field);
  infoFields[lineIdentifier] = std::make_unique<InfoField>(label, field);
  return infoFields[lineIdentifier].get();
}

InfoField* InfoContent::addInfoField(const std::string& lineIdentifier, const Glib::ustring& labelText)
{
  return addInfoField(lineIdentifier, labelText, new SingleLineInfoContent());
}

void InfoContent::fixLayout()
{
  int y = 0;

  for(const auto& infoKey : infoOrder)
  {
    y = infoFields[infoKey]->format(y);
  }

  Rect r = getPosition();
  r.setHeight(y);
  ControlWithChildren::setPosition(r);
}

void InfoContent::updateContent()
{
  fillContents();
  fixLayout();
}
