#pragma once

#include "DisplayEditor.h"

class UISoftwareVersionEditor : public DisplayEditor {
protected:
    Glib::ustring m_branch, m_commitCount, m_head;
public:
    UISoftwareVersionEditor();
    virtual void setPosition (const Rect &) override;
};
