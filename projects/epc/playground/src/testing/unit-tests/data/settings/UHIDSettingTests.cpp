#include <testing/TestHelper.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <Application.h>
#include <device-info/DeviceInformation.h>
#include <device-info/SerialNumber.h>

TEST_CASE("UHID formatting test")
{
  auto proxy = Application::get().getPlaycontrollerProxy();
  auto info = Application::get().getDeviceInformation()->getItem<::SerialNumber>();
  
  proxy->setUHID(0);
  CHECK(info->get() == "0000-0000-0000-0000");

  proxy->setUHID(std::numeric_limits<uint64_t>::min());
  CHECK(info->get() == "0000-0000-0000-0000");

  proxy->setUHID(std::numeric_limits<uint64_t>::max());
  CHECK(info->get() == "FFFF-FFFF-FFFF-FFFF");

  proxy->setUHID(0xABCDEF0123456789);
  CHECK(info->get() == "ABCD-EF01-2345-6789");
}
