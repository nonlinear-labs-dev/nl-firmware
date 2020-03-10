#include <proxies/hwui/panel-unit/boled/setup/UpdateAvailableView.h>
#include <glibmm/main.h>
#include <tools/SpawnCommandLine.h>

static const auto c_updateFileName = "/mnt/usb-stick/nonlinear-c15-update.tar";

UpdateAvailableView::UpdateAvailableView()
    : super("", Rect(0, 0, 0, 0))
{
  poll();
  Glib::MainContext::get_default()->signal_timeout().connect_seconds(mem_fun(this, &UpdateAvailableView::poll), 5);
}

UpdateAvailableView::~UpdateAvailableView()
{
}

bool UpdateAvailableView::poll()
{
  if(updateExists())
  {
    setText("Yes");
  }
  else
  {
    setText("No");
  }
  return true;
}

bool UpdateAvailableView::updateExists()
{
#ifdef _DEVELOPMENT_PC
  return false;
#endif
  SpawnCommandLine cmd(
      "ssh -o \"StrictHostKeyChecking=no\" root@192.168.10.11 'ls /mnt/usb-stick/nonlinear-c15-update.tar'");
  return cmd.getExitStatus() == 0;
}
