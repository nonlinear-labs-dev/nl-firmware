#include <testing/TestHelper.h>
#include <Application.h>
#include <device-info/DeviceInformation.h>
#include <device-info/UniqueHardwareID.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"UHID formatting test")
{
  auto info = Application::get().getDeviceInformation()->getItem<::UniqueHardwareID>();

  info->onIDReceived(0);
  CHECK(info->get() == "0000-0000-0000-0000");

  info->onIDReceived(std::numeric_limits<uint64_t>::min());
  CHECK(info->get() == "0000-0000-0000-0000");

  info->onIDReceived(std::numeric_limits<uint64_t>::max());
  CHECK(info->get() == "FFFF-FFFF-FFFF-FFFF");

  info->onIDReceived(0x0BCD0F0103450789);
  CHECK(info->get() == "0BCD-0F01-0345-0789");
}
