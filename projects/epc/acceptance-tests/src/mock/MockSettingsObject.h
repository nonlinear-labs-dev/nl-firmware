#pragma once
#include <device-settings/Settings.h>
#include <functional>

class MockSettingsObject : public Settings
{
 public:
  typedef std::function<int(uint64_t)> tOnChangeCB;
  explicit MockSettingsObject(UpdateDocumentMaster* m);

  void setOnChangeCB(tOnChangeCB&& cb);
  tUpdateID onChange(uint64_t flags) override;

 protected:
  const Glib::ustring& getSettingFileNameToLoadFrom() const override;

 private:
  tOnChangeCB m_onChangeCB;
};