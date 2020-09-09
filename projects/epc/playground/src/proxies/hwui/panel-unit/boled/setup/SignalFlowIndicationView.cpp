#include <proxies/hwui/panel-unit/boled/setup/SignalFlowIndicationView.h>
#include <device-settings/Settings.h>
#include <device-settings/SignalFlowIndicationSetting.h>
#include <Application.h>

SignalFlowIndicationView::SignalFlowIndicationView()
    : SetupLabel("", Rect(0, 0, 0, 0))
{
  Application::get().getSettings()->getSetting<SignalFlowIndicationSetting>()->onChange(
      mem_fun(this, &SignalFlowIndicationView::onSettingChanged));
}

void SignalFlowIndicationView::onSettingChanged(const Setting *s)
{
  auto v = dynamic_cast<const SignalFlowIndicationSetting *>(s);
  setText(v->getDisplayString());
}
