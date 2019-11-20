#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/descriptive-layouts/primitives/Border.h>
#include <proxies/hwui/panel-unit/boled/setup/SmallerParameterValueLabel.h>
#include "MonoGroupControl.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

MonoGroupControl::MonoGroupControl(const Rect &r)
    : ControlWithChildren(r)
    , m_grid { nullptr }
{
  m_connection = Application::get().getHWUI()->onCurrentVoiceGroupChanged([this](auto) { rebuild(); });
}

MonoGroupControl::~MonoGroupControl()
{
  m_connection.disconnect();
}

void MonoGroupControl::rebuild()
{
  clear();

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getHWUI()->getCurrentVoiceGroup();
  auto monoEnableParam = eb->findParameterByID(12345, vg);
  auto monoEnable = monoEnableParam->getDisplayString() == "On";

  auto paramid = std::array<int, 4> { 12345, 12346, 12347, 12348 };

  const auto width = 62;
  const auto height = 16;

  auto index = 0;

  for(auto y = 0; y < 2; y++)
  {
    for(auto x = 0; x < 2; x++)
    {
      auto param = eb->findParameterByID(paramid[index++], vg);

      if(paramid[y + x] != 12345)
      {
        auto str = param->getShortName() + ": " + param->getDisplayString();
        m_grid[y][x] = addControl(new Label(str, { (x * width) + (x), y * height + (y), width, height }));
      }
      else
      {
        auto str = param->getDisplayString();
        m_grid[y][x] = addControl(new Label(str, { (x * width) + (x), y * height + (y), width, height }));
      }
    }
  }
}
