#pragma once

#include <proxies/hwui/panel-unit/boled/preset-screens/RenameLayout.h>

class RenameDeviceLayout : public RenameLayout
{
 public:
  RenameDeviceLayout(std::shared_ptr<Layout> restore);
  virtual ~RenameDeviceLayout();

  virtual void commit(const Glib::ustring &newName) override;
  virtual Glib::ustring getInitialText() const override;

 private:
  std::shared_ptr<Layout> m_restore;
};
