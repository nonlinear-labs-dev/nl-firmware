#pragma once

#include "presets/AttributesOwner.h"
#include "Serializer.h"

class AttributesOwnerSerializer : public Serializer
{
  public:
    AttributesOwnerSerializer (AttributesOwner *owner);

    static Glib::ustring getTagName();

  private:
    void writeTagContent(Writer &writer) const;
    void readTagContent(Reader &reader) const;

    AttributesOwner *m_owner;
};
