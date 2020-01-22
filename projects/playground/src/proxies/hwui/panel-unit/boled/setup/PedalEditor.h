#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include "MenuEditor.h"

class Setting;
class PedalType;

class PedalEditor : public MenuEditor
{
 private:
  typedef MenuEditor super;

 public:
  PedalEditor(std::shared_ptr<PedalType> m);
  virtual ~PedalEditor();

  virtual void incSetting(int inc) override;
  virtual const std::vector<Glib::ustring> &getDisplayStrings() const override;
  virtual int getSelectedIndex() const override;

 private:
  std::shared_ptr<PedalType> m_mode;
};
