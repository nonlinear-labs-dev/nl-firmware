#include "Reader.h"
#include "serialization/Serializer.h"
#include "Attributes.h"

Reader::Reader(InStream &in, UNDO::Transaction *transaction)
    : m_in(in)
    , m_transaction(transaction)
    , m_treeDepth(0)
{
}

Reader::~Reader()
{
}

void Reader::onTextElement(size_t nameHash, tTextElementCB cb)
{
  m_stack.back().m_textCallbacks[nameHash] = cb;
}

void Reader::onTextElement(const Glib::ustring &name, tTextElementCB cb)
{
  onTextElement(m_hash(name), cb);
}

void Reader::onTag(size_t nameHash, tTagElementCB cb)
{
  m_stack.back().m_tagCallbacks[nameHash] = cb;
}

void Reader::onTag(const Glib::ustring &name, tTagElementCB cb)
{
  onTag(m_hash(name), cb);
}

void Reader::loadTextElement(size_t nameHash, Glib::ustring &target)
{
  onTextElement(nameHash, [&](const Glib::ustring &text, const Attributes &attr) mutable {
    auto scope = UNDO::createSwapData(text);
    getTransaction()->addSimpleCommand([=, &target](UNDO::Command::State) mutable { scope->swapWith(target); });
  });
}

void Reader::loadTextElement(size_t nameHash, std::string &target)
{
  onTextElement(nameHash, [&](const Glib::ustring &text, const Attributes &attr) mutable {
    auto scope = UNDO::createSwapData(text.raw());
    getTransaction()->addSimpleCommand([=, &target](UNDO::Command::State) mutable { scope->swapWith(target); });
  });
}

void Reader::loadTextElement(const Glib::ustring &name, Glib::ustring &target)
{
  loadTextElement(m_hash(name), target);
}

void Reader::loadTextElement(const Glib::ustring &name, std::string &target)
{
  loadTextElement(m_hash(name), target);
}

void Reader::preProcess()
{
  m_stack.push_back(StackEntry(m_treeDepth));
}

int Reader::getFileVersion() const
{
  return m_fileVersion;
}

bool Reader::onStartElement(size_t nameHash, const Attributes &attributes)
{
  if(m_treeDepth == 0 && m_fileVersion == 0)
  {
    auto strVersion = attributes.get("version", "0");
    m_fileVersion = std::stoi(strVersion);
    m_versionCheck = m_sigFileVersionRead.send(m_fileVersion);
  }

  if(m_versionCheck == FileVersionCheckResult::OK)
  {
    StackEntry &entry = m_stack.back();
    auto it = entry.m_tagCallbacks.find(nameHash);

    if(it != entry.m_tagCallbacks.end())
    {
      entry.serializer = StackEntry::tSerializer(it->second(attributes));

      if(entry.serializer)
        entry.serializer->read(*this);

      m_treeDepth++;

      return true;
    }

    m_treeDepth++;
  }
  return false;
}

void Reader::onEndElement()
{
  if(m_versionCheck == FileVersionCheckResult::OK)
  {
    m_treeDepth--;

    if(m_stack.back().m_treeDepth == m_treeDepth)
      m_stack.pop_back();
  }
}

void Reader::onTextElement(size_t nameHash, const Attributes &attributes, const Glib::ustring &text)
{
  if(m_versionCheck == FileVersionCheckResult::OK)
  {
    const StackEntry &entry = m_stack.back();
    auto it = entry.m_textCallbacks.find(nameHash);

    if(it != entry.m_textCallbacks.end())
    {
      it->second(text, attributes);
    }
  }
}

UNDO::Transaction *Reader::getTransaction()
{
  return m_transaction;
}

InStream &Reader::getInStream()
{
  return m_in;
}

sigc::connection Reader::onFileVersionRead(sigc::slot<FileVersionCheckResult, int> slot)
{
  return m_sigFileVersionRead.connect(slot);
}

void Reader::onReadFinished(Reader::tReadFinishedCB cb)
{
  m_readFinishedCB = std::move(cb);
}

void Reader::doOnReadFinish()
{
  if(m_readFinishedCB)
  {
    m_readFinishedCB(*this);
    m_readFinishedCB = nullptr;
  }
}
