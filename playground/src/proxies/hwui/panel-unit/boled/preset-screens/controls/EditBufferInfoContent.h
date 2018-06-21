#pragma once

#include <proxies/hwui/panel-unit/boled/InfoContent.h>
#include <presets/EditBuffer.h>

class EditBufferInfoContent : public InfoContent {
private:
    typedef InfoContent super;

public:
    EditBufferInfoContent ();
    virtual ~EditBufferInfoContent ();

private:
    void fixLayout();
    bool fillFromEditBuffer (const EditBuffer *eb);
    bool fillDefaults ();
    EditBuffer *getCurrentEditBuffer ();
    void onEditBufferChanged();
    void connectToEditBuffer (EditBuffer* preset);

    sigc::connection m_bankConnection;
    sigc::connection m_editBufferConnection;
};