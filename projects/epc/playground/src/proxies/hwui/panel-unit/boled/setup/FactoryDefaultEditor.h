#pragma once

#include <filesystem>
#include "proxies/hwui/controls/ControlWithChildren.h"
#include "SetupEditor.h"

class FactoryDefaultEditor : public ControlWithChildren, public SetupEditor {
public:
    FactoryDefaultEditor();
    ~FactoryDefaultEditor() override = default;
    void setPosition(const Rect &) override;
    bool onButton(Buttons i, bool down, ButtonModifiers modifiers) override;

private:
    void addLabel(const Glib::ustring &text);
};
