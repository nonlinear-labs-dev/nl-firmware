#include <device-settings/WifiSetting.h>
#include <Application.h>
#include "WiFiSettingView.h"

WiFiSettingView::WiFiSettingView() : base_type("", Rect(0, 0, 0, 0))
{
    Application::get().getSettings()->getSetting<WifiSetting>()->onChange(
            mem_fun(this, &WiFiSettingView::onSettingChanged));
}

WiFiSettingView::~WiFiSettingView() = default;

void WiFiSettingView::onSettingChanged(const Setting *s) {
    auto v = dynamic_cast<const WifiSetting *>(s);
    setText(v->getDisplayString());
}
