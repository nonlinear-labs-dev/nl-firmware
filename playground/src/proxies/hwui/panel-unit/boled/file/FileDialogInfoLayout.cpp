#include "FileDialogInfoLayout.h"
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ScrollArea.h>
#include <proxies/hwui/panel-unit/boled/file/FileInfoContent.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/InfoLabel.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>
#include "FileNameHeadlineLabel.h"
#include "FileInfoCaption.h"

FileDialogInfoLayout::FileDialogInfoLayout(std::experimental::filesystem::directory_entry file, std::string header) :
    InfoLayout(), m_file(file), m_header(header)
{
}

void FileDialogInfoLayout::addModuleCaption ()
{
  addControl (new FileInfoCaption ("File", Rect (0, 0, 50, 13)));
}

void FileDialogInfoLayout::addHeadline ()
{
  addControl (new FileNameHeadlineLabel (m_header, Rect (50, 0, 156, 13)));
}

void FileDialogInfoLayout::addInfoLabel ()
{
  addControl (new InfoLabel (Rect (206, 0, 50, 13)));
}

Scrollable* FileDialogInfoLayout::createScrollableContent()
{
  return new FileInfoContent(m_file);
}

bool FileDialogInfoLayout::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    switch (i)
    {
    case BUTTON_PRESET:
      Application::get().getHWUI()->undoableSetFocusAndMode(
      { UIFocus::Presets, UIMode::Select });
      return true;


    case BUTTON_STORE:
    case BUTTON_EDIT:
      return true;


    case BUTTON_INFO:
    case BUTTON_ENTER:
      Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().resetOverlay();
      return true;
    }
  }

  return InfoLayout::onButton(i, down, modifiers);
}

