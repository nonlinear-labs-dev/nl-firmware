#include <proxies/hwui/panel-unit/boled/preset-screens/controls/EditBufferInfoContent.h>
#include <Application.h>
#include "EditBufferInfoLayout.h"
#include "proxies/hwui/panel-unit/boled/preset-screens/controls/EditBufferInfoCaption.h"
#include "EditBufferEditLayout.h"
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/InfoLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/PresetNameHeadlineLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/EditBufferNameHeadlineLabel.h>

EditBufferInfoLayout::EditBufferInfoLayout() : InfoLayout() {
}

bool EditBufferInfoLayout::onButton(int button, bool down, ButtonModifiers modifiers) {
  if(down) {
    switch(button) {
      case BUTTON_INFO:
        Application::get().getHWUI()->setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Select));
        return true;
      case BUTTON_EDIT:
        Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().reset(new EditBufferEditLayout());
        return true;
    }
  }
  return InfoLayout::onButton(button, down, modifiers);
}

Scrollable *EditBufferInfoLayout::createScrollableContent() {
 return new EditBufferInfoContent();
}

void EditBufferInfoLayout::addModuleCaption() {
  addControl(new EditBufferInfoCaption(Rect(0,0,64,13)));
}

void EditBufferInfoLayout::addHeadline() {
  addControl (new EditBufferNameHeadlineLabel (Rect (64, 0, 128, 13)));
}

void EditBufferInfoLayout::addInfoLabel() {
  addControl (new InfoLabel (Rect (192, 0, 64, 13), "2/2"));
}