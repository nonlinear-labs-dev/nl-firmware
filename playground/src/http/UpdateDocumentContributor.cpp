#include "UpdateDocumentContributor.h"
#include "UpdateDocumentMaster.h"
#include "libundo/undo/Transaction.h"


UpdateDocumentContributor::UpdateDocumentContributor (UpdateDocumentContributor *parent) :
      m_parent (parent),
      m_updateIDOnLastChange (1)
{
  if (m_parent)
  {
    m_updateIDOnLastChange = m_parent->getUpdateIDOfLastChange () + 1;
    m_parent->m_children.prepend(this);
  }
}

UpdateDocumentContributor::~UpdateDocumentContributor ()
{
  for(UpdateDocumentContributor *c : m_children)
    c->orphan();

  if (m_parent)
  {
    m_parent->m_children.remove(this);
  }
}

void UpdateDocumentContributor::orphan()
{
  m_parent = nullptr;
}


void UpdateDocumentContributor::adopt(UpdateDocumentContributor *parent)
{
  m_parent = parent;
}

UpdateDocumentContributor::tUpdateID UpdateDocumentContributor::onChange ()
{
  m_updateIDOnLastChange = m_parent->onChange ();
  return m_updateIDOnLastChange;
}

void UpdateDocumentContributor::onRestore()
{
  onChange();
  propagateChangeDownstream();
}

void UpdateDocumentContributor::propagateChangeDownstream ()
{
  for(UpdateDocumentContributor *c : m_children)
  {
    c->setUpdateID(m_updateIDOnLastChange);
    c->propagateChangeDownstream();
  }
}

bool UpdateDocumentContributor::didChangeSince (tUpdateID clientsUpdateID) const
{
  return m_updateIDOnLastChange > clientsUpdateID;
}

UpdateDocumentContributor::tUpdateID UpdateDocumentContributor::getUpdateIDOfLastChange () const
{
  return m_updateIDOnLastChange;
}

void UpdateDocumentContributor::setUpdateID (tUpdateID id)
{
  m_updateIDOnLastChange = id;
}

void UpdateDocumentContributor::incUpdateID ()
{
  m_updateIDOnLastChange++;
}

const UpdateDocumentContributor *UpdateDocumentContributor::getParent () const
{
  return m_parent;
}

UpdateDocumentContributor *UpdateDocumentContributor::getParent ()
{
  return m_parent;
}

UpdateDocumentMaster *UpdateDocumentContributor::getRoot ()
{
  if(m_parent)
    return m_parent->getRoot();

  return static_cast<UpdateDocumentMaster*>(this);
}

const UpdateDocumentMaster *UpdateDocumentContributor::getRoot () const
{
  if(m_parent)
    return m_parent->getRoot();

  return static_cast<const UpdateDocumentMaster*>(this);
}

UNDO::Scope &UpdateDocumentContributor::getUndoScope ()
{
  return getParent ()->getUndoScope ();
}

const UNDO::Scope &UpdateDocumentContributor::getUndoScope () const
{
  return getParent ()->getUndoScope ();
}

