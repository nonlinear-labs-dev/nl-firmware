#include <Application.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/setup/UISoftwareVersionEditor.h>

UISoftwareVersionEditor::UISoftwareVersionEditor()
{
  addEntry("branch", "Branch:", "renew-nonmaps-project-with-maven");
  addEntry("commits", "Commits:", "2242");
  addEntry("head", "HEAD:", "4fbdde62");
  doLayout();
}

void UISoftwareVersionEditor::setPosition(const Rect &)
{
  doLayout();
}
