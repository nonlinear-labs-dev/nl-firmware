#include <xml/BinaryWriter.h>

BinaryWriter::BinaryWriter (shared_ptr<OutStream> out) :
    Writer (out)
{
}

BinaryWriter::~BinaryWriter ()
{
}

void BinaryWriter::implWriteTextElement (const Glib::ustring &name, const Glib::ustring &text,
                                         const initializer_list<Attribute> &attributes)
{
  writeToStream ("X", 1);
  writeRawString (name);
  writeRawString (text);
  writeRawAttributes (attributes);
}

void BinaryWriter::writeRawAttributes (const initializer_list<Attribute>& attributes)
{
  uint32_t numAttributes = attributes.size ();
  writeToStream ((const char*) &numAttributes, 4);

  for (auto& a : attributes)
    implWriteAttribute (a);
}

void BinaryWriter::writeRawString (const Glib::ustring &str)
{
  uint32_t len = str.raw ().size ();
  writeToStream ((const char*) &len, 4);
  writeToStream (str.raw ().data (), len);
}

void BinaryWriter::implWriteTag (const Glib::ustring &name, const initializer_list<Attribute> &attributes, const tTagContentWriter &w)
{
  writeToStream ("T", 1);
  writeRawString (name);
  writeRawAttributes (attributes);
  w ();
  writeToStream ("t", 1);
}

void BinaryWriter::implWriteAttribute (const Attribute &a)
{
  writeToStream ("A", 1);
  writeRawString (a.getName ());
  writeRawString (a.getValue ());
}
