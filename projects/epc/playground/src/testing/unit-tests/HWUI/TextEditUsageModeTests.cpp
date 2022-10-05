#include <testing/TestHelper.h>
#include <proxies/hwui/TextEditUsageMode.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"TextEditUsageMode", "[HWUI")
{
  TextEditUsageMode mode("");
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Normal);

  mode.toggleCapsLock();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);

  mode.toggleCapsLock();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Normal);

  mode.toggleCapsLock();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);

  mode.toggleSymbol();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::SymbolShift);

  mode.toggleSymbol();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);

  mode.toggleSymbol();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::SymbolShift);

  mode.toggleCapsLock();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Symbol);

  mode.toggleSymbol();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Normal);

  mode.handleShiftButton(true);
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);

  mode.handleShiftButton(false);
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Normal);

  mode.toggleCapsLock();
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);

  mode.handleShiftButton(true);
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Normal);

  mode.handleShiftButton(false);
  CHECK(mode.getLayout() == TextEditUsageMode::Layout::Shift);
}