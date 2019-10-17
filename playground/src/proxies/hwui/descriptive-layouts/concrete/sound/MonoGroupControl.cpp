#include <Application.h>
#include <proxies/hwui/descriptive-layouts/primitives/Border.h>
#include <proxies/hwui/panel-unit/boled/setup/SmallerParameterValueLabel.h>
#include "MonoGroupControl.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

MonoGroupControl::MonoGroupControl(const Rect &r)
    : ControlWithChildren(r)
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
  auto monoEnableParam = eb->findParameterByID(12345, vg);
  auto monoEnable = monoEnableParam->getDisplayString() == "On";

  //Non enable Parameters
  auto i = 0;
  for(auto id : { 12346, 12347, 12348 })
  {
    auto y = i++ * 10;
    auto param = eb->findParameterByID(id, vg);
    auto label = addControl(new Label(param->getShortName(), { 0, y, 40, 10 }));
    auto val = addControl(new SmallerParamValueLabel(param, { 41, y, 50, 10 }));
    label->setHighlight(monoEnable);
    val->setHighlight(monoEnable);
  }
}
