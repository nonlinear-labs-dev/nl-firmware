#include <testing/TestHelper.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <Application.h>
#include <device-info/DeviceInformation.h>
#include <device-info/UniqueHardwareID.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"UHID formatting test")
{
  auto proxy = Application::get().getPlaycontrollerProxy();
  auto info = Application::get().getDeviceInformation()->getItem<::UniqueHardwareID>();

  proxy->setUHID(0);
  CHECK(info->get() == "0000-0000-0000-0000");

  proxy->setUHID(std::numeric_limits<uint64_t>::min());
  TestHelper::doMainLoopIteration();
  CHECK(info->get() == "0000-0000-0000-0000");

  proxy->setUHID(std::numeric_limits<uint64_t>::max());
  TestHelper::doMainLoopIteration();
  CHECK(info->get() == "FFFF-FFFF-FFFF-FFFF");

  proxy->setUHID(0x0BCD0F0103450789);
  TestHelper::doMainLoopIteration();
  CHECK(info->get() == "0BCD-0F01-0345-0789");
}
