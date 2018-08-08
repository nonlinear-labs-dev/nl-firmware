#pragma once
#include <proxies/hwui/panel-unit/boled/parameter-screens/InfoLayout.h>

class EditBufferInfoLayout : public InfoLayout {
public:
    EditBufferInfoLayout();
    virtual bool onButton(int button, bool down, ButtonModifiers modifiers) override;
    virtual Scrollable *createScrollableContent() override;
    virtual void addModuleCaption() override;
    virtual void addHeadline() override;
    virtual void addInfoLabel() override;
};

