#pragma once

#include <presets/EditBuffer.h>
#include "EditPresetInfoLayout.h"

class EditBufferEditLayout : public EditPresetInfoLayout {
public:
    EditBufferEditLayout ();
    void commit (const Glib::ustring &comment);
    virtual bool onButton(int button, bool down, ButtonModifiers modifiers) override;
};
