#include <Application.h>
#include <proxies/hwui/descriptive-layouts/primitives/Border.h>
#include <proxies/hwui/panel-unit/boled/setup/SmallerParameterValueLabel.h>
#include "MonoGroupControl.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

MonoGroupControl::MonoGroupControl(const Rect &r)
    : ControlWithChildren(r)
    , m_grid{ nullptr }
{
  m_connection = Application::get().getVoiceGroupSelectionHardwareUI()->onHwuiSelectionChanged([this]() { rebuild(); });
}

MonoGroupControl::~MonoGroupControl()
{
  m_connection.disconnect();
}

void MonoGroupControl::rebuild()
{
  clear();

  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto vg = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  auto monoEnableParam = eb->findParameterByID(364, vg);
  auto monoEnable = monoEnableParam->getDisplayString() == "On";

  auto paramid = std::array<int, 4>{ 364, 366, 367, 365 };

  const auto width = 62;
  const auto height = 16;

  auto index = 0;

  for(auto y = 0; y < 2; y++)
  {
    for(auto x = 0; x < 2; x++)
    {
      auto param = eb->findParameterByID(paramid[index++], vg);

      if(paramid[y + x] != 364)
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
