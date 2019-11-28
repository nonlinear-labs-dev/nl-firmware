#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <cassert>

#include "SingleLineInfoContent.h"
#include "MultiLineInfoContent.h"
#include "InfoField.h"

class InfoContent : public Scrollable, public ControlWithChildren
{
 private:
  typedef ControlWithChildren super;

 public:
  InfoContent();

  void setPosition(const Rect& rect) override;
  const Rect& getPosition() const override;
  void setDirty() override;

  virtual void fillContents() = 0;
  void updateContent();

  InfoField* addInfoField(const std::string& lineIdentifier, const Glib::ustring& labelText, Control* field);
  InfoField* addInfoField(const std::string& lineIdentifier, const ustring& labelText);

 protected:
  void fixLayout();

  std::vector<std::string> infoOrder;
  std::map<std::string, std::unique_ptr<InfoField>> infoFields;
};
