#pragma once

#include "playground.h"
#include "XmlClass.h"
#include "OutStream.h"
#include "Writer.h"
#include <functional>

class XmlWriter : public Writer
{
 private:
  typedef Writer super;

 public:
  XmlWriter(std::shared_ptr<OutStream> out);
  virtual ~XmlWriter();

 private:
  virtual void implWriteTextElement(const Glib::ustring &name, const Glib::ustring &text,
                                    const std::initializer_list<Attribute> &attributes) override;
  virtual void implWriteTag(const Glib::ustring &name, const std::initializer_list<Attribute> &attributes,
                            const tTagContentWriter &w) override;
  virtual void implWriteAttribute(const Attribute &a) override;

 private:
  size_t m_stackSize = 0;
};
