#pragma once
#include <device-settings/Settings.h>

class MockSettingsObject : public Settings
{
 public:
  typedef std::function<int(uint64_t)> tOnChangeCB;
  explicit MockSettingsObject(const Glib::ustring& file, UpdateDocumentMaster* m);

  void setOnChangeCB(tOnChangeCB&& cb);
  tUpdateID onChange(uint64_t flags) override;

 private:
  tOnChangeCB m_onChangeCB;
};