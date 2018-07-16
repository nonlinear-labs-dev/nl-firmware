#pragma once

#include "DisplayEditor.h"

class UISoftwareVersionEditor : public DisplayEditor {
protected:
    Glib::ustring m_branch, m_commitCount, m_head;
public:
    UISoftwareVersionEditor();
    ~UISoftwareVersionEditor() = default;
    void prepareData();
    virtual void setPosition (const Rect &) override;
    const vector<ustring> getDisplayStrings() const;
};
