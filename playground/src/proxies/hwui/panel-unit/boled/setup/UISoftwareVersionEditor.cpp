#include <Application.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include "UISoftwareVersionEditor.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

UISoftwareVersionEditor::UISoftwareVersionEditor()
    : m_head(TOSTRING(HEAD_REF))
    , m_commitCount(TOSTRING(COMMIT_COUNT))
    , m_branch(TOSTRING(BRANCH_NAME))
    , DisplayEditor()
{
  int y = 0;
  addEntry("branch", "Branch:", m_branch);
  addEntry("commits", "Commits:", m_commitCount);
  addEntry("head", "HEAD:", m_head);
  doLayout();
}

void UISoftwareVersionEditor::setPosition(const Rect &)
{
  doLayout();
}
