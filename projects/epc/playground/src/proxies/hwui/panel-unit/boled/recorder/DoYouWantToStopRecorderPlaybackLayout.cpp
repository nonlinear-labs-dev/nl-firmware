#include "DoYouWantToStopRecorderPlaybackLayout.h"
#include "proxies/hwui/controls/labels/LabelStyleable.h"
#include "proxies/hwui/controls/Button.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include "use-cases/RecorderManager.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h"

DoYouWantToStopRecorderPlaybackLayout::DoYouWantToStopRecorderPlaybackLayout()
    : Layout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
{
  LabelStyle headlineStyle{};
  headlineStyle.size = FontSize::Size9;
  headlineStyle.decoration = FontDecoration::Bold;

  auto headline = addControl(new LabelStyleable("Recorder still in playback!", {0, 0, 256, 16}, headlineStyle));
  auto content = addControl(new MultiLineLabel({8, 16, 248, 48}, "All recorder tabs have been closed. Do you want to stop the playback?\n"
                                                                   "It can also be stopped anytime in the Setup menu."));
  auto nope = addControl(new Button("No", Buttons::BUTTON_A));
  auto okay = addControl(new Button("Yes", Buttons::BUTTON_D));
}

bool DoYouWantToStopRecorderPlaybackLayout::onButton(Buttons i, bool down, ::ButtonModifiers modifiers)
{
  if(down && i == Buttons::BUTTON_A)
  {
    Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().resetOverlay();
    return true;
  }

  if(down && i == Buttons::BUTTON_D)
  {
    RecorderManager::stopRecorderPlayback();
    Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().resetOverlay();
    return true;
  }

  return Layout::onButton(i, down, modifiers);
}
