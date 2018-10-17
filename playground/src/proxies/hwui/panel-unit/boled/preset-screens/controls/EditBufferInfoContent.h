#pragma once

#include <proxies/hwui/panel-unit/boled/InfoContent.h>
#include <presets/EditBuffer.h>

class EditBufferInfoContent : public InfoContent
{
 private:
  typedef InfoContent super;

 public:
  EditBufferInfoContent();
  virtual ~EditBufferInfoContent();

 private:
  void fixLayout();
  bool fillFromEditBuffer();
  void fillDefaults();
  EditBuffer *getEditBuffer();
  void onEditBufferChanged();

  sigc::connection m_bankConnection;
};
