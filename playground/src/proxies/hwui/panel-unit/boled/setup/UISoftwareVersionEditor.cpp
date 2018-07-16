#include <Application.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <tools/FileTools.h>
#include "UISoftwareVersionEditor.h"

UISoftwareVersionEditor::UISoftwareVersionEditor() : m_head("A"), m_commitCount("78172"), m_branch("master"), DisplayEditor()
{
  prepareData();
  int y = 0;
  addEntry("branch", "Branch:", m_branch);
  addEntry("commits", "Commits:", m_commitCount);
  addEntry("head", "HEAD:", m_head);
  doLayout();
}


void UISoftwareVersionEditor::setPosition (const Rect &)
{
 doLayout();
}

void UISoftwareVersionEditor::prepareData() {
  m_branch = FileTools::readFromFile("resources/version-infos/git-branch");
  m_commitCount = FileTools::readFromFile("resources/version-infos/git-commit-count");
  m_head = FileTools::readFromFile("resources/version-infos/git-head-ref");
}
