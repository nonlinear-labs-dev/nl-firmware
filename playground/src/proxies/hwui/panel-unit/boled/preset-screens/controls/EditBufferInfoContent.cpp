#include <Application.h>
#include <tools/TimeTools.h>
#include "EditBufferInfoContent.h"

EditBufferInfoContent::EditBufferInfoContent () : InfoContent() {
  addInfoField("name", "Name", new MultiLineContent());
  addInfoField("comment", "Comment", new MultiLineContent());
  addInfoField("lastchange", "Last Change");
  addInfoField("devicename", "Device Name");
  addInfoField("uiversion", "UI Version");

  Application::get().getPresetManager()->getEditBuffer()->onPresetChanged(
          mem_fun(this, &EditBufferInfoContent::onEditBufferChanged));
}

EditBufferInfoContent::~EditBufferInfoContent () {

}

void EditBufferInfoContent::fixLayout() {
  int y = 0;

  for(auto infoKey : { "name", "comment", "lastchange", "devicename", "uiversion" })
  {
    y = infoFields[infoKey]->format(y);
  }

  Rect r = getPosition();
  r.setHeight(y);
  super::setPosition(r);
}

bool EditBufferInfoContent::fillFromEditBuffer (const EditBuffer *eb) {
  infoFields["name"]->setInfo(eb->getName(), FrameBuffer::Colors::C128);
  infoFields["comment"]->setInfo(eb->getAttribute("Comment", "---"), FrameBuffer::Colors::C128);
  infoFields["lastchange"]->setInfo(TimeTools::getDisplayStringFromIso(eb->getAttribute("StoreTime", "---")));
  infoFields["devicename"]->setInfo(eb->getAttribute("DeviceName", "---"));
  infoFields["uiversion"]->setInfo(eb->getAttribute("SoftwareVersion", "---"));
  return true;
}

bool EditBufferInfoContent::fillDefaults()
{
  infoFields["name"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["comment"]->setInfo("---", FrameBuffer::Colors::C128);
  infoFields["lastchange"]->setInfo("---");
  infoFields["devicename"]->setInfo("---");
  infoFields["uiversion"]->setInfo("---");
  return true;
}

EditBuffer *EditBufferInfoContent::getCurrentEditBuffer () {
  return Application::get().getPresetManager()->getEditBuffer().get();
}

void EditBufferInfoContent::onEditBufferChanged() {
  if(auto eb = getCurrentEditBuffer())
  {
    connectToEditBuffer(eb);
  }
  else if(fillDefaults())
  {
    fixLayout();
    return;
  }

  if(fillFromEditBuffer(getCurrentEditBuffer())) {
    fixLayout();
  }
}
void EditBufferInfoContent::connectToEditBuffer (EditBuffer* eb) {
  m_editBufferConnection.disconnect();
  m_editBufferConnection = eb->onChange(mem_fun(this, &EditBufferInfoContent::onEditBufferChanged));

}