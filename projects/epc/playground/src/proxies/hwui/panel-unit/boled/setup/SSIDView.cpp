#include <Application.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <device-settings/SSID.h>
#include <proxies/hwui/panel-unit/boled/setup/SSIDView.h>

SSIDView::SSIDView()
    : base_type("", Rect(0, 0, 0, 0))
{
  Application::get().getDeviceInformation()->getItem<SSID>()->onChange(mem_fun(this, &SSIDView::onSettingChanged));
}

SSIDView::~SSIDView()
{
}

void SSIDView::onSettingChanged(const DeviceInformationItem *s)
{
  if(auto v = dynamic_cast<const SSID *>(s))
  {
    setText(v->get());
  }
}
