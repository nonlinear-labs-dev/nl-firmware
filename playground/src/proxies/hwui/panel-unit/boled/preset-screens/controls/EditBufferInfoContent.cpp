#include <Application.h>
#include <tools/TimeTools.h>
#include "EditBufferInfoContent.h"

EditBufferInfoContent::EditBufferInfoContent()
    : InfoContent()
{
  addInfoField("name", "Name", new MultiLineContent());
  addInfoField("comment", "Comment", new MultiLineContent());
  addInfoField("lastchange", "Last Change");
  addInfoField("uiversion", "UI Version");

  fillDefaults();

  Application::get().getPresetManager()->getEditBuffer()->onChange(
      mem_fun(this, &EditBufferInfoContent::onEditBufferChanged));
}

EditBufferInfoContent::~EditBufferInfoContent()
{
}

void EditBufferInfoContent::fixLayout()
{
  int y = 0;

  for(auto infoKey : { "name", "comment", "lastchange", "uiversion" })
  {
    y = infoFields[infoKey]->format(y);
  }

  Rect r = getPosition();
  r.setHeight(y);
  super::setPosition(r);
}

bool EditBufferInfoContent::fillFromEditBuffer()
{
  auto eb = getEditBuffer();
  infoFields["name"]->setInfo(eb->getName(), FrameBuffer::Colors::C128);
  infoFields["comment"]->setInfo(eb->getAttribute("Comment", "---"), FrameBuffer::Colors::C128);
  infoFields["lastchange"]->setInfo(TimeTools::getDisplayStringFromIso(eb->getAttribute("StoreTime", "---")));
  infoFields["uiversion"]->setInfo(eb->getAttribute("SoftwareVersion", "---"));
  return true;
}

void EditBufferInfoContent::fillDefaults()
{
  infoFields["name"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["comment"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["lastchange"]->setInfo("---");
  infoFields["uiversion"]->setInfo("---");
}

EditBuffer *EditBufferInfoContent::getEditBuffer()
{
  return Application::get().getPresetManager()->getEditBuffer().get();
}

void EditBufferInfoContent::onEditBufferChanged()
{
  if(fillFromEditBuffer())
  {
    fixLayout();
  }
}
