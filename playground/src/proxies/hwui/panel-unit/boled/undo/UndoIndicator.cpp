#include <Application.h>
#include "UndoIndicator.h"
#include <http/UndoScope.h>

UndoIndicator::UndoIndicator(const Rect &pos) : Control(pos) {
    Application::get ().getUndoScope ()->onUndoScopeChanged (mem_fun (this, &UndoIndicator::setDirty));
}

bool UndoIndicator::redraw (FrameBuffer &fb) {
    if(Application::get().getUndoScope()->canRedo()) {
        fb.setColor(FrameBuffer::C179);
        fb.drawHorizontalLine(getPosition().getLeft(), getPosition().getTop() + 1, getPosition().getWidth());
        fb.drawVerticalLine(getPosition().getLeft() + 3, getPosition().getTop(), 3);
        Control::redraw(fb);
    }
    return true;
}
